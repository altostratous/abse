#include <iostream>

#include "../../core/util.h"

using namespace ui;
using namespace search;

int main(int argc, char** argv) {
	/*
	config conf("abse.config");
	file f(dir::getFiles(conf.getString("FilesDirectory").c_str(), true));
	f.setWordSeperators(conf.getString("WordSeperators"));
	wanalysis wa("word");
	f.iterate(wa);
	cout<<wa.getCount();
	for(int i = 0; i < wa.getCount(); i++)
	{
		occurrance o = wa.getOccurrance(i);
		o.index -= 3*o.length;
		o.length *= 7;
		cout<<"File#"<<o.paragraph_id<<"#"<<o.file_id<<endl;
		cout<<"..."<<f.look(o)<<"..."<<endl;
	}
	*/
	
	//cmdui cmd;
	
	//cmd.start();
	/*distancing dising("keymap.config");
	while(1)
	{
		string w1, w2;
		cin>>w1;
		cin>>w2;
		cout<<dising.good_distance(w1, w2)<<endl;
	}*/
	
	trienode* root = new trienode(NULL, '\000', "", 0);
	for(int i = 0; i < 10; i++)
	{
		string input;
		cin>> input;
		root->add(input, input);
	}
	for(int i = 0; i < 5; i++)
	{
		string key;
		cin>> key;
		cout<< root->find(key)->getValue() << endl << root->find(key)->getMark() << endl;
	}
	return 0;
}
