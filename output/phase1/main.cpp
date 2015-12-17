#include <iostream>

#include "../../core/util.h"

using namespace index;
using namespace std;
using namespace disk;

int main(int argc, char** argv) {
	file f(dir::getFiles("E:\\absefiles"));
	wanalysis wa("computer");
	f.iterate(wa);
	cout<<wa.getCount();
	return 0;
}
