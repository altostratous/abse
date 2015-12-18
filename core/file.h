/*
	Purpose: to access files efficiently and perform processes
	Multithreadings should be done here
	Here we will imlement functions to iterate files and perform any process
*/

#ifndef ABSE_FILE
#define ABSE_FILE 1

#include<string>
#include<vector>
#include<fstream>
#include<dirent.h>

#include "index.h"

using namespace std;
using namespace index;

namespace disk
{
	
//	class tprocessor
//	{
//		public:
//			string process(string input){return "";}
//		 
//	};
	
	/*
		This class should have these members:
			1- a function to iterate files word by word like this 
				iterate(pointer to process function(string, fileid, index), output)
			2- list of files to be processed
			3- a funciton to iterate files paragraph by paragraph
			4- characters to be word seperators
			5- characters to be paragraph seperators
			optionals:
			6- function to process files by grams as given gram scale
	*/
	class file
	{
		/* TODO (rasekh#1#): Implement properties. */
		/* TODO (asgari#1#): Implement iterating functions. */
		private:
			vector<string>filenames;
		public:
			file(vector<string>filenames)
			{
				this->filenames = filenames;
			}
			
			void iterate(analysis& p)
			{
				vector<string>::iterator i = filenames.begin();
				while(i != filenames.end())
				{
					ifstream fin((*i).c_str());
					cout<<*i<<" Opened:"<<endl;
					string word = "";
					while(!fin.eof())
					{
						char c;
						fin.get(c);
						if(c != ' ')
							word += c;
						else
						{
							p.process(word);
							word = "";
						}
					}
					fin.close();
					++i;
				}
			}
	};
	
	/*
		This class is for getting files in a direcory
	*/
	class dir
	{
		public:
			static vector<string> getFiles(string directory)
			{
				vector<string> res;
				DIR *pDIR;
		        struct dirent *entry;
		        if(pDIR=opendir(directory.c_str()))
				{
		                while(entry = readdir(pDIR))
						{
		                        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
		                        	res.insert(res.end(),directory + "\\" + entry->d_name);
		                }
		                closedir(pDIR);
        		}
        		return res;
			}
	};
}

#endif
