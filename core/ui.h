/*
	Purpose: User interface
*/
#ifndef ABSE_UI
#define ABSE_UI 1

#include<iostream>
#include<vector>
#include <windows.h>
#include <map>
#include <set>
#include <sys/time.h>

#include "util.h"
#include "search.h"

using namespace disk;
using namespace index;
using namespace std;
using namespace search;
using namespace ds;

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
			unsigned long int starttime;
			config conf;
			watable wat;
			bool pleaseexit;
			bool ranconcommand;
			//trienode* dic;
			spellcheck* sp;
			
			unsigned long int clock()
			{
				struct timeval tp;
				gettimeofday(&tp, NULL);
				return tp.tv_sec * 1000 + tp.tv_usec / 1000;
			}
			
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
				cout<<blue<<"config "<<yellow<<"[/s]"<<endl;
				cout<<blue<<"search "<<yellow<<"[search-conditions]"<<endl;
				cout<<blue<<"view "<<yellow<<"[file-id]"<<endl;
				cout<<blue<<"mark "<<yellow<<"[word]"<<endl;
				cout<<blue<<"index"<<endl;
				cout<<blue<<"save "<<endl;
				cout<<blue<<"exit"<<endl;
				cout<<"help"<<endl;
				cout<<"about"<<white<<endl;
				
			}
			
			report(wanalysis& wa, file& f)
			{
				ofstream fout;
				if(conf.isset("LogFile"))
					fout.open(conf.getString("LogFile").c_str());
				if(conf.getInteger("OccurranceView"))
				{	
					cout<<blue<<"--------------------------------- Occurrances ---------------------------------"<<white<<endl;
					if(conf.getInteger("ShowCount"))
						cout<<"Found "<<yellow<<wa.getCount()<<white<<" items.\n";
					if(conf.getInteger("ReportFile") || conf.getInteger("ShowSummary"))
						for(int i = 0; i < wa.getCount(); i++)
						{
							occurrance o1 = wa.getOccurrance(i);
							occurrance o2 = o1;
							o1.index -= 3*o1.length;
							o1.length *= 3;
							o2.index += o2.length;
							o2.length *= 3;
							if(conf.getInteger("ReportFile"))
								cout<<yellow<<"File"<<blue<<"#"<<yellow<<o1.file_id<<blue<<": "<<white<<f.getFileNameById(o1.file_id)<<"\n"<<yellow<<"Paragraph"<<blue<<"#"<<yellow<<o1.paragraph_id<<white<<endl;
							if(conf.getInteger("ShowSummary"))
								cout<<"..."<<f.look(o1)<<yellow<<f.look(wa.getOccurrance(i))<<white<<f.look(o2)<<"..."<<endl;
							if(conf.isset("LogFile"))
							{
								fout<<"File: "<<f.getFileNameById(o1.file_id)<<"\n"<<"Paragraph"<<"#"<<o1.paragraph_id<<endl;
								fout<<"..."<<f.look(o1)<<f.look(wa.getOccurrance(i))<<f.look(o2)<<"..."<<endl;
							}
						}
				}
				if(conf.getInteger("FileView"))
				{
					cout<<blue<<"------------------------------------ Files ------------------------------------"<<white<<endl;
					map<int, int> files;
					for(int i = 0; i < wa.getCount(); i++)
					{
						occurrance o = wa.getOccurrance(i);
						if(files.count(o.file_id))
						{
							files[o.file_id]++;
						}
						else
						{
							files.insert(make_pair(o.file_id, 1));
						}
					}
					cout<<"Found "<<yellow<<distance(files.begin(), files.end())<<white<<" files."<<endl;
					for(map<int, int>::iterator i = files.begin(); i != files.end(); i++)
					{
						cout<<yellow<<"File"<<blue<<"#"<<yellow<<(*i).first<<blue<<" "<<white<<f.getFileNameById((*i).first)<<" Found "<<yellow<<(*i).second<<white<<" items."<<endl;
					}
				}
				if(conf.isset("LogFile"))
					fout.close();
			}
			
			mark()
			{
				string word;
				cin>> word;
				vector<pair<int, string>> nearests = sp->getDic()->marked_nearests(word);
				for(vector<pair<int, string>>::iterator i = nearests.begin(); i != nearests.end(); i++)
				{
					cout<<i->first<<"\t"<<i->second<<endl;
				}
				sp->recommand(word);
			}
			
			find()
			{
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
					if(arguments == "/f")
						f.iterate(wa);
				report(wa, f);
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
				if(key != "/s")
				{
					cin>>value;
					conf.add(key, value);
					cout<<green<<"Configuration set successfully."<<white<<endl;
				}
				else
				{
					cout<<blue<<"Configurations are set as below:"<<green<<endl;
					cout<<conf.view();
					cout<<white;
				}
			}
			
			save()
			{
				conf.save();
				sp->save(conf.getString("DicPath"));
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
				//fasterate(f, "", wat, conf.getInteger("ThreadsCount"));
				f.iterate(wat);
				cout<<green<<"Indexed the files successfully!"<<white<<endl;
			}
			
			search()
			{
				string cond_str;
				getline(cin, cond_str);
				file f(dir::getFiles(conf.getString("FilesDirectory").c_str(), true)); 
				condition cond(cond_str, conf.getInteger("StemInput"), sp);
				if(conf.getInteger("DoOptimization"))
				{
					if(conf.getInteger("EchoOptimization"))
						cout<<red<<"Understood: " + cond.toString() <<endl<<white;
					cond.optimize(wat);
					if(conf.getInteger("EchoOptimization"))
						cout<<green<<"Optimized: " + cond.toString() <<endl<<white;
				}
				wanalysis wa = *cond.filter(wat);
				report(wa, f);
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
				starttime = clock();
				pleaseexit = false;
				ranconcommand = false;
				about();
				help();
				//dic = new trienode("words.txt");
				sp = new spellcheck(conf.getString("DicPath"), conf.getInteger("correction"), conf.getInteger("learning"), new distancing("keymap.config"));
			}
			
			
			void start()
			{
				streambuf* orig = cin.rdbuf();
				int ignoretimeshow = false;
				while(!pleaseexit)
				{
					starttime = clock() - starttime;
					if(!ignoretimeshow && conf.getInteger("ShowTime")) 
					{
						cout<<green<<"Done in "<<starttime<<" clocks."<<endl<<white;
					}
					else
					{
						if(ignoretimeshow)
						{
							ignoretimeshow = false;
						}
					}
					// get command and process
					string command = "";
					if(ranconcommand)
					{
						cin>>command;
						if(command == "")
						{
    						std::cin.rdbuf(orig);
    						ignoretimeshow = true;
							continue;
						}
					}
					else
					{
						if(conf.isset("Command"))
						{
							istringstream oss(conf.getString("Command"));
							cin.rdbuf(oss.rdbuf());
							cin>>command;
							ranconcommand = true;
						}
						else
						{
							ranconcommand = true;
							ignoretimeshow = true;
							continue;
						}
					}
					
					starttime = clock();
					
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
					
					if(command == "mark")
					{
						mark();
						continue;
					}
					
					cout<<red<<"No such command: "<<command<<endl<<white;
					getline(cin, command);
				}
			}
	};
}

#endif
