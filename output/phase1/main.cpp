#include <iostream>

#include "../../core/util.h"

using namespace index;
using namespace std;
using namespace disk;
using namespace ds;

int main(int argc, char** argv) {
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
	return 0;
}
