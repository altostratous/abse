/*
	Purpose: to index the content
*/

#ifndef ABSE_INDEX
#define ABSE_INDEX 1

#include<vector>
#include<set>
#include<string>

#include "ds.h"
#include "file.h"
#include "language.h"

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
				return input;
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
	
	
	/*
		This class provides replacing a word with another
		in the word sequence.
	*/
	class replacer : public analysis
	{
		private:
			string src, dst;
		public:
			string process(string input, occurrance o)
			{
				if(input == src)
					return dst;
				else
					return input;
			}
			
			replacer(string tofind, string toreplace)
			{
				this->src = tofind;
				this->dst = toreplace;
			}
	};
	
	class stemmer : public analysis
	{
		public:
			string process(string input, occurrance o)
			{
				return porter::stem(input);
			}
	};
	
	class lowerer : public analysis
	{
		public:
			string process(string input, occurrance o)
			{
				for(int i = 0; i < input.length(); i++)
				{
					input[i] = tolower(input[i]);
				}
				return input;
			}
	};
	
	class removestopwords : public analysis
	{
		private:
			set<string>stopwords;
		public:
			string process(string input, occurrance o)
			{
				if(stopwords.find(input) != stopwords.end())
				{
					return "";
				}
				else
				{
					return input;
				}
			}
			
			removestopwords(string stopwords)
			{
				string word = "";
				for(int i = 0; i < stopwords.length(); i++)
				{
					if(stopwords[i] == ' ')
					{
						this->stopwords.insert(this->stopwords.end(), word);
						word = "";
					}
					else
					{
						word += stopwords[i];
					}
				} 
			}
	};
	
	class normalizer : public analysis
	{
		private:
			string stopwords;
		public:
			normalizer(string stopwords)
			{
				this->stopwords = stopwords;
			}
			string process(string input, occurrance o)
			{
				lowerer l;
				stemmer s;
				removestopwords r(stopwords);
				return r.process(s.process(l.process(input, o), o), o);
			}
	};
}

#endif
