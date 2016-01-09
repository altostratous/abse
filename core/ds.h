/*
	Purpose: to impliment datastructures here
	Note: Everything here should be storable and retrivable
*/

#ifndef ABSE_DS
#define ABSE_DS 1

#include <functional>

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
	        trienode* children[50];
	        char keypart;
	        int count;
	        bool is_end;
	        void addchild(trienode* child)
	        {
	            children[count] = child;
	            child->parent = this;
	            count++;    
	        }
	    public:
	        trienode(trienode* parent, char keypart)
	        {
	            is_end = false;
	            this->parent = parent;
	            count = 0;
	            this->keypart = keypart;
	        }
	        void addtrienode(string key)
	        {
	            if(key.length() == 0)
	            {
	                is_end = true;
	                return;
	            }
	            int i;
	            bool test = false;
	            for(i = 0; i < count; i++)
	            {
	                if(children[i]->keypart == key[0])
	                {
	                    test = true;
	                    break;
	                }
	            }
	            if(test)
	            {
	                children[i]->addtrienode(key.substr(1));
	            }
	            else
	            {
	                trienode* ch = new trienode(this, key[0]);
	                addchild(ch);
	                addtrienode(key);
	            }
	        }
	        
	        bool haskey(string key)
	        {
	            if(key.length() == 0)
	            {
	                return is_end;
	            }
	            bool test = false;
	            int i;
	            for(i = 0; i < count; i++)
	            {
	                if(children[i]->keypart == key[0])
	                {
	                    test = true;
	                    break;
	                }
	            }
	            if(test)
	                return children[i]->haskey(key.substr(1));
	            else
	                return false;
	        }
	};
}

#endif
