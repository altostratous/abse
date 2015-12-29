/*
	Purpose: User interface
*/
#ifndef ABSE_UI
#define ABSE_UI 1

#include<iostream>
#include<vector>
#include <windows.h>

#include "util.h"
#include "search.h"

using namespace disk;
using namespace index;
using namespace std;
using namespace search;

namespace ui
{
	
	// Adding colors to consule. Copied from here: 
	// http://www.codeproject.com/Articles/16431/Add-color-to-your-std-cout
	inline std::ostream& blue(std::ostream &s)
	{
	    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
	    SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE
	              |FOREGROUND_GREEN|FOREGROUND_INTENSITY);
	    return s;
	}
	
	inline std::ostream& red(std::ostream &s)
	{
	    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
	    SetConsoleTextAttribute(hStdout, 
	                FOREGROUND_RED|FOREGROUND_INTENSITY);
	    return s;
	}
	
	inline std::ostream& green(std::ostream &s)
	{
	    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
	    SetConsoleTextAttribute(hStdout, 
	              FOREGROUND_GREEN|FOREGROUND_INTENSITY);
	    return s;
	}
	
	inline std::ostream& yellow(std::ostream &s)
	{
	    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
	    SetConsoleTextAttribute(hStdout, 
	         FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
	    return s;
	}
	
	inline std::ostream& white(std::ostream &s)
	{
	    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
	    SetConsoleTextAttribute(hStdout, 
	       FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
	    return s;
	}
	
	struct color {
	    color(WORD attribute):m_color(attribute){};
	    WORD m_color;
	};
	
	template <class _Elem, class _Traits>
	std::basic_ostream<_Elem,_Traits>& 
	      operator<<(std::basic_ostream<_Elem,_Traits>& i, color& c)
	{
	    HANDLE hStdout=GetStdHandle(STD_OUTPUT_HANDLE); 
	    SetConsoleTextAttribute(hStdout,c.m_color);
	    return i;
	}
	
	
	/*
		ghabeliate tanzim option haye mokhtalef
		ghabeliate anjame karhaye mokhtalef az jomleh index va search
		command ha:
			config key value
			config save
			index word
			replace src dst output
			count word
			current file
			find word
			exit
	*/
	class cmdui
	{
		private:
			config conf;
			watable wat;
			bool pleaseexit;
			// commands
			exit()
			{
				pleaseexit = true;
			}
			
			about()
			{
				cout<<blue<<"                          *** In the name of Allah ***"<<endl;
				cout<<"Welcome to ABSE."<<endl;
				cout<<yellow<<"Authors:"<<white<<endl;
				cout<<"\tAli Rasekh, CE Sharif University of Technology."<<endl;
				cout<<"\tAli Asgari, CE Sharif University of Technology."<<endl<<endl;
			}
			
			help()
			{
				cout<<yellow<<"Instructions:"<<blue<<endl;
				cout<<"current "<<yellow<<"[filename]"<<endl;
				cout<<blue<<"count "<<yellow<<"[word]"<<endl;
				cout<<blue<<"replace "<<yellow<<"[word-to-find] [word-to-replace] [output-filename]"<<endl;
				cout<<blue<<"find "<<yellow<<"[word]"<<" /f"<<endl;
				cout<<blue<<"log "<<yellow<<"[filename]"<<endl;
				cout<<blue<<"normalize "<<yellow<<"[output-dir]"<<endl;
				cout<<blue<<"config "<<yellow<<"[key] [value]"<<endl;
				cout<<blue<<"search "<<yellow<<"[search-conditions]"<<endl;
				cout<<blue<<"view "<<yellow<<"[file-id]"<<endl;
				cout<<blue<<"index"<<endl;
				cout<<blue<<"save "<<endl;
				cout<<blue<<"exit"<<endl;
				cout<<"help"<<endl;
				cout<<"about"<<white<<endl;
				
			}
			
			find()
			{
				ofstream fout;
				if(conf.isset("LogFile"))
					fout.open(conf.getString("LogFile").c_str());
				
				string word;
				cin>>word;
				
				file f(dir::getFiles(conf.getString("FilesDirectory").c_str(), true));
				f.setWordSeperators(conf.getString("WordSeperators"));
				wanalysis wa(word);
				string arguments;
				/* TODO (asgari#1#): catch user faults */
				
				getline(cin, arguments);
				while(arguments[0] == ' ' && arguments.length() > 0)
				{
					arguments = arguments.substr(1);
				}
				if(arguments == "")
				{
					// search in the index
					wa = *(wat.find(word));
				}
				else
					if(arguments == " /f")
						f.iterate(wa);
				cout<<"Found "<<yellow<<wa.getCount()<<white<<" times.\n";
				for(int i = 0; i < wa.getCount(); i++)
				{
					occurrance o1 = wa.getOccurrance(i);
					occurrance o2 = o1;
					o1.index -= 3*o1.length;
					o1.length *= 3;
					o2.index += o2.length;
					o2.length *= 3;
					cout<<yellow<<"Filef"<<blue<<"#"<<yellow<<o1.file_id<<blue<<": "<<white<<f.getFileNameById(o1.file_id)<<"\n"<<yellow<<"Paragraph"<<blue<<"#"<<yellow<<o1.paragraph_id<<white<<endl;
					cout<<"..."<<f.look(o1)<<yellow<<word<<white<<f.look(o2)<<"..."<<endl;
					if(conf.isset("LogFile"))
					{
						fout<<"File: "<<f.getFileNameById(o1.file_id)<<"\n"<<"Paragraph"<<"#"<<o1.paragraph_id<<endl;
						fout<<"..."<<f.look(o1)<<word<<f.look(o2)<<"..."<<endl;
					}
				}
				if(conf.isset("LogFile"))
					fout.close();
			}
			
			replace()
			{
				string word;
				string toreplace;
				string output;
				cin>>word;
				cin>>toreplace;
				cin>>output;
				vector<string> files;
				files.insert(files.begin(), conf.getString("CurrentFile"));
				file f(files);
				f.setWordSeperators(conf.getString("WordSeperators"));
				replacer rep(word, toreplace);
				f.iterate(rep, output);
			}
			
			count(string output = "")
			{
				/* TODO (asgari#1#): add functionallity to count on the ds
 */
				
				
				ofstream fout;
				if(output!="")
				{
					fout.open(output.c_str());
				}
				string word;
				cin>>word;
				vector<string> files;
				files.insert(files.begin(), conf.getString("CurrentFile"));
				file f(files);
				f.setWordSeperators(conf.getString("WordSeperators"));
				wanalysis wa(word);
				f.iterate(wa);
				cout<<"Found "<<yellow<<wa.getCount()<<white<<" instances."<<endl;
			}
			
			current()
			{
				string filename;
				cin>>filename;
				conf.add("CurrentFile", filename);
				cout<<green<<"Current file set successfully."<<white<<endl;
			}
			
			configure()
			{
				string key;
				string value;
				cin>>key;
				cin>>value;
				conf.add(key, value);
				cout<<green<<"Configuration set successfully."<<white<<endl;
			}
			
			save()
			{
				conf.save();
				wat.save("index");
				cout<<green<<"Configurations saved successfully."<<white<<endl;
			}
			
			log()
			{
				string filename;
				cin>>filename;
				conf.add("LogFile", filename);
				cout<<green<<"Configurations saved successfully."<<white<<endl;
			}
			
			normalize(void)
			{
				normalizer n(conf.getString("StopWords"));
				
				string odir;
				cin>> odir;
				
				file f(dir::getFiles(conf.getString("FilesDirectory").c_str(), true));
				f.setWordSeperators(conf.getString("WordSeperators"));
				fasterate(f, odir, n, conf.getInteger("ThreadsCount"));
				cout<<green<<"Normalized successfully!"<<white<<endl;
			}
			
			index()
			{
				file f(dir::getFiles(conf.getString("FilesDirectory").c_str(), true));
				f.setWordSeperators(conf.getString("WordSeperators"));
				/* TODO (asgari#1#): Implement multithreaded version */
				
				//fasterate(f, "", wat, conf.getInteger("ThreadsCount"));
				f.iterate(wat);
				cout<<green<<"Indexed the files successfully!"<<white<<endl;
			}
			
			search()
			{
				string cond_str;
				getline(cin, cond_str);
				file f(dir::getFiles(conf.getString("FilesDirectory").c_str(), true));
				condition cond(cond_str);
				wanalysis wa = *cond.filter(wat);
				cout<<"Found "<<yellow<<wa.getCount()<<white<<" times.\n";
				for(int i = 0; i < wa.getCount(); i++)
				{
					occurrance o1 = wa.getOccurrance(i);
					occurrance o2 = o1;
					o1.index -= 3*o1.length;
					o1.length *= 3;
					o2.index += o2.length;
					o2.length *= 3;
					cout<<yellow<<"Filef"<<blue<<"#"<<yellow<<o1.file_id<<blue<<": "<<white<<f.getFileNameById(o1.file_id)<<"\n"<<yellow<<"Paragraph"<<blue<<"#"<<yellow<<o1.paragraph_id<<white<<endl;
					cout<<"..."<<f.look(o1)<<yellow<<f.look(wa.getOccurrance(i))<<white<<f.look(o2)<<"..."<<endl;
				}
			}
			
			view()
			{
				int fileid;
				cin>>fileid;
				file f(dir::getFiles(conf.getString("FilesDirectory").c_str(), true));
				ifstream fin(f.getFileNameById(fileid));
				cout<<green;
				while(!fin.eof())
				{
					string line;
					getline(fin, line);
					cout<<line<<endl;
				}
				cout<<white;
				fin.close();
			}
		public:
			cmdui()
			{
				pleaseexit = false;
				about();
				help();
				wat = *(new watable("index"));
			}
			
			
			void start()
			{
				while(!pleaseexit)
				{
					// get command and process
					string command = "";
					cin>>command;
					
					if(command == "exit")
					{
						exit();
						continue;	
					}
					
					if(command == "current")
					{
						current();
						continue;
					}
					
					if(command == "count")
					{
						count();
						continue;
					}
					
					if(command == "replace")
					{
						replace();
						continue;
					}
					
					if(command == "find")
					{
						find();
						continue;
					}
					
					if(command == "help")
					{
						help();
						continue;
					}
					
					if(command == "about")
					{
						about();
						continue;
					}
					
					if(command == "save")
					{
						save();
						continue;
					}
					
					if(command == "config")
					{
						configure();
						continue;
					}
					
					if(command == "log")
					{
						log();
						continue;
					}
					
					if(command == "normalize")
					{
						normalize();
						continue;
					}
					
					if(command == "index")
					{
						index();
						continue;
					}
					
					if(command == "search")
					{
						search();
						continue;
					}
					
					if(command == "view")
					{
						view();
						continue;
					}
					cout<<red<<"No such command: "<<command<<endl<<white;
					getline(cin, command);
				}
			}
	};
}

#endif
