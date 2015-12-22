#ifndef ABSE_PORTER
#define ABSE_PORTER 1

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>

#include "index.h"
#include "porter2_stemmer.h"

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
	}
}
#endif
