#include <iostream>
#include <vector>
#include <list>
#include <set>

struct node_based_tag{};

struct vector_based_tag{};

template <typename C>
struct container_traits;

template<typename T>
struct container_traits {
    typedef node_based_tag tag;
};

template<typename E>
struct container_traits<std::vector<E>> {
    typedef vector_based_tag tag;
};

template<>
struct container_traits<std::list<>> {
    typedef node_based_tag tag;
};

template<>
struct container_traits<std::set<>> {
    typedef node_based_tag tag;
};

/*
template<typename C, typename F>
void erase_if(C& c, F pred) {
    erase_if_impl(c, pred, container_traits<C>::tag());
};

template<typename C, typename F>
void erase_if_impl(C& c, F pred, node_based_tag) {
    typedef typename C::iterator iter;
    iter curr = c.begin();
    while (curr != c.end()) {
        if (pred(*curr)) {
            curr = xc.erase(curr);
        } else {
            curr++;
        }
    }
}

template<typename C, typename F>
void erase_if_impl(C& c, F pred, vector_based_tag) {
    typedef typename C::iterator iter;
    c.resize(std::remove_if(c.begin(), c.end(), pred) - c.begin());
}
 */

 template <typename U, typename V>
 struct is_same{
     static bool const value = false;
 };

template<typename T>
struct is_same<T, T> {
    static bool const value = true;
};

template<bool coundition, typename T>
struct enable_if;

template<typename T>
struct enable_if<true, T> {
    typedef T type;
};

template<typename C, typename F>
typename enable_if<is_same<typename container_traits<C>::tag, node_based_tag>::value, void>::type erase_if(C& c, F pred) {


};

int main() {
    std::vector<int> v;
    return 0;
}