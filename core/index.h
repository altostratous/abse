/*
	Purpose: to index the content
*/

#ifndef ABSE_INDEX
#define ABSE_INDEX 1

#include<vector>
#include<string>

#include "ds.h"
#include "file.h"

using namespace ds;
using namespace std;

namespace index
{
	class wanalysis
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
			
			wanalysis(string word)
			{
				this->word = word;
			}
	};
}

#endif
