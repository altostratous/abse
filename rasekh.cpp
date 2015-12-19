#include<iostream>
#include<string>
#ifdef compileit
string replace(string input, string src, string dst)
{
	if(input == src)
		return dst;
	else
		return input;
}
#endif


