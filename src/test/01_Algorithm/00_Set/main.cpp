#include <UContainer/Algorithm.h>

#include <iostream>
#include <vector>

using namespace std;
using namespace Ubpa;

int main() {
	vector<int> s0 = { 1,2,3,4,5,6 };
	vector<int> s1 = { 4,5,6,7,8,9 };
	auto r0 = SetUnion(s0, s1);
	auto r1 = SetDifference(s0, s1);
	auto r2 = SetSymmetricDifference(s0, s1);
	auto r3 = SetIntersection(s0, s1);

	for (auto i : r0)
		cout << i;
	cout << endl;
	for (auto i : r1)
		cout << i;
	cout << endl;
	for (auto i : r2)
		cout << i;
	cout << endl;
	for (auto i : r3)
		cout << i;
	cout << endl;
	
	return 0;
}
