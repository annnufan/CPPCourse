#ifndef BIND_H
#define BIND_H

#include <tuple>

template <size_t N>
struct place_holder{};

place_holder<0> _1;
place_holder<1> _2;
place_holder<2> _3;

template <size_t... SIZE>
struct range_t {};

template <size_t... SIZE, size_t... Curr>
struct range_builder{
	typedef typename range_builder<SIZE - 1, SIZE - 1, Curr...>::range range;
}

template <size_t... Curr>
struct range_builder<0, Curr...> {
	typedef range_t<Curr...> range;
};


template<typename F, typename... Args>
struct bind_t {

	bind_t(F &&func, Args &&...args) : func(std::forward<F>(func)), arguments(std::forward<Args>(args)...) {}

	template <typename... Func_args>
	auto operator()(Func_args &&...args) const {
		return call(get_range(), std::forward<Func_args>(args) ...);
	}

	auto get_range() {
		return typename range_builder<std::tuple_size<args_t>::value>::range();
	}


	template <size_t... N, typename... Call_args>
	auto call(range_t<N...> &&, Call_args... _args) const{
		return func(get(std::get<N>(args), _args ...) ...);
	}

	template<typename T, typename... Get_args>
	auto &&get(T &&curr, Get_args &... args) const {
		return curr;
	}

	template<size_t N, typename... Get_args>
	auto &&get(place_holder<N>, Get_args &... args) const {
		return std::get<N>(std::forward_as_tuple(args...));
	}

	template<typename Func, typename ... Func_args, typename ... Get_args>
	auto get(const bind_t<Func, Func_args ...> &f, Get_args &... _args) const {
		return f(_args ...);
	}


	typedef  std::tuple<Args...> args_t;
	F func;
	args_t args;
};



template<typename F, typename ... Args>
bind_t<F, Args...> bind(F &&f, Args &&... args) {
    return bind_t<F, Args...>(std::forward<F>(f), std::forward<Args>(args)...);
};

#endif // BIND_H