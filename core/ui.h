/*
	Purpose: User interface
*/
#ifndef ABSE_UI
#define ABSE_UI 1

#include<iostream>
#include<vector>

#include "util.h"

using namespace disk;
using namespace index;
using namespace std;

namespace ui
{
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
			bool pleaseexit;
			string currentfile;
			// commands
			exit()
			{
				pleaseexit = true;
			}
		public:
			cmdui()
			{
				pleaseexit = false;
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
						cin>>currentfile;
						continue;
					}
					
					if(command == "count")
					{
						string word;
						cin>>word;
						vector<string> files;
						files.insert(files.begin(), currentfile);
						file f(files);
						wanalysis wa(word);
						f.iterate(wa);
						cout<<"Found "<<wa.getCount()<<" instances.";
					}
					
					if(command == "replace")
					{
						string word;
						string toreplace;
						string output;
						cin>>word;
						cin>>toreplace;
						cin>>output;
						vector<string> files;
						files.insert(files.begin(), currentfile);
						file f(files);
						replacer rep(word, toreplace);
						f.iterate(rep, output);
					}
					
					if(command == "find")
					{
						string word;
						cin>>word;
						
						file f(dir::getFiles(conf.getString("FilesDirectory").c_str(), true));
						f.setWordSeperators(conf.getString("WordSeperators"));
						wanalysis wa(word);
						f.iterate(wa);
						cout<<"Found "<<wa.getCount()<<" times.\n";
						for(int i = 0; i < wa.getCount(); i++)
						{
							occurrance o = wa.getOccurrance(i);
							o.index -= 3*o.length;
							o.length *= 7;
							cout<<"File: "<<f.getFileNameById(o.file_id)<<"\n"<<"Paragraph#"<<o.paragraph_id<<endl;
							cout<<"..."<<f.look(o)<<"..."<<endl;
						}
					}
				}
			}
	};
}

#endif
