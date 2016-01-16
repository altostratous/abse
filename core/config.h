/*
	Purpose: to provide configuration
*/

#ifndef ABSE_CONFIG
#define ABSE_CONFIG 1

#include<fstream>
#include<map>

#include "file.h"

using namespace std;

namespace disk
{
	/*
		This class will be initialized with the config file
		This will provide a dictionary to read config vars
		This will provide casting to a type
		This will provide writing properties to a file
		This will provide reading properties from a file
	*/
	class config
	{
		private:
			string path;
			map<string, string>properties;
			
			void add(string input)
			{
				string key = "";
				int i;
				for(i = 0; i < input.length(); i++)
				{
					if(input[i] != ':')
						key += input[i];
					else
						break;
				}
				i++;
				string val = "";
				for(; i < input.length(); i++)
				{
					val += input[i];
				}
				add(key, val);
			}
		public:
			config()
			{
				path = "abse.config";
				reload();
			}
			
			config(string path)
			{
				this->path = path;
				reload();
			}
			
			void reload()
			{
				if(dir::isFile(path.c_str()))
				{
					string l;
					ifstream fin(path.c_str());
					while(!fin.eof())
					{
						getline(fin, l);
						if(l[0] == '\\' && l[1] == '\\')
							continue;
						add(l);
					}
					fin.close();
				}
			}
			
			bool isset(string key)
			{
				return properties.count(key);
			}
			
			string getString(string key)
			{
				return properties[key];
			}
			
			void add(string key, string value)
			{
				if(isset(key))
				{
					properties[key] = value;
				}
				if(key.length() > 0)
					properties.insert(make_pair(key, value));
			}
			
			void save()
			{
				ofstream fout(path.c_str());
				map<string, string>::iterator i;
				for(i = properties.begin(); i != properties.end(); i++)
				{
					fout<<(*i).first<<":"<<(*i).second<<endl;
				}
				fout.close();
			}
			
			int getInteger(string key)
			{
				char* str = (char*) malloc(getString(key).length());
				strcpy(str, getString(key).c_str());
				int res =  atoi(str);
				free (str);
				return res;
			}
			
			string view()
			{
				map<string, string>::iterator i;
				string res = "";
				for(i = properties.begin(); i != properties.end(); i++)
				{
					res = res + "\t" + (*i).first + ":\t\t" + (*i).second + "\n";
				}
				return res;
			}
	};
}
#endif
