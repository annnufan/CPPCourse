#include <iostream>
#include <limits>

struct signed_based_tag{};

struct unsigned_based_tag{};

template <typename T>
struct number_traits {
    signed_based_tag tag;
};

template<>
struct number_traits<int> {
    typedef signed_based_tag tag;
};

template<>
struct number_traits<unsigned > {
    typedef unsigned_based_tag tag;
};

template <typename T>
struct check {
    T value;

    check<T>(T v = 0) : value(v) {}

    check<T>(const check<T> &other) : value(other.value) {}

    check<T> operator=(check<T> const &other) {
        value = other.value;
        return *this;
    }

    check<T> operator-();
};

template <typename T>
bool check_negate_overflow(check<T> el, signed_based_tag) {
    return el.value == std::numeric_limits<T>::min();
}

template <typename T>
bool check_negate_overflow(check<T> el, unsigned_based_tag) {
    return el.value != 0;
}

template <typename T>
check<T> check<T>::operator-() {
	if (!check_negate_overflow(*this, typename number_traits<T>::tag())) {
		value = -value;
		return *this;	
	} else {
		throw -1;	
	}	
}

template <typename T>
bool check_add_overflow(check<T> x, check<T> y, signed_based_tag) {
	T a = x.value, b = y.value;
	return ((a > 0) ? (std::numeric_limits<T>::max() - a < b) : (std::numeric_limits<T>::max() - a > b) );		
}

template <typename T>
bool check_add_overflow(check<T> x, check<T> y, unsigned_based_tag) {
	T a = x.value, b = y.value;
	return (std::numeric_limits<T>::max() - a < b);		
}

template <typename T>
check<T> operator +(check<T> x, check<T> y) {
	if (!check_add_overflow(x, y, typename number_traits<T>::tag())) {
		return check<T>(x.value + y.value);
	} else {
		throw -1;	
	}
}

template <typename T>
bool check_sub_overflow(check<T> x, check<T> y, signed_based_tag) {
	T a = x.value, b = y.value;
	return (a >= 0) ? (b < a - std::numeric_limits<T>::max()) : (b > a - std::numeric_limits<T>::min()); 		
}

template <typename T>
bool check_sub_overflow(check<T> x, check<T> y, unsigned_based_tag) {
	T a = x.value, b = y.value;
	return (b < a - std::numeric_limits<T>::max()); 		
}

template <typename T>
check<T> operator -(check<T> x, check<T> y) {
	if (!check_sub_overflow(x, y, typename number_traits<T>::tag())) {
		return check<T>(x.value - y.value);
	} else {
		throw -1;	
	}
}

template <typename T>
bool check_mul_overflow(check<T> x, check<T> y, signed_based_tag) {
	T a = x.value, b = y.value, c = a * b;
	return (a == std::numeric_limits<T>::min() && b == -1 || b == std::numeric_limits<T>::min() && a == -1 || (b != 0 && c / b != a));
}

template <typename T>
bool check_mul_overflow(check<T> x, check<T> y, unsigned_based_tag) {
	T a = x.value, b = y.value, c = a * b;
	return  (b != 0 && c / b != a);
}

template <typename T>
check<T> operator *(check<T> x, check<T> y) {
	if (!check_mul_overflow(x, y, typename number_traits<T>::tag())) {
		return check<T>(x.value * y.value);
	} else {
		throw -1;	
	}
}

template <typename T>
bool check_div_overflow(check<T> x, check<T> y, signed_based_tag) {
	T a = x.value, b = y.value;
	return (a == std::numeric_limits<T>::min() && b == -1) || (b == 0);
}

template <typename T>
bool check_div_overflow(check<T> x, check<T> y, unsigned_based_tag) {
	T a = x.value, b = y.value;
	return (b == 0);
}

template <typename T>
check<T> operator /(check<T> x, check<T> y) {
	if (!check_div_overflow(x, y, typename number_traits<T>::tag())) {
		return check<T>(x.value / y.value);
	} else {
		throw -1;	
	}
}

int main() {

    return 0;
}