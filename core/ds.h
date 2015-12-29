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
	// Rasekh: here I added a vector for the function line 264 of index
	vector<string> str_path; // vector for saving string path
	
	
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
	};
	/* TODO (rasekh#1#): Add datastructure for dictionary and spell 
	                     check and suggest */
	
}

#endif
