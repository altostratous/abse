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
#include<sys/types.h>
				

#include "index.h"
#include "ds.h"

using namespace std;
using namespace index;
using namespace ds;

namespace disk
{	
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
			string wordSeperators;
		public:
			file(vector<string>filenames)
			{
				this->filenames = filenames;
				wordSeperators = " ";
			}
			
			void setWordSeperators(string ws)
			{
				wordSeperators = ws;
			}
			
			void iterate(analysis& p, string output="")
			{
				ofstream fout;
				if(output != "")
				{
					fout.open(output.c_str());
				}
				vector<string>::iterator i = filenames.begin();
				while(i != filenames.end())
				{
					ifstream fin((*i).c_str());
					int paragraph_id = 0;
					string word = "";
					while(!fin.eof())
					{
						char c;
						fin.get(c);
						if(wordSeperators.find(c)==string::npos && c!='\n')
							word += c;
						else
						{
							if(c=='\n')
								paragraph_id++;
							occurrance o;
							o.file_id = distance(filenames.begin(), i);
							o.index = ((int)fin.tellg() - word.length());
							o.length = word.length();
							o.paragraph_id = paragraph_id;
							fout << p.process(word, o);
							word = "";
						}
					}
					fin.close();
					++i;
				}
				if(output != "")
				{
					fout.close();
				}
			}
			
			string look(occurrance o)
			{
				string path = *(filenames.begin() + o.file_id);
				ifstream fin(path.c_str());
				fin.seekg(o.index -1, ios::beg);
				string word;
				char c;
				for(int i = 0; i < o.length; i++)
				{
					fin.get(c);
					word += c;
				}
				return word;
			}
			
			string getFileNameById(int id)
			{
				return filenames[id];
			}
	};
	
	/*
		This class is for getting files in a direcory
	*/
	class dir
	{
		public:
			static vector<string> getFiles(string directory, bool recursive = false)
			{
				vector<string> res;
				DIR *pDIR;
		        struct dirent *entry;
		        if(pDIR=opendir(directory.c_str()))
				{
	                while(entry = readdir(pDIR))
					{
						 if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
						{
							string path = directory + "\\" + entry->d_name;
							if(dir::isFile(path.c_str()))
							{
		                       	res.insert(res.end(),path);
		                	}
		                	else
		                	{
		                		if(recursive)
		                		{
		                			vector<string>subfiles = dir::getFiles(path, true);
		                			res.insert(res.begin(), subfiles.begin(), subfiles.end());
								}
							}
						}
	                }
	                closedir(pDIR);
        		}
        		return res;
			}
			
			static int isFile(const char* name)
			{
			    DIR* directory = opendir(name);
			
			    if(directory != NULL)
			    {
				    closedir(directory);
				    return 0;
			    }
			
			    if(errno == ENOTDIR)
			    {
			    	return 1;
			    }
			
			    return -1;
			}
	};
}

#endif
