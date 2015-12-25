/*
	Purpose: to index the content
*/

#ifndef ABSE_INDEX
#define ABSE_INDEX 1

#include<vector>
#include<set>
#include<string>
#include<algorithm>

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
			
			vector<occurrance> getOccurrances()
			{
				return occurrances;
			}
			
			merge(wanalysis* wa)
			{
				vector<occurrance> ocs = wa->getOccurrances();
				occurrances.insert(ocs.begin(), ocs.begin(), ocs.end());
			}
			
			string getWord()
			{
				return word;
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

using namespace index;
namespace ds
{
	
	struct datarow
	{
		long long hash;
		int colision;
		wanalysis* wa;
	};
	
}

namespace ds
{
	
	class watable
	{
		private:
			hash<string>hash_str;
			vector<datarow> datarows;
			sort()
			{
				std::sort(datarows.begin(), datarows.end(), watable::compHash);
			}
		public:
			static bool compHash(datarow& r1, datarow& r2)
			{
				return r1.hash < r2.hash;
			}
			wanalysis* find(string word) 
			{
				int h = hash_str(word);
				int a = 0;
				int b = datarows.size();
				int i = (a+b)/2;
				while(datarows[i].hash != h)
				{
					if(a == b)
						return NULL;
					if(datarows[i].hash < h)
					{
						b = i;
					}
					else
					{
						a = i;
					}
				}
				return datarows[i].wa;
			}
			
			watable(vector<wanalysis>wa)
			{
				for(int i = 0; i < wa.size(); i++)
				{
					datarow dr;
					dr.hash = hash_str(wa[i].getWord());
					dr.wa = &(wa[i]);
					datarows.push_back(dr);
				}
				sort();
			}
			
			void insert(wanalysis* wa)
			{	
				int h = hash_str(wa->getWord());
				int a = 0;
				int b = datarows.size();
				int i = (a+b)/2;
				while(datarows[i].hash != h)
				{
					if(a == b)
					{
						datarow dr;
						dr.hash = hash_str(wa->getWord()); 
						dr.wa = &(wa[i]);
						datarows.insert(datarows.begin()+a, dr);
						return;
					}
					if(datarows[i].hash < h)
					{
						b = i;
					}
					else
					{
						a = i;
					}
				}
				datarows[i].wa->merge(wa);
			}
			
			
			save(string path)
			{
				cout<<"not implemented"<<endl;
			}
			
			watable(string path)
			{
				cout<<"not implemented"<<endl;
			}
	};
}

#endif
