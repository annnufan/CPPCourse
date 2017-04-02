#ifndef BIND_H
#define BIND_H

#include <tuple>

template <size_t N>
struct place_holder{};

place_holder<0> _1;
place_holder<1> _2;
place_holder<2> _3;


template<typename F, typename... Args>
struct bind_t {

	bind_t(F && tmp_func, Args &&... tmp_args) : func(std::forward<F>(tmp_func)), args(std::forward<Args>(tmp_args)...) {}

	template <typename... Func_args>
	auto operator()(Func_args &&... tmp_args) {
		return call(typename range_builder<std::tuple_size<args_t>::value>::range(),
					std::forward<Func_args>(tmp_args) ...);
	}

private:
	template <size_t... N>
	struct range_t {};

	template <size_t N, size_t... Curr>
	struct range_builder{
		typedef typename range_builder<N - 1, N - 1, Curr...>::range range;
	};

	template <size_t... Curr>
	struct range_builder<0, Curr...> {
		typedef range_t<Curr...> range;
	};

	template <size_t... N, typename... Call_args>
	auto call(const range_t<N ...>&, Call_args &&... tmp_args) {
		return func(get(std::get<N>(args), tmp_args ...) ...);
	}

	template<typename Base_args, typename... Get_args>
	decltype(auto) get(Base_args &base_args, Get_args &...) {
		return base_args;
	}

	template<size_t N, typename... Get_args>
	decltype(auto) get(place_holder<N>&, Get_args &... tmp_args) {
		return std::get<N>(std::forward_as_tuple(tmp_args...));
	}

	template<typename Func, typename ... Func_args, typename ... Get_args>
	auto get(bind_t<Func, Func_args ...> &f, Get_args &... tmp_args) {
		return f(tmp_args ...);
	}

	typedef  std::tuple<std::decay_t<Args>...> args_t;
	typedef typename std::decay<F>::type F_t;
	F_t func;
	args_t args;
};

template<typename F, typename ... Args>
bind_t<F, Args...> bind(F &&f, Args &&... args) {
    return bind_t<F, Args...>(std::forward<F>(f), std::forward<Args>(args)...);
}

#endif // BIND_H