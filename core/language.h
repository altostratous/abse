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
