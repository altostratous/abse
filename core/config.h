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
				properties.insert(make_pair(key, val));
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
						cout<<l<<endl;
						add(l);
					}
					fin.close();
				}
			}
			
			string getString(string key)
			{
				return properties[key];
			}
	};
}
#endif
