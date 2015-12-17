/*
	Purpose: to do preprocessing on file
*/

#ifndef ABSE_PREPROCESS
#define ABSE_PREPROCESS 1

#include<string>
#include "file.h"

using namespace disk;

namespace preprocess
{
	/*
		This class will provide extracting text parts of a file. For example ignoring 
		non-meaningful parts of a file.
		This will have followings:
			1- process function
			2- map between the extracted and base file
	*/
	class textractor// : processor
	{
		public:
			void process(char input)
			{
				/* TODO (rasekh#1#): Gets a paragraph and if it is free of meaningfull 
				                     words it will be removed. */
				
			}
	};
}

#endif
