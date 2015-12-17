/*
	Purpose: to index the content
*/

#ifndef ABSE_INDEX
#define ABSE_INDEX 1

#include<vector>
#include<string>

#include "ds.h"

using namespace ds;
using namespace std;

namespace index
{
	class wcounter
	{
		private:
			vector<occurrance> occurrances;
			string word;
		public:
			int getCount()
			{
				return occurrances.size();
			}
			
			string process(string input)
			{
				if(input == word)
				{
					occurrance o;
					occurrances.insert(occurrances.end(), o);
				}
				return input;
			}
			
			wcounter(string word)
			{
				this->word = word;
			}
	};
}

#endif
