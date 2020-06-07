#include <UContainer/VarSizeAllocMngr.h>

#include <iostream>

using namespace std;
using namespace Ubpa;

int main() {
	VarSizeAllocMngr allocMngr(1024);

	auto allocation0 = allocMngr.Allocate(102, 32);
	auto allocation1 = allocMngr.Allocate(1000, 4);

	cout
		<< allocation0.IsValid() << endl
		<< allocation0.size << endl
		<< allocation0.unalignedOffset << endl;

	cout
		<< allocation1.IsValid() << endl
		<< allocation1.size << endl
		<< allocation1.unalignedOffset << endl;
	
	return 0;
}
