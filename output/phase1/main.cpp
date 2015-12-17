#include <iostream>

#include "../../core/util.h"

using namespace index;
using namespace std;

int main(int argc, char** argv) {
	file f("51908");
	wcounter wc("66");
	f.iterate(wc);
	cout<<"number of someone: "<< wc.getCount();
	return 0;
}
