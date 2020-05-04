#pragma once

#include <map>
#include <string>

namespace Ubpa::detail::RsrcMngr_ {
	template<typename Value>
	struct DefaultKey {
		using type = std::string;
	};
}

namespace Ubpa {
	template<typename Value, typename Key = typename detail::RsrcMngr_::DefaultKey<Value>::type>
	class RsrcMngr {
	public:
		static RsrcMngr& Instance() noexcept;

		~RsrcMngr();

		Value* Get(const Key& key) const;

		bool Regist(const Key& key, Value* val);

		// if sizeof...(Args) == 0, use Value(const Key&), fallback to default constructor
		// use default_first = true to use default constructor
		template<typename... Args, bool default_first = false>
		Value* GetOrCreate(const Key& key, Args&&... args);

		void Release(const Key& key);

		void Clear();

	private:
		std::map<Key, Value*> key2val;

	private:
		RsrcMngr() = default;
	};
}

#include "detail/RsrcMngr.inl"
