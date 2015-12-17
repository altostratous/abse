/*
	Purpose: to access files efficiently and perform processes
	Multithreadings should be done here
	Here we will imlement functions to iterate files and perform any process
*/

#ifndef ABSE_FILE
#define ABSE_FILE 1

#include<string>

namespace disk
{
	/*
		This class should have these members:
			1- a function to iterate files word by word like this 
				iterate(pointer to process function(string, fileid, index), output)
			2- list of files to be processed
			3- a funciton to iterate files paragraph by paragraph
			4- characters to be word seperators
			5- characters to be paragraph seperators
			optionals:
			6- function to process files by grams as given gram scale
	*/
	class file
	{
		/* TODO (rasekh#1#): Implement properties. */
		/* TODO (asgari#1#): Implement iterating functions. */
		
	};
	
//	class processor
//	{
//		public:
//			virtual string process(string input) = 0; 
//	};
}

#endif
