#ifndef ABSE_PORTER
#define ABSE_PORTER 1

#include <string>
#include <map>
#include <iostream>
#include <cmath>

#include "index.h"
#include "porter2_stemmer.h"
#include "ds.h"

#define TRUE 1
#define FALSE 0

/* The main part of the stemming algorithm starts here. b is a buffer
   holding a word to be stemmed. The letters are in b[k0], b[k0+1] ...
   ending at b[k]. In fact k0 = 0 in this demo program. k is readjusted
   downwards as the stemming progresses. Zero termination is not in fact
   used in the algorithm.
 
   Note that only lower case sequences are stemmed. Forcing to lower case
   should be done before stem(...) is called.
*/
using namespace std;
using namespace ds;

namespace index
{
	class porter
	{
		public:
			static string stem(string word)
			{
				Porter2Stemmer::stem(word);
				return word;
			}
	};
	
	class keymap
	{
		/* DONE (asgari#1#): Add members to store a keymap for furthure 
		                     use */
		map<char, pair<int, int>> char_table;
		public:
			keymap(string path)
			{
				/* DONE (asgari#1#): Construct the keymap from the file given.
				                     assume that keyboard chars in the same rows 
				                     are given in lines and each line contains chars 
				                     immediate after each other just like this:
				                     qwertyuiop[]
				                     asdfghjkl;'
				                     zxcvbnm,./ */
				ifstream fin(path);
				string tablestr = "";
				while(!fin.eof())
				{
					char c;
					fin.get(c);
					tablestr += c;
				}
				construct_char_table(tablestr);
				fin.close();
			}
			construct_char_table(string tablestr)
			{
				int row = 0, col = 0;
				for(int i = 0; i < tablestr.length(); i++)
				{
					if(tablestr[i] == '\n')
					{
						row++;
						col = 0;
						continue;
					}
					char_table.insert(make_pair(tablestr[i], make_pair(col, row)));
					col++;
				}
			}
			double distance(char c1, char c2)
			{
				/* DONE (asgari#1#): Calculate the distance between two keyboard 
				                     chars according to the fed keymap.
									 the distance sould have a threshold, 
									 it means the distance between far keys should be
									 the same but the distance between near keys should
									 differ more */
				pair<int, int> c1coords = char_table[c1];
				pair<int, int> c2coords = char_table[c2];
				double euclid_distance = pow(pow((double)(c1coords.first - c2coords.first), 2.0) + pow((double)(c1coords.second - c2coords.second), 2.0), 0.5);
				if(euclid_distance > 1)
					euclid_distance = 3;
				euclid_distance += 0.5;
				return euclid_distance;
			}
			vector<char> nearests(char input, double maxdis)
			{
				vector<char> res;
				for(map<char, pair<int, int>>::iterator i = char_table.begin(); i != char_table.end(); i++)
				{
					if(distance(i->first, input) <= maxdis)
						res.push_back(i->first);
				}
				return res;
			}
	};
	
	class distancing
	{
		private:
			keymap* acci;
			/* TODO (rasekh#1#): Add some configurations:
			                       weight of each distance in deep_disntace 
			                     calculation
			                       etc. */
			
		public:
			distancing(string path)
			{
				acci = new keymap(path);
			}
			int livenSteign(string word1, string word2)
			{
				/* TODO (rasekh#1#): Calculate and return the Liven Steign distance 
				                     between the words given. */
				
				return 0;
			}
			
			double good_distance(string word1, string word2)
			{
				/* DONE (asgari#1#): calculate the distance between the words 
				                     using the statistical algorithm on wich we 
				                     aggreed. */
				double sum = 0;
				for(int i = 0; i < word1.length(); i++)
				{
					double p1 = i / (double)word1.length();
					for(int j = 0; j < word2.length(); j++)
					{
						double p2 = i / (double)word2.length();
						sum += acci->distance(word1[i], word2[j]) / (abs(j - i) + 0.5);
					}
				}
				
				int ld = abs(word1.length() - word2.length());
				if(ld > 1)
					sum *= ld;
				
				return sum;
			}
			
			double deep_distance(string word1, string word2)
			{
				/* TODO (rasekh#1#): Calculate the distance deeply using the user 
				                     marks and levinsteign and good_distance */
				//word1 = porter::stem(word1);
				//word2 = porter::stem(word2);
//				double sd1 = good_distance(word1, word1);
//				double sd2 = good_distance(word2, word2);
				double dis = good_distance(word1, word2);
//				dis /= sd1;
//				dis /= sd2;
				return dis;
			}
	};
	

}

using namespace index;

namespace ds
{
	/* DONE (asgari#1#): Add datastructure for dictionary and spell 
	                     check and suggest */
	                     
	
	class trienode
	{
	    private:
	        trienode* parent;
	        map<char, trienode*> children;
	        char keypart;
	        // string value;
	        map<string, int> values;
	        // int mark;
	        void inner_add(trienode* child)
	        {
	        	if(children.count(child->keypart) == 0)
	        	{
	        		children.insert(make_pair(child->keypart, child));
	        		child->parent = this;
				}
				else
				{
					if(child->getValue() != "")
					{
						if(children[child->keypart]->values.count(child->getValue()) > 0)
							children[child->keypart]->values[child->getValue()]+=child->getMark();
						else
							children[child->keypart]->values[child->getValue()]=child->getMark();
						// children[child->keypart]->value = child->value;
					}
				}
	        }
	        string getKey()
	        {
	        	if(parent == NULL)
	        		return "";
	        	return parent->getKey() + keypart;
			}
			vector<string> inner_nearests(string input)
			{
				// go deep in the trie and return the nearest neighbours using good_distance
				vector<string> res;
				if(input.length() < 2)
				{
					if(values.size() > 0)
					{
						res.push_back(getKey());
					}
					vector<string>subres = first_level_words();
					res.insert(res.end(), subres.begin(), subres.end());
					if(input.length() == 0)
						return res;
				}
				for(map<char, trienode*>::iterator i = children.begin(); i != children.end(); i++)
				{
					if(i->second->keypart == input[0])
					{
						vector<string> subres = i->second->inner_nearests(input.substr(1));
						res.insert(res.end(), subres.begin(), subres.end());
					}
					if(i->second->containsChild(input[0]))
					{
						vector<string> subres = i->second->children[input[0]]->inner_nearests(input.substr(1));
						res.insert(res.end(), subres.begin(), subres.end());
					}
					if(input.length() > 1)
					{
						if(i->second->keypart == input[1])
						{
							vector<string> subres = i->second->inner_nearests(input.substr(2));
							res.insert(res.end(), subres.begin(), subres.end());
						}
						if(i->second->containsChild(input[1]))
						{
							vector<string> subres = i->second->children[input[1]]->inner_nearests(input.substr(2));
							res.insert(res.end(), subres.begin(), subres.end());
						}
					}
					if(input.length() > 2)
					{
						if(i->second->keypart == input[2])
						{
							vector<string> subres = i->second->inner_nearests(input.substr(3));
							res.insert(res.end(), subres.begin(), subres.end());
						}
						if(i->second->containsChild(input[2]))
						{
							vector<string> subres = i->second->children[input[2]]->inner_nearests(input.substr(3));
							res.insert(res.end(), subres.begin(), subres.end());
						}
					}
				}
				return res;
			}
	    public:
			static bool comprecom(pair<int, string> e1, pair<int, string> e2)
			{
				return (e1.first < e2.first);
			}
			// returns keys to first level words
	    	vector<string> first_level_words()
	    	{
	    		vector<string> res;
				for(map<char, trienode*>::iterator i = children.begin(); i != children.end(); i++)
				{
					if(i->second->values.size() > 0)
					{
						res.push_back(i->second->getKey());
					}
				}
				return res;
			}
	    	set<string> nearests(string input)
	    	{
	    		set<string> res;
	    		vector<string> inner = inner_nearests(input);
	    		for(vector<string>::iterator i = inner.begin(); i != inner.end(); i++)
	    		{
	    			res.insert(find(*i)->getValue());
				}
				return res;
			}
			
			vector<pair<int, string>> marked_nearests(string input)
			{
	    		map<string, int> res;
	    		distancing dis("keymap.config");
	    		vector<string> inner = inner_nearests(input);
	    		for(vector<string>::iterator i = inner.begin(); i != inner.end(); i++)
	    		{
	    			for(map<string, int>::iterator val = find(*i)->values.begin(); val != find(*i)->values.end(); val++)
	    			{
						
		    			int marginal_mark = find(*i)->values[val->first];
		    			string current_value = val->first;
		    			if(res.count(current_value) > 0)
		    			{
		    				res[current_value] += marginal_mark;
		    			}
		    			else
		    			{
		    				res.insert(make_pair(current_value, marginal_mark));
						}
					}
				}
				vector<pair<int, string>> sortedres;
				for(map<string, int>::iterator i = res.begin(); i != res.end(); i++)
				{
					if(i->first != "")
						sortedres.push_back(make_pair(i->second, i->first));
				}
				sort(sortedres.begin(), sortedres.end(), comprecom);
				return sortedres;
			}
			bool containsChild(char key)
			{
				return children.count(key) > 0;
			}
			
	        trienode(trienode* parent, char keypart, string value, int mark)
	        {
	            this->values[value] = mark;
	            this->parent = parent;
	            this->keypart = keypart;
	        }
	        
	        trienode(string dicpath)
	        {
	            this->parent = NULL;
	            this->keypart = '\000';
	            ifstream fin(dicpath);
	            string word;
	            while(!fin.eof())
	            {
	            	getline(fin, word);
	            	add(word, word);
				}
	        }
	        // funtion for adding the word to the dictianory and rating corrections
	        void add(string key, string value, int rate = 1)
	        {
	        	if(key.length() == 1)
	        	{	
		        	inner_add(new trienode(this, key[0], value, rate));
		        	return;
		        }
		        else
		        {
		        	inner_add(new trienode(this, key[0], "", 0));
		        	children[key[0]]->add(key.substr(1), value, rate);
				}
			}
	        
	        bool haskey(string key)
	        {
	            if(key.length() == 0)
	            {
	                return values.size() > 0;
	            }
	            if(children.count(key[0]) > 0)
	                return children[key[0]]->haskey(key.substr(1));
	            else
	                return false;
	        }
	        
	        trienode* find(string key)
	        {
	        	if(key.length() == 0)
	            {
	                if(values.size() > 0)
	                	return this;
	                else
	                	return NULL;
	            }
	            if(children.count(key[0]) > 0)
	                return children[key[0]]->find(key.substr(1));
	            else
	                return NULL;
			}
			
			int getMark()
			{
				int max_mark = 0;
				if(values.size() > 0)
					max_mark = values.begin()->second;
				for(map<string, int>::iterator i = values.begin(); i != values.end(); i++)
				{
					if(i->second > max_mark)
						max_mark = i->second;
				}
				return max_mark;
			}
			string getValue()
			{
				int max_mark = getMark();
				for(map<string, int>::iterator i = values.begin(); i != values.end(); i++)
				{
					if(i->second == max_mark)
						return i->first;
				}
				return "";
			}
	};
}

namespace index 
{
		/* TODO (asgari#1#): Designing the input assistant */
	/* TODO (asgari#1#): Migrate this to trienode */
	
	class spellcheck
	{
		private:
			/* TODO (rasekh#1#): Add some configurations:
			                       1- nearest words distance threshold
			                       2- learning rate
			                       ... */
			bool dorecommand;
			bool dolearn;
			trienode * dic;
			distancing* dis;
			
		public:
			string recommand(string input)
			{
				if(!dorecommand)
					return input;
				vector<pair<int, string>> res = dic->marked_nearests(input);
				if(res.size() == 0)
				{
					if(dolearn)
						dic->add(input, input);
					return input;
				}
				else
				{
					int max_mark = 0;
					for(vector<pair<int, string>>::iterator i = res.begin(); i != res.end(); i++)
					{
						if(i->first > max_mark)
							max_mark = i->first;
					}
					for(int i = res.size() - 1; i >= 0; i--)
					{
						if(abs(res[i].first - max_mark) > (max_mark / 2))
							res.erase(res.begin() + i);
					}
					int min_dis_ind = 0; // the index of the word with the least distance
					double min_dis = dis->deep_distance(input, res[0].second);
					int counter = 0;
					for(vector<pair<int, string>>::iterator i = res.begin(); i != res.end(); i++)
					{
						double distance = dis->deep_distance(input, i->second);
						//cout<<distance<<" "<<i->second<<endl;
						if(distance < min_dis)
						{
							min_dis = distance;
							min_dis_ind = counter;
						}
						counter++;
					}
					if(dolearn)
					{
						string recom =res[min_dis_ind].second;
						if(recom == input)
							return recom;
						cout<<"We recommend "<<recom<<" instead of "<<input<<"."<<endl;
						cout<<"What is your idea? 1. Like! 2. No, the Original one! 3. Don't mind it! 4. You're wrong definitly!";
						int idea;
						cin>>idea;
						switch(idea)
						{
							case 1:
								dic->add(input, recom, 1);
								return recom;
								break;
							case 2:
							    dic->add(input, input, 1);
								return input;
								break;
							case 3:
								return "";
								break;
							case 4:
								dic->add(input, recom, -1);
								return "";
								break;
							default:
								cout<<"Your idea is not important!"<<endl;
								return input;
								break;
						}
					}
					else
					{
						return res[min_dis_ind].second;
					}
				}
			}
			spellcheck(trienode * dic, bool dorecommand, bool dolearn, distancing* dis)
			{
				this->dic = dic;
				this->dolearn = dolearn;
				this->dorecommand = dorecommand;
				this->dis = dis;
				ignore = false;
			}
			void save(string path)
			{
				/* TODO (rasekh#1#): the spelling and recommandation system 
				                     should be storable and restorable.
				                     do the storing  here. */
				
			}
			void load(string path)
			{
				/* TODO (rasekh#1#): the spelling and recommandation system 
				                     should be storable and restorable. Do restore 
				                     the system in here */
				
			}
	};
}
#endif
