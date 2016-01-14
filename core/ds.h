/*
	Purpose: to impliment datastructures here
	Note: Everything here should be storable and retrivable
*/

#ifndef ABSE_DS
#define ABSE_DS 1

#include <functional>
#include <map>
#include <set>

using namespace std;

namespace ds
{
	/*
		This is the data structure to store occurrances of grams
		Things to store
			1- Index of the word
			2- Id of the file
			3- Id of the word
	*/
	struct occurrance
	{
		int file_id;
		int word_id;
		int index;
		int length;
		int paragraph_id;
		int line_id;
	};
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
					if(child->mark > 0)
						children[child->keypart]->mark++;
				}
	        }
	        
			vector<string> inner_nearests(string input)
			{
				// go deep in the trie and return the nearest neighbours using good_distance
				if(value == "cat")
					cout<<value<<endl;
				vector<string> res;
				if(input.length() == 0)
				{
					res.push_back(value);
					return res;
				}
				for(map<char, trienode*>::iterator i = children.begin(); i != children.end(); i++)
				{
					if(i->second->keypart == 'c')
					{
						int kdfj = 0;
					}
					if(i->second->keypart == 'a')
					{
						int kdfj = 0;
					}
					if(i->second->keypart == 't')
					{
						int kdfj = 0;
					}
					if(i->second->keypart == input[0])
					{
						vector<string> subres = i->second->inner_nearests(input.substr(1));
						res.insert(res.begin(), subres.begin(), subres.end());
					}
					if(i->second->containsChild(input[0]))
					{
						vector<string> subres = i->second->children[input[0]]->inner_nearests(input.substr(1));
						res.insert(res.begin(), subres.begin(), subres.end());
					}
					if(input.length() > 1)
					{
						if(i->second->keypart == input[1])
						{
							vector<string> subres = i->second->inner_nearests(input.substr(2));
							res.insert(res.begin(), subres.begin(), subres.end());
						}
						if(i->second->containsChild(input[1]))
						{
							vector<string> subres = i->second->children[input[1]]->inner_nearests(input.substr(2));
							res.insert(res.begin(), subres.begin(), subres.end());
						}
					}
				}
				return res;
			}
	    public:
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
			
			map<string, int> marked_nearests(string input)
			{
	    		map<string, int> res;
	    		vector<string> inner = inner_nearests(input);
	    		for(vector<string>::iterator i = inner.begin(); i != inner.end(); i++)
	    		{
	    			if(res.count(*i) > 0)
	    			{
	    				res[*i]++;
	    			}
	    			else
	    			{
	    				res.insert(make_pair(*i, 1));
					}
				}
				return res;
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
	        void add(string key, string value)
	        {
	        	if(key.length() == 1)
	        	{	
		        	inner_add(new trienode(this, key[0], value, 1));
		        	return;
		        }
		        else
		        {
		        	inner_add(new trienode(this, key[0], value, 0));
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
