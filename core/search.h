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
			condition(string cond_string)
			{
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
