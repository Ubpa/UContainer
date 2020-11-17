#pragma once

#include <vector>
#include <array>
#include <cstdlib>

namespace Ubpa {
	template<typename T, size_t BLOCK_SIZE = 1024>
	class Pool {
	public:
		Pool() noexcept = default;
		Pool(Pool&& pool) noexcept;
		Pool& operator=(Pool&& pool) noexcept;
		~Pool();

		template<typename... Args>
		T* Request(Args&&... args);
		void Recycle(T* object);
		void Reserve(size_t n);
		// no ~T()
		void FastClear();
		void Clear();

	private:
		void NewBlock();

	private:
		std::vector<T*> blocks;
		std::vector<T*> freeAdresses;
	};
}

#include "details/Pool.inl"
