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
	
	class condition
	{
		private:
			boolean_operator operand;
			condition* left;
			condition* right;
			string word;
		public:
			condition(string cond, bool steminput = false)
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
				/* TODO (asgari#1#): to parse the string to formatted conditions */
				
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
				
				// find the first NOT if exists
				for(int i = 0; i < phrases.size(); i++)
				{
					string phrase_str = cond.substr(phrases[i].index, phrases[i].length);
					if(phrase_str == "NOT")
					{
						// continue the recursion of conditions
						/* TODO (asgari#1#): normalize the input condition for duplicate 
						                     spaces and etc. */
						
						this->left = new condition(cond.substr(0, phrases[i].index));
						this->right = new condition(cond.substr(phrases[i].index + 3));
						this->operand = NOT;
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
						/* TODO (asgari#1#): normalize the input condition for duplicate 
						                     spaces and etc. */
						
						this->left = new condition(cond.substr(0, phrases[i].index));
						this->right = new condition(cond.substr(phrases[i].index + 3));
						this->operand = AND;
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
						/* TODO (asgari#1#): normalize the input condition for duplicate 
						                     spaces and etc. */
						
						this->left = new condition(cond.substr(0, phrases[i].index));
						this->right = new condition(cond.substr(phrases[i].index + 2));
						this->operand = OR;
						return;
					}
				}
				
				// if there's no operand or parantheses
				if(cond.length() == phrases[0].length)
				{
					this->operand = CONTAINS;
					if(steminput)
						this->word = porter::stem(cond);
					else
						this->word = cond;
					return;
				}
				
				// otherwise
				condition* constructed = new condition(cond.substr(phrases[0].index, phrases[0].length));
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
