/*
	Purpose: to index the content
*/

#ifndef ABSE_INDEX
#define ABSE_INDEX 1

#include<vector>
#include<set>
#include<string>
#include<iostream>
#include<sstream>
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
			
			merge(occurrance o)
			{
				occurrances.push_back(o);
			}
			
			merge(vector<occurrance>& occurrs)
			{
				occurrances.insert(occurrances.end(), occurrs.begin(), occurrs.end());
			}
			
			// this merges the argument if its word is different to this wanalysis using "or" logic in file names
			ormerge(wanalysis* wa)
			{
				if(wa->getWord() != this->getWord())
				{
					word = "(" + word + " OR " + wa->getWord() + ")";
					vector<occurrance> ocs = wa->getOccurrances();
					occurrances.insert(occurrances.end(), ocs.begin(), ocs.end());
				}
			}
			
			// this merges the argument if its word is different to this wanalysis using "and" logic in file names
			andmerge(wanalysis* wa)
			{
				if(wa->getWord() != this->getWord())
				{
					word = "(" + word + " OR " + wa->getWord() + ")";
					vector<occurrance> ocs, waocs = wa->getOccurrances();
					for(int i = 0; i < occurrances.size(); i++)
					{
						for(int j = 0; j < waocs.size(); j++)
						{
							if(occurrances[i].file_id == waocs[j].file_id)
							{
								ocs.push_back(occurrances[i]);
								ocs.push_back(waocs[j]);
							}
						}
					}
					occurrances = ocs;
				}
			}
			
			notmerge(wanalysis* wa)
			{
				vector<occurrance> ocs, waocs = wa->getOccurrances();
				for(int i = 0; i < occurrances.size(); i++)
				{
					bool test = false;
					for(int j = 0; j < waocs.size(); j++)
					{
						if(waocs[j].file_id == occurrances[i].file_id)
						{
							test = true;
							break;
						}
					}
					if(!test)
						ocs.push_back(occurrances[i]);
				}
				occurrances = ocs;
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
			wanalysis* all;
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
				return new wanalysis(word);
			}
			
			wanalysis* getAll() 
			{
				return all;
			}
			
			int getWordCount()
			{
				/* TODO (rasekh#1#): Manage hash collision */
				
				return datarows.size();
			}
			
			void insert(wanalysis* wa)
			{	
				all->merge(wa);
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
			
			/* TODO (rasekh#1#): Get the dictionary from words */
			
			
			watable(int hashcount = 40000)
			{
				all  = new wanalysis("");
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
			
			int getOccurranceCount()
			{
				int counter = 0;
				for(int i = 0; i < datarows.size(); i++)
				{
					for(int j = 0; j < datarows[i].was.size(); j++)
					{
						counter += datarows[i].was[j]->getCount();
					}
				}
				return counter;
			}
	};
}

#endif
