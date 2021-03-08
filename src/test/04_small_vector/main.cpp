#include <UContainer/small_vector.h>

#include <iostream>

using namespace std;
using namespace Ubpa;
struct D {
	inline static int ctor_cnt = 0;
	inline static int copy_ctor_cnt = 0;
	inline static int move_ctor_cnt = 0;
	inline static int dtor_cnt = 0;
	int flag;
	D() : flag{ 1 } { std::cout << ctor_cnt++ << ": [c]tor D" << std::endl; }
	D(const D&) : flag{ 1 } { std::cout << copy_ctor_cnt++ << ": [copy] ctor D" << std::endl; }
	D(D&&) noexcept : flag{ 1 } { std::cout << move_ctor_cnt++ << ": [move] ctor D" << std::endl; }
	~D() { assert(flag == 1); std::cout << dtor_cnt++ << ": [d]tor D" << std::endl; flag = 0; }
};
int main() {
	small_vector<D, 5> datas;
	datas.resize(4);
	datas.emplace_back();
	datas.emplace_back();
	datas.pop_back();
	datas.pop_back();
	
	small_vector<int, 5> ints{ 1,2,3,4 };
	for (int i : ints) {
		std::cout << i << std::endl;
	}
	for(auto iter = ints.rbegin(); iter != ints.rend(); ++iter)
		std::cout << *iter << std::endl;
	return 0;
}
