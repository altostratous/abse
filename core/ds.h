/*
	Purpose: to impliment datastructures here
	Note: Everything here should be storable and retrivable
*/

#ifndef ABSE_DS
#define ABSE_DS 1

#include <functional>
#include <map>

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
	    public:
//			bool containsChild(char key)
//			{
//				return children.count(key) > 0;
//			}
			
			vector<string> nearests(string input)
			{
				// go deep in the trie and return the nearest neighbours using good_distance
				vector<string> res;
				if(input.length() == 0)
				{
					res.push_back(value);
					return res;
				}
				for(map<char, trienode*>::iterator i = children.begin(); i != children.end(); i++)
				{
					if(i->first == input[0])
					{
						vector<string> subres = i->second->nearests(input.substr(1));
						res.insert(res.end(), subres.begin(), subres.end());
					}
					if(input.length() > 1)
						if(i->first == input[1])
						{
							vector<string> subres = i->second->nearests(input.substr(1));
							res.insert(res.end(), subres.begin(), subres.end());
						}
					if(input.length() > 2)
						if(i->first == input[2])
						{
							vector<string> subres = i->second->nearests(input.substr(1));
							res.insert(res.end(), subres.begin(), subres.end());
						}
				}
				return res;
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
