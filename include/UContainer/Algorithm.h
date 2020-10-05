#pragma once

#include <algorithm>
#include <iterator>
#include <set>

namespace Ubpa {
	template<typename Container>
	Container SetUnion(const Container& x, const Container& y);
	template<typename Container>
	Container SetDifference(const Container& x, const Container& y);
	template<typename Container>
	Container SetIntersection(const Container& x, const Container& y);
	template<typename Container>
	Container SetSymmetricDifference(const Container& x, const Container& y);
}

#include "details/Algorithm.inl"
