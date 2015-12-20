#include<iostream>
#include<string>
using namespace std;

string replace(string input, string src, string dst)
{
	if(input == src)
		return dst;
	else
		return input;
}


