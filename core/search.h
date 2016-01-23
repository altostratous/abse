/*
	Purpose: to do search through data structures
*/

#ifndef ABSE_SEARCH
#define ABSE_SEARCH 1

#include<string>
#include<vector>

#include "util.h"

using namespace std;
using namespace ds;
using namespace index;

namespace search
{
	enum boolean_operator
	{
		CONTAINS,
		AND,
		OR,
		NOT,
		ALL
	};
	
	enum condition_sort_logic
	{
		INFIMUM_BASED,
		SUPRIMUM_BASED
	};
	
	class condition
	{
		private:
			boolean_operator operand;
			condition* left;
			condition* right;
			string word;
			void sort(vector<condition**> list, condition_sort_logic logic, watable& wat)
			{
				bool test = false;
				for(int i = 0; i < (((int)list.size()) - 1); i++)
				{
					for(int j = i + 1; j < list.size(); j++)
					{
						if(comp(*(list[i]), *(list[j]), logic, wat) > 0)
						{
							condition* temp = *(list[j]);
							*(list[j]) = *(list[i]);
							*(list[i]) = temp;
							test = true;
						}
					}
				}
				if(test)
				{
					sort(list, logic, wat);
				}
			}
			int comp(condition* c1, condition* c2, condition_sort_logic logic, watable& wat)
			{
				// to prevent condition recursion
				if(c1->left == c2 || c1->right == c1 || c2->left == c1 || c2->right == c1)
					return -1;
				int inf2 = c2->infimum(wat);
				int inf1 = c1->infimum(wat);
				int sup2 = c2->suprimum(wat);
				int sup1 = c1->suprimum(wat);
				if(logic == INFIMUM_BASED)
				{
					if(inf2 != inf1)
						return inf2 - inf1;
					else
						if(sup1 == sup2)
							return 0;
						else
							return sup2 - sup1;
				}
				
				if(logic == SUPRIMUM_BASED)
				{
					if(sup1 != sup2)
						return sup1 - sup2;
					else
						if(inf1 == inf2)
							return 0;
						else
							return inf1 - inf2;
				}
				
			}
			void binarizeNOTs()
			{
				if(operand == CONTAINS || operand == ALL)
				{
					return;
				}
				if(operand == AND)
				{
					if(left->operand == NOT)
					{
						operand = NOT;
						condition* temp = left->right;
						left = right;
						right = temp;
						left->binarizeNOTs();
					}
				}
				else
				{
					left->binarizeNOTs();
					right->binarizeNOTs();
				}
			}
			// iptimizes the query unless the NOTS
			void inner_optimize(watable& wat)
			{
				// find pointers to the conditions having same level to this condition
				// it means:
				//		1- NOT, AND, ALL
				// 		2- OR, ALL
				
				// the isolevel conditions
				if(operand == CONTAINS || operand == ALL)
					return;
				vector<condition**> isolevels = getIsoLevels();
				left->inner_optimize(wat);
				right->inner_optimize(wat);
				
				if(operand == OR)
				{
					sort(isolevels, INFIMUM_BASED, wat);
					if(left->operand == ALL)
					{
						operand = ALL;
					}
				}
				if(operand == AND)
				{
					/* DONE (asgari#1#): Complete optimization on NOTS and ANDS
 */
					
					sort(isolevels, SUPRIMUM_BASED, wat);
					
					// optimize NOTs
					//binarizeNOTs();
					// remove alls 
					if(left->operand == ALL)
					{
						this->operand = right->operand;
						this->left = right->left;
						this->right = right->right;
					}
					if(right->operand == ALL)
					{
						this->operand = left->operand;
						this->left = left->left;
						this->right = left->right;
					}
					
				}
				
				/* DONE (asgari#1#): optimize non-isolevels */
				
			}
		public:
			vector<condition**> getIsoLevels()
			{
				vector<condition**> res;
				if(operand == ALL || operand == CONTAINS || operand == NOT)
				{
					return res;
				}
				if(right->operand == operand)
				{
					vector<condition**> rightside = right->getIsoLevels();
					res.insert(res.end(), rightside.begin(), rightside.end());
				}
				if(right->operand == CONTAINS || right->operand == ALL || right->operand == NOT)
				{
					if(operand != NOT)
						res.push_back(&right);
				}
				if(left->operand == operand)
				{
					vector<condition**> leftside = left->getIsoLevels();
					res.insert(res.end(), leftside.begin(), leftside.end());
				}
				if(left->operand == CONTAINS || left->operand == ALL || left->operand == NOT)
				{
					res.push_back(&left);
				}
				return res;
			}
			void optimize(watable& wat)
			{
				inner_optimize(wat);
				binarizeNOTs();
			}
			
			
			condition(string cond, bool steminput = false, spellcheck* sp = NULL)
			{
				// do some normalization
				while(cond[0] == ' ' && cond.length() > 0)
					cond = cond.substr(1);
				if(cond.length() == 0)
				{
					this->operand = ALL;
					return;
				}
				while(cond[cond.length() - 1] == ' ')
					cond = cond.substr(0, cond.length() - 1);
				for(int i = 0; i < cond.length() - 1; i++)
				{
					if(cond[i] == ' ' && cond[i + 1] == ' ')
					{
						cond = cond.substr(0, i) + cond.substr(i + 1);
					}
				}
				/* DONE (asgari#1#): to parse the string to formatted conditions */
				
				// find the search phrases
				int par_depth = 0;
				int depth = 0;
				vector<occurrance> phrases;
				occurrance par_phrase;
				occurrance phrase;
				phrase.index = 0;
				for(int i = 0; i < cond.length(); i++)
				{
					if(cond[i] == ' ' || cond[i] == '(' || cond[i] == ')' || i == 0 || i == cond.length() - 1)
					{
						if(i == cond.length() - 1 && (cond[i] != '(' && cond[i] != ')'))
						{
							phrase.length = cond.length() - phrase.index;
						}
						else
						{
							phrase.length = i - phrase.index; 
						}
						if(phrase.length > 1 || (cond[phrase.index] != '(' && cond[phrase.index] != ')'))
						{
							phrases.push_back(phrase);
						}
						if(i == 0 && (cond[i] != '(' && cond[i] != ')'))
						{
							phrase.index = 0;
						}
						else
						{
							phrase.index = i + 1;
						}
					}
					if(cond[i] == '(')
					{
						if(par_depth == 0)
						{
							par_phrase.index = i + 1;
						}
						par_depth++;
					}
					if(cond[i] == ')')
					{
						par_depth--; 
						if(par_depth == 0)
						{
							int end = i - 1;
							par_phrase.length = end - par_phrase.index + 1;
							phrases.push_back(par_phrase);
						}
					}
				}
				
				// remove sub phrases
				for(int i = 0; i < phrases.size(); i++)
				{
					for(int j = 0; j < phrases.size(); j++)
					{
						if(phrases[j].index >= phrases[i].index && phrases[j].index + phrases[j].length <= phrases[i].index + phrases[i].length && i != j)
						{
							phrases[j].length = 0;
						}
					}
				}
				for(int i = phrases.size() - 1; i >= 0; i--)
				{
					if(phrases[i].length == 0)
						phrases.erase(phrases.begin() + i);
				}
				
				// put AND instead of spaces
				for(int i = 0; i < phrases.size() - 1; i++)
				{
					string left_phrase = cond.substr(phrases[i].index, phrases[i].length);
					string right_phrase = cond.substr(phrases[i + 1].index, phrases[i + 1].length);
					if(
						(right_phrase != "AND" && right_phrase != "OR") &&
						(left_phrase != "AND" && left_phrase != "OR") &&
						left_phrase != "NOT"
					)
					{
						condition* constructed = new condition(cond.substr(0, phrases[i + 1].index) + " AND " + cond.substr(phrases[i + 1].index), steminput, sp);
						this->operand = constructed->operand;
						this->word = constructed->word;
						this->left = constructed->left;
						this->right = constructed->right;
						delete constructed;
						return;
					}
				}
				
				// find the first OR if exists
				for(int i = 0; i < phrases.size(); i++)
				{
					string phrase_str = cond.substr(phrases[i].index, phrases[i].length);
					if(phrase_str == "OR")
					{
						// continue the recursion of conditions
						/* DONE (asgari#1#): normalize the input condition for duplicate 
						                     spaces and etc. */
						
						this->left = new condition(cond.substr(0, phrases[i].index), steminput, sp);
						this->right = new condition(cond.substr(phrases[i].index + 2), steminput, sp);
						this->operand = OR;
						return;
					}
				}
				
				// find the first AND if exists
				for(int i = 0; i < phrases.size(); i++)
				{
					string phrase_str = cond.substr(phrases[i].index, phrases[i].length);
					if(phrase_str == "AND")
					{
						// continue the recursion of conditions
						/* DONE (asgari#1#): normalize the input condition for duplicate 
						                     spaces and etc. */
						
						this->left = new condition(cond.substr(0, phrases[i].index), steminput, sp);
						this->right = new condition(cond.substr(phrases[i].index + 3), steminput, sp);
						this->operand = AND;
						return;
					}
				}
				
				
				// find the first NOT if exists
				for(int i = 0; i < phrases.size(); i++)
				{
					string phrase_str = cond.substr(phrases[i].index, phrases[i].length);
					if(phrase_str == "NOT")
					{
						// continue the recursion of conditions
						/* DONE (asgari#1#): normalize the input condition for duplicate 
						                     spaces and etc. */
						
						this->left = new condition(cond.substr(0, phrases[i].index), steminput, sp);
						this->right = new condition(cond.substr(phrases[i].index + 3), steminput,  sp);
						this->operand = NOT;
						return;
					}
				}
				
				// if there's no operand or parantheses
				if(cond.length() == phrases[0].length)
				{
					this->operand = CONTAINS;
					
					// do recommandation
					if(sp != NULL)
					{
						cond = sp->recommand(cond);
					}
					
					if(steminput)
						this->word = porter::stem(cond);
					else
						this->word = cond;
					return;
				}
				
				// otherwise
				condition* constructed;
				constructed = new condition(cond.substr(phrases[0].index, phrases[0].length), steminput, sp);
				/*if(phrases.size() == 1)
				{
					constructed = new condition(cond.substr(phrases[0].index, phrases[0].length));
				}
				else
				{
					string and_standared_cond = cond.substr(phrases[0].index, phrases[0].length);;
					for(int i = 1; i < phrases.size(); i++)
					{
						and_standared_cond += " AND " + cond.substr(phrases[i].index, phrases[i].length);
					}
					constructed = new condition(and_standared_cond);
				}*/
				this->operand = constructed->operand;
				this->word = constructed->word;
				this->left = constructed->left;
				this->right = constructed->right;
				delete constructed;
			}
			
			wanalysis* filter (watable& wat)
			{
				if(operand == ALL)
					return wat.getAll();
				wanalysis* res;
				if(operand != CONTAINS)
				{
					wanalysis
						*leftanal = left->filter(wat),
						*rightanal = right->filter(wat);
					
					switch(operand)
					{
						case OR:
							leftanal->ormerge(rightanal);
							return leftanal;
							break;
						case AND:
							leftanal->andmerge(rightanal);
							return leftanal;
							break;
						case NOT:
							leftanal->notmerge(rightanal);
							return leftanal;
							break;
					}
				}
				else
				{
					res = new wanalysis(word);
					*res = *wat.find(word);
				}
				return res;
			}
			
			string toString()
			{
				if(operand == CONTAINS)
					return word;
				if(operand == ALL)
					return "";
				string opstr = "AND";
				if(operand == NOT)
					opstr = "NOT";
				if(operand == OR)
					opstr = "OR";
				return "(" + left->toString() + " " + opstr + " " + right->toString() + ")";
			}
			
			int infimum(watable& wat)
			{
				if(operand == AND)
				{
					return 0;
				}
				if(operand == ALL)
				{
					return wat.getAll()->getCount();
				}
				if(operand == OR)
				{
					return left->infimum(wat) + right->infimum(wat);
				}
				if(operand == CONTAINS)
				{
					return wat.find(word)->getCount();
				}
				if(operand == NOT)
				{
					if(left->operand == ALL)
						return  wat.getAll()->getCount() - right->suprimum(wat);
					return left->infimum(wat) - right->suprimum(wat);
				}
			}
			
			int suprimum(watable& wat)
			{
				
				if(operand == AND)
				{
					return left->suprimum(wat) + right->suprimum(wat);
				}
				if(operand == ALL)
				{
					return wat.getAll()->getCount();
				}
				if(operand == OR)
				{
					return left->suprimum(wat) + right->suprimum(wat);
				}
				if(operand == CONTAINS)
				{
					return wat.find(word)->getCount();
				}
				if(operand == NOT)
				{
					if(left->operand == ALL)
					{
						return  wat.getAll()->getCount() - right->infimum(wat);
					}
					return left->suprimum(wat);
				}
			}
			
			/*
				// here 2 funtions are added to return the suprimum of a condition
				// in fact two functions are written.one of them can be true				
				int suprimum()
				{
					if(wanalysis.getCount().word1 > wanalysis.getCount().word2)
						return wanalysis.getCount().word1;
					if(wanalysis.getCount().word1 <= wanalysis.getCount().word2)
						return wanalysis.getCount().word2;
				}
				
				int suprimum()
				{
					if(datarows.word1.size() > datarows.word2.size())
						return	datarows.word1.size();
					if(datarows.word1.size() <= datarows.word2.size())
						return 	datarows.word2.size();
				}
				
				
				// start of lower part adde by rasekh
				
				void andsort(condition * c)
				{
					if(operand == AND)
					{
						if(*leftanal == NULL || *rightanal == NULL)
								return;
						if(*leftanal.suprimum > c.suprimum) // new added
							swap(*c , *leftanal);
						if(*leftanal.suprimum > c.suprimum) // new added
							swap(*c , *leftanal);
						if(*leftanal.suprimum > *rightanl.suprimum) // 	first version:	swap(*leftanal.suprimum , *rightanl.suprimum);
							swap(*leftanal , *rightanal);
						andsort(conditon * c.leftanal);
					}
				}
				
				void orsort(condition * c)
				{
					if(operand == OR)
					{
						if(*leftanal == NULL || *rightanal == NULL)
							return;
						if(*leftanal.suprimum > c.suprimum) // new added
							swap(*c , *leftanal);
						if(*leftanal.suprimum > c.suprimum) // new added
							swap(*c , *leftanal);
						if(*leftanal.suprimum > *rightanl.suprimum)
							swap(*leftanal, *rightanl); // 	first version:	swap(*leftanal.suprimum , *rightanl.suprimum);
						orsort(conditon * c.leftanal);
					}
				}
				// 	end of this part
				// note : not function isn't added
			*/	
	};
}

#endif
