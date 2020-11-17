#pragma once

#include <unordered_set>

namespace Ubpa {
	template<typename T, size_t BLOCK_SIZE>
	Pool<T, BLOCK_SIZE>::Pool(Pool&& pool) noexcept
		: blocks{ std::move(pool.blocks) },
		freeAdresses{ std::move(pool.freeAdresses) }
	{
		pool.blocks.clear();
		pool.freeAdresses.clear();
	}

	template<typename T, size_t BLOCK_SIZE>
	Pool<T, BLOCK_SIZE>::~Pool() {
		Clear();
	}

	template<typename T, size_t BLOCK_SIZE>
	Pool<T, BLOCK_SIZE>& Pool<T, BLOCK_SIZE>::operator=(Pool&& pool) noexcept {
		Clear();
		blocks = std::move(pool.blocks);
		freeAdresses = std::move(pool.freeAdresses);
		pool.blocks.clear();
		pool.freeAdresses.clear();
		return *this;
	}

	template<typename T, size_t BLOCK_SIZE>
	template<typename... Args>
	T* Pool<T, BLOCK_SIZE>::Request(Args&&... args) {
		if (freeAdresses.empty())
			NewBlock();
		T* freeAdress = freeAdresses.back();
		new(freeAdress)T{ std::forward<Args>(args)... };
		freeAdresses.pop_back();
		return freeAdress;
	}

	template<typename T, size_t BLOCK_SIZE>
	void Pool<T, BLOCK_SIZE>::Recycle(T* object) {
		if constexpr (!std::is_trivially_destructible_v<T>)
			object->~T();
		freeAdresses.push_back(object);
	}

	template<typename T, size_t BLOCK_SIZE>
	void Pool<T, BLOCK_SIZE>::Reserve(size_t n) {
		size_t blockNum = n / BLOCK_SIZE + static_cast<size_t>(n % BLOCK_SIZE > 0);
		for (size_t i = blocks.size(); i < blockNum; i++)
			NewBlock();
	}

	template<typename T, size_t BLOCK_SIZE>
	void Pool<T, BLOCK_SIZE>::FastClear() {
		for (auto* block : blocks) {
#if defined(WIN32) || defined(_WINDOWS)
			_aligned_free(block);
#else
			free(block);
#endif // defined(WIN32) || defined(_WINDOWS)
		}
		blocks.clear();
		freeAdresses.clear();
	}

	template<typename T, size_t BLOCK_SIZE>
	void Pool<T, BLOCK_SIZE>::Clear() {
		if constexpr (std::is_trivially_destructible_v<T>)
			FastClear();
		else {
			std::unordered_set<T*> freeAdressesSet(freeAdresses.begin(), freeAdresses.end());
			for (auto* block : blocks) {
				for (size_t i = 0; i < BLOCK_SIZE; i++) {
					T* adress = block + i;
					if (freeAdressesSet.find(adress) == freeAdressesSet.end())
						adress->~T();
				}
#if defined(WIN32) || defined(_WINDOWS)
				_aligned_free(block);
#else
				free(block);
#endif // defined(WIN32) || defined(_WINDOWS)
			}
			blocks.clear();
			freeAdresses.clear();
		}
	}

	template<typename T, size_t BLOCK_SIZE>
	void Pool<T, BLOCK_SIZE>::NewBlock() {
#if defined(WIN32) || defined(_WINDOWS)
		auto block = reinterpret_cast<T*>(_aligned_malloc(BLOCK_SIZE * sizeof(T), std::alignment_of_v<T>));
#else
		auto block = reinterpret_cast<T*>(aligned_alloc(BLOCK_SIZE * sizeof(T), std::alignment_of_v<T>));
#endif // defined(WIN32) || defined(_WINDOWS)
		blocks.push_back(block);
		for (size_t i = 0; i < BLOCK_SIZE; i++)
			freeAdresses.push_back(block + i);
	}
}
