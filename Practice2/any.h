#ifndef ANY_H
#define ANY_H

#include <typeinfo>
#include <type_traits>
#include <stdexcept>
#include <iostream>

struct any {
public:
	any() : state(EMPTY), data(nullptr) {}

	template<>
	any(const any &other) : state(other.state), data(other.data) {
		if (!other.empty()) {
			other.data -> copy(other.val, val);
		}
	}

	template<>
	any(any && other) : state(other.state), data(other.data) {
		if (!other.empty()) {
			other.data -> move(other.val, val);
			other.data = nullptr;
		}
	}

	template <typename ValType>
	any(const ValType &other) {
		construct(other);
	}

	template <typename ValType>
	any(ValType &&other) {
		construct(std::forward<ValType>(other));
	}

	any & operator=(any const  & other) {
		any(other).swap(*this);
		return *this;
	}

	any & operator=(any && other) {
		any(std::move(other)).swap(*this);
		return *this;
	}

	template<typename ValType> 
	any & operator=(ValType const & other) {
		any(other).swap(*this);
		return *this;
	}

	template<typename ValType> 
	any & operator=(ValType && other) {
		any(std::forward<ValType>(other)).swap(*this);
        return *this;
	}

	void swap(any & other) {
		if (data == other.data) {
			if (state != EMPTY) {
				data->swap(val, other.val);
			}
			return;
		}
		any tmp(std::move(other));
		other.data = data;
		other.state = state;
		if (state != EMPTY) {
			data->move(val, other.val);
		}
		data = tmp.data;
		state = tmp.state;
		if (tmp.data != nullptr) {
			tmp.data->move(tmp.val, val);
			tmp.data = nullptr;
			tmp.state = EMPTY;
		}
	}

	bool empty() const {
		return state == EMPTY;
	}

	const std::type_info & type() const {
		return empty() ? typeid(void) : data -> type();
	}

	~any() {
		clear();
	}


	template<typename CastType>
	CastType* cast() {
		if (type() != typeid(CastType)) {
			throw std::runtime_error("Invalid cast");
		}
		switch (state) {
			case SMALL:
				return reinterpret_cast<CastType *>(&val.small_val);
			case BIG:
				return reinterpret_cast<CastType *>(val.big_val);
			default:
				return nullptr;

		}
	}

	template<typename CastType>
	const CastType* cast() const {
		if (type() != typeid(CastType)) {
			throw std::runtime_error("Invalid cast");
		}
		switch (state) {
			case SMALL:
				return reinterpret_cast<const CastType *>(&val.small_val);
			case BIG:
				return reinterpret_cast<const CastType *>(val.big_val);
			default:
				return nullptr;

		}
	}

private:

	static const size_t SIZE = 16;

	enum storage_state {
		EMPTY,
		SMALL,
		BIG
	} state;

	union storage {
		void* big_val;
		std::aligned_storage<SIZE,SIZE>::type small_val;
	} val;
	
	struct base_holder {
		const std::type_info &(*type)();

        void (*destroy)(storage &);

        void (*copy)(const storage &src, storage &dest);

        void (*move)(storage &src, storage &dest);

        void (*swap)(storage &lhs, storage &rhs);
	};

	template <typename ValType>
	struct small_holder {
		small_holder() {}
		static const std::type_info& type(){
			return typeid(ValType);
		}

		static void destroy(storage &val) {
			reinterpret_cast<ValType *>(&val.small_val)->~ValType();
		}

		static void copy(const storage &src, storage &other) {
			new(&other.small_val) ValType(reinterpret_cast<const ValType &>(src.small_val));
		}

		static void move(storage &src, storage &other) {
			new(&other.small_val) ValType(std::move(reinterpret_cast<ValType &>(src.small_val)));
			destroy(src);
		}

		static void swap(storage &lhs, storage &rhs) {
			std::swap(reinterpret_cast<ValType &>(lhs.small_val), reinterpret_cast<ValType &>(rhs.small_val));
		}
	};

	template <typename ValType>
	struct big_holder {
		big_holder() {}
		static const std::type_info& type(){
			return typeid(ValType);
		}

		static void destroy(storage &val) {
			delete reinterpret_cast<ValType *>(val.big_val);
		}

		static void copy(const storage &src, storage &other) {
			other.big_val = new ValType(*reinterpret_cast<const ValType *>(src.big_val));
		}

		static void move(storage &src, storage &other) {
			other.big_val = src.big_val;
			src.big_val = nullptr;
		}

		static void swap(storage &lhs, storage &rhs) {
			std::swap(lhs.big_val, rhs.big_val);
		}
	};

	base_holder* data;

	void clear() {
		if (state != EMPTY) {
			data->destroy(val);
			data = nullptr;
			state = EMPTY;
		}
	}

	template<typename ValType>
    struct is_small {
		const static bool value = (std::is_nothrow_copy_constructible<ValType>::value) && (sizeof(ValType) <= SIZE);
    };

	template<typename ValType>
	typename std::enable_if<is_small<ValType>::value>::type construct(ValType &&value) {
		using Type = typename std::decay<ValType>::type;
		static base_holder holder = {
			small_holder<Type>::type,
			small_holder<Type>::destroy,
			small_holder<Type>::copy,
			small_holder<Type>::move,
			small_holder<Type>::swap
		};
		data = &holder;
		new(&val.big_val) Type(std::forward<ValType>(value));
		state = SMALL;
	}

	template<typename ValType>
	typename std::enable_if<!is_small<ValType>::value>::type construct(ValType &&value) {
		using Type = typename std::decay<ValType>::type;
		static base_holder holder = {
			big_holder<Type>::type,
			big_holder<Type>::destroy,
			big_holder<Type>::copy,
			big_holder<Type>::move,
			big_holder<Type>::swap
		};
		data = &holder;
		val.big_val = new Type(std::forward<ValType>(value));
		state = BIG;
	} 
};

template <typename ValType>
const ValType* any_cast(const any *other) {
	if (other == nullptr) {
		throw std::runtime_error("Invalid cast");
	}
	return other->cast<ValType>();	
}

template <typename ValType>
ValType* any_cast(any *other) {
	if (other == nullptr) {
		throw std::runtime_error("Invalid cast");
	}
	return other->cast<ValType>();	
}

template<class ValType>
ValType any_cast(const any& other) {
	return *other.cast<ValType>();
}

template<class ValType>
ValType any_cast(any& other) {
	return *other.cast<ValType>();
}

template<class ValType>
ValType any_cast(any&& other) {
	return *other.cast<ValType>();
}

#endif // BIND_H