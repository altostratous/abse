#include <iostream>

#include "../../core/util.h"

using namespace ui;

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
	cout<<"    *** In the name of Allah ***"<<endl;
	cout<<"Welcome to ABSE."<<endl;
	cout<<"Authors:"<<endl;
	cout<<"\tAli Rasekh, CE Sharif University of Technology."<<endl;
	cout<<"\tAli Asgari, CE Sharif University of Technology."<<endl<<endl;
	cout<<"Instructions:"<<endl;
	cout<<"current [filename]"<<endl;
	cout<<"count [word]"<<endl;
	cout<<"replace [word-to-find] [word-to-replace] [output-filename]"<<endl;
	cout<<"find [word]"<<endl;
	cout<<"exit"<<endl;
	
	cmdui cmd;
	
	cmd.start();
	return 0;
}
