#ifndef ABSE_PORTER
#define ABSE_PORTER 1

#include <string>

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
		/* TODO (rasekh#1#): Add members to store a keymap for furthure 
		                     use */
		
		public:
			keymap(string path)
			{
				/* TODO (rasekh#1#): Construct the keymap from the file given.
				                     assume that keyboard chars in the same rows 
				                     are given in lines and each line contains chars 
				                     immediate after each other just like this:
				                     qwertyuiop[]
				                     asdfghjkl;'
				                     zxcvbnm,./ */
				
			}
			
			double distance(char c1, char c2)
			{
				/* TODO (rasekh#1#): Calculate the distance between two keyboard 
				                     chars according to the fed keymap.
									 the distance sould have a threshold, 
									 it means the distance between far keys should be
									 the same but the distance between near keys should
									 differ more */
				
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
				/* TODO (rasekh#1#): calculate the distance between the words 
				                     using the statistical algorithm on wich we 
				                     aggreed. */
				
			}
			
			double deep_distance(string word1, string word2, int mark)
			{
				/* TODO (rasekh#1#): Calculate the distance deeply using the user 
				                     marks and levinsteign and good_distance */
				
			}
	};
	
	/* TODO (asgari#1#): Designing the input assistant */
	class spellcheck : public trienode
	{
		private:
			/* TODO (rasekh#1#): Add some configurations:
			                       1- nearest words distance threshold
			                       2- learning rate
			                       ... */
			
			vector<string> nearests(string input)
			{
				// go deep in the trie and return the nearest neighbours using good_distance
			}
		public:
			vector<string> recommand(string input)
			{
				/* TODO (rasekh#1#): recommand the nearest neighbours for the 
				                     word given word sorted by the distance measure */
				
				// sort the words using LivenSteign
				// sort the reocmmanded words Using good_distance
			}
			vector<string> raterecom(string input, string recommanded, int mark)
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
#endif
