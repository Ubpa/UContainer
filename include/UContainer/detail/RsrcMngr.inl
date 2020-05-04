#pragma once

namespace Ubpa {
	template<typename Value, typename Key>
	RsrcMngr<Value, Key>& RsrcMngr<Value, Key>::Instance() noexcept {
		static RsrcMngr instance;
		return instance;
	}

	template<typename Value, typename Key>
	RsrcMngr<Value, Key>::~RsrcMngr() {
		for (const auto& [key, val] : key2val)
			delete val;
	}

	template<typename Value, typename Key>
	Value* RsrcMngr<Value, Key>::Get(const Key& key) const {
		auto target = key2val.find(key);
		if (target == key2val.end())
			return nullptr;
		return target->second;
	}

	template<typename Value, typename Key>
	bool RsrcMngr<Value, Key>::Regist(const Key& key, Value* val) {
		if (Get(key))
			return false;
		key2val[key] = val;
		return true;
	}

	template<typename Value, typename Key>
	template<typename... Args, bool default_first>
	Value* RsrcMngr<Value, Key>::GetOrCreate(const Key& key, Args&&... args) {
		Value* rst = Get(key);
		if (!rst) {
			if constexpr (sizeof...(Args) == 0) {
				if constexpr (!default_first) {
					if constexpr (std::is_constructible_v<Value, Key>)
						rst = new Value{ key };
					else if constexpr (std::is_constructible_v<Value>)
						rst = new Value;
					else
						static_assert(true, "not correct Args...");
				}
				else {
					if constexpr (std::is_constructible_v<Value>)
						rst = new Value;
					else if constexpr (std::is_constructible_v<Value, Key>)
						rst = new Value{ key };
					else
						static_assert(true, "not correct Args...");
				}
			}
			else
				rst = new Value{ std::forward<Args>(args)... };
			key2val[key] = rst;
		}
		return rst;
	}

	template<typename Value, typename Key>
	void RsrcMngr<Value, Key>::Release(const Key& key) {
		auto target = key2val.find(key);
		if (target == key2val.end())
			return;

		delete target->second;
		key2val.erase(target);
	}

	template<typename Value, typename Key>
	void RsrcMngr<Value, Key>::Clear() {
		for (const auto& [key, val] : key2val)
			delete val;
	}
}
