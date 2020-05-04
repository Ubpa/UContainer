#pragma once

#include <unordered_set>

namespace Ubpa {
	template<typename T>
	Pool<T>::Pool(Pool&& pool) noexcept
		: blocks{ std::move(pool.blocks) },
		freeAdresses{ std::move(pool.freeAdresses) }
	{
		pool.blocks.clear();
		pool.freeAdresses.clear();
	}

	template<typename T>
	Pool<T>::~Pool() {
		Clear();
	}

	template<typename T>
	template<typename... Args>
	T* Pool<T>::Request(Args&&... args) {
		if (freeAdresses.empty())
			NewBlock();
		T* freeAdress = freeAdresses.back();
		new(freeAdress)T{ std::forward<Args>(args)... };
		freeAdresses.pop_back();
		return freeAdress;
	}

	template<typename T>
	void Pool<T>::Recycle(T* object) {
		object->~T();
		freeAdresses.push_back(object);
	}

	template<typename T>
	void Pool<T>::Reserve(size_t n) {
		size_t blockNum = n / BLOCK_SIZE + static_cast<size_t>(n % BLOCK_SIZE > 0);
		for (size_t i = blocks.size(); i < blockNum; i++)
			NewBlock();
	}

	template<typename T>
	void Pool<T>::FastClear() {
		for (auto block : blocks) {
#ifdef WIN32
			_aligned_free(block);
#else
			free(block);
#endif // WIN32
		}
		blocks.clear();
		freeAdresses.clear();
	}

	template<typename T>
	void Pool<T>::Clear() {
		if constexpr (std::is_trivially_destructible_v<T>)
			FastClear();
		else {
			std::unordered_set<T*> freeAdressesSet(freeAdresses.begin(), freeAdresses.end());
			for (auto block : blocks) {
				for (size_t i = 0; i < BLOCK_SIZE; i++) {
					T* adress = block + i;
					if (freeAdressesSet.find(adress) == freeAdressesSet.end())
						adress->~T();
				}
#ifdef WIN32
				_aligned_free(block);
#else
				free(block);
#endif // WIN32
			}
			blocks.clear();
			freeAdresses.clear();
		}
	}

	template<typename T>
	void Pool<T>::NewBlock() {
#ifdef WIN32
		auto block = reinterpret_cast<T*>(_aligned_malloc(BLOCK_SIZE * sizeof(T), std::alignment_of_v<T>));
#else
		auto block = reinterpret_cast<T*>(aligned_alloc(BLOCK_SIZE * sizeof(T), std::alignment_of_v<T>));
#endif // WIN32
		blocks.push_back(block);
		for (size_t i = 0; i < BLOCK_SIZE; i++)
			freeAdresses.push_back(block + i);
	}
}
