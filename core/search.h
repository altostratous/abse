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
			
			vector<wanalysis> or_anals(vector<wanalysis>& left, vector<wanalysis>& right)
			{
				vector<wanalysis> res;
				for(int i = 0; i < left.size(); i++)
				{
					
				}
				for(int i = 0; i < left.size(); i++)
				{
					
				}
				return res;
			}
			
			vector<wanalysis> and_anals(vector<wanalysis>& left, vector<wanalysis>& right)
			{
				
			}
		public:
			condition(string cond_string)
			{
			}
			
			vector<wanalysis> filter (watable& wat)
			{
				vector<wanalysis>res;
				if(operand != CONTAINS)
				{
					vector<wanalysis>leftanal, rightanal;
					leftanal = left->filter(wat);
					rightanal = right->filter(wat);
					
					switch(operand)
					{
						case OR:
							return or_anals(rightanal, leftanal);
							break;
						case AND:
							return and_anals(rightanal, leftanal);
							break;
					}
				}
				else
				{
					res.push_back(*wat.find(word));
				}
				return res;
			}
	};
	
	class finder
	{
		public:
			
	};
}

#endif
