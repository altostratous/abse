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
		OR
	};
	
	class condition
	{
		private:
			boolean_operator operand;
			condition* left;
			condition* right;
			string word;
		public:
			condition(string cond)
			{
				// do some normalization
				while(cond[0] == ' ')
					cond = cond.substr(1);
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
					}
				}
				else
				{
					res = new wanalysis(word);
					*res = *wat.find(word);
				}
				return res;
			}
	};
}

#endif
