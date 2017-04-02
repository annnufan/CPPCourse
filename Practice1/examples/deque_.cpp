#include <iostream>
#include "deque.h"
#include <cassert>

deque::iterator::iterator() {}

deque::iterator::iterator(std::string* _ptr, std::string* _mostl, std::string* _mostr) : ptr(_ptr), mostl(_mostl), mostr(_mostr) {}

deque::iterator::iterator(deque::iterator const& other): ptr(other.ptr), mostl(other.mostl), mostr(other.mostr) {}

std::string& deque::iterator::operator*() {
    return *ptr;
}

deque::iterator& deque::iterator::operator=(iterator const& other) {
    iterator tmp(other);
    std::swap(tmp.ptr, ptr);
    std::swap(tmp.mostl, mostl);
    std::swap(tmp.mostr, mostr);
    return *this;
}

deque::iterator& deque::iterator::operator++() {
    ptr++;
    if (ptr == mostr) {
        ptr = mostl;
    }
    return *this;
}

deque::iterator& deque::iterator::operator--() {
    if (ptr == mostl) {
        ptr = mostr;
    }
    ptr--;
    return *this;
}

deque::iterator deque::iterator::operator++(int) {
    iterator ret = *this;
    ptr++;
    if (ptr == mostr) {
        ptr = mostl;
    }
    return ret;
}

deque::iterator deque::iterator::operator--(int) {
    iterator ret = *this;
    if (ptr == mostl) {
        ptr = mostr;
    }
    ptr--;
    return ret;
}

deque::iterator& deque::iterator::operator+=(int shift) {
    int pos = ptr - mostl;
    pos = (pos + shift) % (mostr - mostl);
    ptr = mostl + pos;
    return *this;
}                

deque::iterator& deque::iterator::operator-=(int shift) {
    int pos = ptr - mostl;
    pos = (pos - shift + (mostr - mostl)) % (mostr - mostl);
    ptr = mostl + pos;
    return *this;
}

deque::const_iterator::const_iterator() {}

deque::const_iterator::const_iterator(std::string* _ptr, std::string* _mostl, std::string* _mostr) : ptr(_ptr), mostl(_mostl), mostr(_mostr) {}

deque::const_iterator::const_iterator(deque::const_iterator const& other): ptr(other.ptr), mostl(other.mostl), mostr(other.mostr) {}

std::string const& deque::const_iterator::operator*() const {
    return *ptr;
}

deque::const_iterator& deque::const_iterator::operator=(const_iterator const& other) {
    const_iterator tmp(other);
    std::swap(tmp.ptr, ptr);
    std::swap(tmp.mostl, mostl);
    std::swap(tmp.mostr, mostr);
    return *this;
}

deque::const_iterator& deque::const_iterator::operator++() {
    ptr++;
    if (ptr == mostr) {
        ptr = mostl;
    }
    return *this;
}

deque::const_iterator& deque::const_iterator::operator--() {
    if (ptr == mostl) {
        ptr = mostr;
    }
    ptr--;
    return *this;
}

deque::const_iterator deque::const_iterator::operator++(int) {
    const_iterator ret = *this;
    ptr++;
    if (ptr == mostr) {
        ptr = mostl;
    }
    return ret;
}

deque::const_iterator deque::const_iterator::operator--(int) {
    const_iterator ret = *this;
    if (ptr == mostl) {
        ptr = mostr;
    }
    ptr--;
    return ret;
}

deque::const_iterator& deque::const_iterator::operator+=(int shift) {
    int pos = ptr - mostl;
    pos = (pos + shift) % (mostr - mostl);
    ptr = mostl + pos;
    return *this;
}

deque::const_iterator& deque::const_iterator::operator-=(int shift) {
    int pos = ptr - mostl;
    pos = (pos - shift + (mostr - mostl)) % (mostr - mostl);
    ptr = mostl + pos;
    return *this;
}
            
void deque::resize_x2() {
    deque tmp(*this, capacity * 2);
    swap(*this, tmp);
    /*
    std::swap(tmp.storage, storage);
    std::swap(tmp.capacity, capacity);
    std::swap(tmp.sz, sz);
    std::swap(tmp.from, from);
    */
}

deque::deque() : capacity(2), sz(0), from(0), storage((std::string*) operator new(capacity * sizeof(std::string))) {}

deque::deque(deque const& other) {
    void* p = operator new(other.capacity * sizeof(std::string));

    storage = (std::string*) p;
    capacity = other.capacity;
    sz = other.sz;
    from = 0;
    size_t i = 0;
    try {
        for (auto it = other.begin(); it != other.end(); ++it) {
            new(storage + i) std::string(*it);
            ++i;
        }
    } catch (...) {
        for (size_t j = 0; j < i; ++j) {
            (storage + j) -> ~basic_string();
        }
        operator delete(p);
    }
}

deque::deque(deque const& other, size_t new_capacity) {
    void* p = operator new(new_capacity * sizeof(std::string));

    storage = (std::string*) p;
    assert(new_capacity > other.capacity);
    capacity = new_capacity;
    sz = other.sz;
    from = 0;
    size_t i = 0;
    try {
        for (auto it = other.begin(); it != other.end(); ++it) {
            new(storage + i) std::string(*it);
            ++i;
        }
    } catch (...) {
        for (size_t j = 0; j < i; ++j) {
            (storage + j) -> ~basic_string();
        }
        operator delete(p);
    }
}

deque::~deque() {
    for (size_t i = 0; i < sz; ++i) {
        (storage + (from + i) % capacity) -> ~basic_string();
    }
    void* p = (void*) storage;
    operator delete(p);
}

deque& deque::operator=(deque const& other) {
    deque tmp(other);
    swap(*this, tmp);
    /*
    std::swap(tmp.storage, storage);
    std::swap(tmp.capacity, capacity);
    std::swap(tmp.sz, sz);
    std::swap(tmp.from, from);
    */
    return *this;
}

bool deque::empty() const {
    return sz == 0;
}

void deque::clear() {
    for (auto it = begin(); it != end(); ++it) {
        pop_front();
    }
}

std::string& deque::back() {
    assert(sz);
    return storage[(from + sz - 1) % capacity];
}

std::string const& deque::back() const {
    assert(sz);
    return storage[(from + sz - 1) % capacity];
}

void deque::pop_back() {
    assert(sz);
    --sz;
    (storage + (from + sz) % capacity) -> ~basic_string();
}

void deque::push_back(std::string const& el) {
    if (sz + 1 >= capacity) {
        resize_x2();
    }
    new(storage + (from + sz) % capacity) std::string(el);
    ++sz;
}

std::string& deque::front() {
    assert(sz);
    return storage[from];
}

std::string const& deque::front() const {
    assert(sz);
    return storage[from];
}

void deque::pop_front() {
    assert(sz);
    (storage + from) -> ~basic_string();
    from = (from + 1) % capacity;
    --sz;
}

void deque::push_front(std::string const& el) {
    if (sz + 1 >= capacity) {
        resize_x2();
    }
    from = (from + capacity - 1) % capacity;
    new(storage + from) std::string(el);
    ++sz;
}

size_t deque::size() const {
    return sz;
}

std::string& deque::operator[](size_t ind) {
    assert(ind < sz);
    return storage[(from + ind) % capacity];
}

std::string const& deque::operator[](size_t ind) const {
    assert(ind < sz);
    return storage[(from + ind) % capacity];
}

deque::iterator deque::begin() {
    return iterator(storage + from, storage, storage + capacity);
}

deque::iterator deque::end() {
    return iterator(storage + (from + sz) % capacity, storage, storage + capacity);
}

deque::const_iterator deque::begin() const {
    return const_iterator(storage + from, storage, storage + capacity);
}

deque::const_iterator deque::end() const {
    return const_iterator(storage + (from + sz) % capacity, storage, storage + capacity);
}

deque::iterator deque::erase(deque::iterator it) {
    iterator ret;
    if (it - begin() < (int) sz / 2) {
        ret = it + 1;
        while (it != begin()) {
            auto next = it--;
            std::swap(*it, *next);
            it = next;
        }
        pop_front();
    } else {
        ret = it;
        it++;
        while (it != end()) {
            auto prev = it - 1;
            std::swap(*it, *prev);
            it++;
        }
        pop_back();
    }
    return ret;
}

deque::iterator deque::insert(deque::iterator it, std::string const& el) {
    iterator ret;
    size_t pos = it - begin();
    if (pos < sz / 2) {
        push_front(el);
        for (size_t i = 0; i < pos; ++i) {
            swap(storage[(from + i) % capacity], storage[(from + i + 1) % capacity]);
        }
        ret = iterator(storage + (from + pos) % capacity, storage, storage + capacity);
    } else {
        push_back(el);
        for (size_t i = sz - 1; i > pos; --i) {
            swap(storage[(from + i) % capacity], storage[(from + i - 1) % capacity]);
        }
        ret = iterator(storage + (from + pos) % capacity, storage, storage + capacity);
    }
    return ret;
}

void deque::erase(deque::iterator it1, deque::iterator it2) {
    size_t num = it2 - it1;
    for (auto i1 = it1, i2 = it2; i2 != end(); ++i1, ++i2) {
        swap(*i1, *i2);
    }
    for (size_t i = 0; i < num; ++i) {
        pop_back();
    }
}

bool operator==(deque::iterator const& it1, deque::iterator const& it2) {
    return it1.ptr == it2.ptr;
}

bool operator!=(deque::iterator const& it1, deque::iterator const& it2) {
    return it1.ptr != it2.ptr;
}

bool operator==(deque::const_iterator const& it1, deque::const_iterator const& it2) {
    return it1.ptr == it2.ptr;
}

bool operator!=(deque::const_iterator const& it1, deque::const_iterator const& it2) {
    return it1.ptr != it2.ptr;
}

int operator-(deque::iterator const& it1, deque::iterator const& it2) {
    if (it1.ptr >= it2.ptr) {
        return it1.ptr - it2.ptr;
    }
    return (it1.mostr - it1.mostl) - (it2.ptr - it1.ptr);
}

int operator-(deque::const_iterator const& it1, deque::const_iterator const& it2) {
    if (it1.ptr >= it2.ptr) {
        return it1.ptr - it2.ptr;
    }
    return (it1.mostr - it1.mostl) - (it2.ptr - it1.ptr);
}

deque::iterator operator+(deque::iterator it, int num) {
    return it += num;
}

deque::iterator operator-(deque::iterator it, int num) {
    return it -= num;
}

deque::const_iterator operator+(deque::const_iterator it, int num) {
    return it += num;
}

deque::const_iterator operator-(deque::const_iterator it, int num) {
    return it -= num;
}

void swap(deque& d1, deque& d2) {
    std::swap(d1.capacity, d2.capacity);
    std::swap(d1.sz, d2.sz);
    std::swap(d1.storage, d2.storage);
    std::swap(d1.from, d2.from);
}