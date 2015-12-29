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
				occurrances.insert(occurrances.end(), ocs.begin(), ocs.end());
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
		int hash;
		// int colision;
		vector<wanalysis*> was;
	};
	
}

namespace ds
{
	
	class watable : public analysis
	{
		private:
			hash<string>inner_hash;
			int hashcount;
			vector<datarow> datarows;
			/*
			sort()
			{
				std::sort(datarows.begin(), datarows.end(), watable::compHash);
			}
			*/
			int hash_str(string word)
			{
				return (inner_hash(word) % hashcount);
			}
		public:
			static bool compHash(datarow& r1, datarow& r2)
			{
				return r1.hash < r2.hash;
			}
			wanalysis* find(string word) 
			{
				int index = hash_str(word);
				for(int i = 0; i < datarows[index].was.size(); i++)
				{
					if(datarows[index].was[i]->getWord() == word)
					{
						return datarows[index].was[i];
					}
				}
				return NULL;
			}
			
			watable(vector<wanalysis>wa)
			{
				/*
				for(int i = 0; i < wa.size(); i++)
				{
					datarow dr;
					dr.hash = hash_str(wa[i].getWord());
					dr.wa = &(wa[i]);
					datarows.push_back(dr);
				}
				sort();
				*/
			}
			
			int getWordCount()
			{
				/* TODO (rasekh#1#): Manage hash collision */
				
				return datarows.size();
			}
			
			void insert(wanalysis* wa)
			{	
				int index = hash_str(wa->getWord());
				for(int i = 0; i < datarows[index].was.size(); i++)
				{
					if(datarows[index].was[i]->getWord() == wa->getWord())
					{
						datarows[index].was[i]->merge(wa);
						return;
					}
				}
				datarows[index].was.push_back(wa);
			}
			
			
			save(string path)
			{
				/* TODO (rasekh#1#): Save the ds in a file */
				ds.str_pasth = pasth
			}
			
			watable(string path)
			{
				/* TODO (rasekh#1#): load the ds from the file */
				vector<string>ds::iterator i = v.begin ();

   				while (i != v.end ())
				{
			    	++ i;
  				}

  				ssi = find (v.begin (),v.end (), path);

  			 	if (i != v.end ())
  				{
   	   			int nPosition = distance (v.begin (), i);
				return *i;
				}
			}
			
			/* TODO (rasekh#1#): Get the dictionary from words */
			
			
			watable(int hashcount = 40000)
			{
				this->hashcount = hashcount;
				datarow row;
				for(int i = 0; i < hashcount; i++)
				{
					row.hash = i;
					datarows.push_back(row);
				}
			}
			
			string process(string word, occurrance o)
			{
				wanalysis* wa = new wanalysis(word);
				wa->process(word, o);
				insert(wa);
				return word;
			}
	};
}

#endif
