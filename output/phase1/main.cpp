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
	wanalysis wa("love");
	f.iterate(wa);
	cout<<wa.getCount();
	for(int i = 0; i < wa.getCount(); i++)
	{
		occurrance o = wa.getOccurrance(i);
		o.index -= o.length;
		o.length *= 3;
		cout<<f.look(o)<<endl;
	}
	return 0;
}
