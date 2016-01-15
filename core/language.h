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
					euclid_distance = 2;
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
					for(int j = 0; j < word2.length(); j++)
					{
						if(abs(i - j) < 2)
							sum += acci->distance(word1[i], word2[j]) / (abs(j - i) * 3 + 1);
					}
				}
				if(abs((int)word1.length() - (int)word2.length()) > 1)
					sum *= word1.length() * word2.length();
				return sum;
			}
			
			double deep_distance(string word1, string word2, int mark)
			{
				/* TODO (rasekh#1#): Calculate the distance deeply using the user 
				                     marks and levinsteign and good_distance */
				
			}
	};
	
	/* TODO (asgari#1#): Designing the input assistant */
	/* TODO (asgari#1#): Migrate this to trienode */
	
	class spellcheck
	{
		private:
			/* TODO (rasekh#1#): Add some configurations:
			                       1- nearest words distance threshold
			                       2- learning rate
			                       ... */
			
			
		public:
			vector<string> recommand(string input)
			{
				/* TODO (rasekh#1#): recommand the nearest neighbours for the 
				                     word given word sorted by the distance measure */
				
				// sort the words using LivenSteign
				// sort the reocmmanded words Using good_distance
			}
			void raterecom(string input, string recommanded, int mark)
			{
				
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
	        string value;
	        int mark;
	        void inner_add(trienode* child)
	        {
	        	if(children.count(child->keypart) == 0)
	        	{
	        		children.insert(make_pair(child->keypart, child));
	        		child->parent = this;
				}
				else
				{
					if(child->value != "")
					{
						children[child->keypart]->mark++;
						if(children[child->keypart]->value == "")
						{
							children[child->keypart]->value = child->value;
						}
					}
				}
	        }
	        
			vector<string> inner_nearests(string input)
			{
				// go deep in the trie and return the nearest neighbours using good_distance
				vector<string> res;
				if(input.length() < 2)
				{
					if(value != "")
					{
						res.push_back(value);
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
	    	vector<string> first_level_words()
	    	{
	    		vector<string> res;
				for(map<char, trienode*>::iterator i = children.begin(); i != children.end(); i++)
				{
					if(i->second->value != "")
					{
						res.push_back(i->second->value);
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
	    			res.insert(*i);
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
	    			int marginal_mark = find(*i)->getMark();
	    			if(res.count(*i) > 0)
	    			{
	    				res[*i] += marginal_mark;
	    			}
	    			else
	    			{
	    				res.insert(make_pair(*i, marginal_mark));
					}
				}
				vector<pair<int, string>> sortedres;
				for(map<string, int>::iterator i = res.begin(); i != res.end(); i++)
				{
					if(i->second > 1)
						sortedres.push_back(make_pair(i->second - 1, i->first));
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
	        	this->mark = mark;
	            this->value = value;
	            this->parent = parent;
	            this->keypart = keypart;
	        }
	        
	        trienode(string dicpath)
	        {
	        	this->mark = 0;
	            this->value = "";
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
	        
	        void add(string key, string value)
	        {
	        	if(key.length() == 1)
	        	{	
		        	inner_add(new trienode(this, key[0], value, 1));
		        	return;
		        }
		        else
		        {
		        	inner_add(new trienode(this, key[0], "", 0));
		        	children[key[0]]->add(key.substr(1), value);
				}
	        }
	        
	        bool haskey(string key)
	        {
	            if(key.length() == 0)
	            {
	                return mark > 0;
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
	                if(mark > 0)
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
				return mark;
			}
			string getValue()
			{
				return value;
			}
	};
}
#endif
