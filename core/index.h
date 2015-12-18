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
	class analysis
	{
		public:
			virtual string process(string input, occurrance o) = 0;
	};
	
	class wanalysis : public analysis
	{
		private:
			vector<occurrance> occurrances;
			string word;
		public:
			int getCount()
			{
				return occurrances.size();
			}
			
			string process(string input, occurrance o)
			{
				//cout<<input<<endl;
				if(input == word)
				{
					occurrances.insert(occurrances.end(), o);
				}
				return input + " ";
			}
			
			wanalysis(string word)
			{
				this->word = word;
			}
			
			occurrance getOccurrance(int id)
			{
				return *(occurrances.begin() + id);
			}
	};
}

#endif
