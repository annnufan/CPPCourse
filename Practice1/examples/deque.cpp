#include "deque.h"

#include <cassert>
#include <iostream>
#include <string>
using namespace std;

void deque::del(int pos) {
  (&a[pos]) -> ~basic_string();
}
void deque::create(int pos, string const& val) {
  new (&a[pos]) std::string(val);
}

void deque::upd_size() {
  if (lok_size + 1 >= max_size) {
    std::string *s = (std::string*) operator new(max_size * 2 * sizeof(std::string));
    
    int t = 0;
    try {
      for (iterator c = begin(); c != end(); ++c) {
        new (&s[t]) std::string(*c);
        t++;
      }
    } catch (...) {
      for (t--; t >= 0; t--) {
        del(t);
      }
      operator delete (s);
      throw;
    }
    for (iterator c = begin(); c != end(); ++c) {
      del(c.cur);
    }
    operator delete (a);
    a = s;
    max_size *= 2;
    first = 0;
    last = t;
  }
}

void swap(deque& c, deque &v) {
  swap(c.a,        v.a);
  swap(c.lok_size, v.lok_size);
  swap(c.first,    v.first);
  swap(c.last,     v.last);
  swap(c.max_size, v.max_size);
}

////////////////////////////////

int deque::iterator::pos() const{
  if (deq->first <= cur) {
    return cur - deq->first;
  }
  return deq->max_size - deq->first + cur;
}

deque::iterator &deque::iterator::operator++ () {
  assert(cur != deq->last);
  cur++;
  if (cur == deq->max_size) {
    cur = 0;
  }
  return *this;
}
deque::iterator deque::iterator::operator++ (int) {
  iterator res = *this;
  ++(*this);
  return res;
}

deque::iterator &deque::iterator::operator-- () {
  assert(cur != deq->first);
  cur--;
  if (cur < 0) {
    cur += deq->max_size;
  }
  return *this;
}
deque::iterator deque::iterator::operator-- (int) {
  iterator res = *this;
  --(*this);
  return res;
}

deque::iterator &deque::iterator::operator+= (int shift) {
  cur = (cur + shift) % (deq->max_size);
  if (cur < 0) {
    cur += deq->max_size;
  }
  return *this;
}
deque::iterator &deque::iterator::operator-= (int shift) {
  return (*this += -shift);
}

std::string const& deque::iterator::operator *() {
  return deq->a[cur];
}

bool operator==(deque::iterator const& c, deque::iterator const& v) {
  return c.cur == v.cur;
}
bool operator!=(deque::iterator const& c, deque::iterator const& v) {
  return c.cur != v.cur;
}

deque::iterator operator+ (deque::iterator c, int shift) {
  return (c += shift);
}
deque::iterator operator- (deque::iterator c, int shift) {
  return (c -= shift);
}

int operator- (deque::iterator const& c, deque::iterator const& v) {
  return c.pos() - v.pos();
}


////////////////////////////


int deque::const_iterator::pos() const{
  if (deq->first <= cur) {
    return cur - deq->first;
  }
  return deq->max_size - deq->first + cur;
}

deque::const_iterator &deque::const_iterator::operator++ () {
  assert(cur != deq->last);
  cur++;
  if (cur == deq->max_size) {
    cur = 0;
  }
  return *this;
}
deque::const_iterator deque::const_iterator::operator++ (int) {
  const_iterator res = *this;
  ++(*this);
  return res;
}

deque::const_iterator &deque::const_iterator::operator-- () {
  assert(cur != deq->first);
  cur--;
  if (cur < 0) {
    cur += deq->max_size;
  }
  return *this;
}
deque::const_iterator deque::const_iterator::operator-- (int) {
  const_iterator res = *this;
  --(*this);
  return res;
}

deque::const_iterator &deque::const_iterator::operator+= (int shift) {
  cur = (cur + shift) % (deq->max_size);
  if (cur < 0) {
    cur += deq->max_size;
  }
  return *this;
}
deque::const_iterator &deque::const_iterator::operator-= (int shift) {
  return (*this += -shift);
}

std::string const& deque::const_iterator::operator *() const{
  return deq->a[cur];
}

bool operator==(deque::const_iterator const& c, deque::const_iterator const& v) {
  return c.cur == v.cur;
}
bool operator!=(deque::const_iterator const& c, deque::const_iterator const& v) {
  return c.cur != v.cur;
}

deque::const_iterator operator+ (deque::const_iterator c, int shift) {
  return (c += shift);
}
deque::const_iterator operator- (deque::const_iterator c, int shift) {
  return (c -= shift);
}

int operator- (deque::const_iterator const& c, deque::const_iterator const& v) {
  return c.pos() - v.pos();
}

////////////////////////////////////////////////////

deque::deque()
  : a((std::string*) operator new(START_SIZE * sizeof(std::string))),
    first(0), last(0), max_size(START_SIZE), lok_size(0)
{}

deque::deque(deque const& v)
  : deque() {
  
  for (const_iterator c = v.begin(); c != v.end(); ++c) {
      push_back(*c);
  }
}

deque& deque::operator=(deque const& v) {
  if (this != &v) {
    deque new_deq(v);
    swap(*this, new_deq);
  }
  return *this;
}

deque::iterator::iterator(deque *deq, int cur) 
  : deq(deq), cur(cur) {}
deque::const_iterator::const_iterator(deque const* deq, int cur)
  : deq(deq), cur(cur) {}

deque::iterator deque::begin() {
  return iterator(this, first);
}
deque::iterator deque::end() {
  return iterator(this, last);
}

deque::const_iterator deque::begin() const{
  return const_iterator(this, first);
}
deque::const_iterator deque::end() const{
  return const_iterator(this, last);
}



deque::iterator deque::insert(iterator pos, std::string const& val) {
  int my_pos = pos.pos();
  
  iterator c = end();
  
  if (my_pos < lok_size - my_pos) {
    push_front(val);
    for (c = begin(); c.pos() < my_pos; ++c) {
      swap(a[c.cur], a[(c + 1).cur]);
    }
  } else {
    push_back(val);
    for (c = end() - 1; c.pos() > my_pos; --c) {
      swap(a[c.cur], a[(c - 1).cur]);
    }
  }
  return c;
}
deque::iterator deque::erase(iterator pos) {
  assert(pos != end());
  int my_pos = pos.pos();
  
  iterator c = end();
  
  if (my_pos < lok_size - my_pos) {
    for (c = pos; c.pos() > 0; --c) {
      swap(a[c.cur], a[(c - 1).cur]);
    }
    pop_front();
  } else {
    for (c = pos; c.pos() < size() - 1; ++c) {
      swap(a[c.cur], a[(c + 1).cur]);
    }
    pop_back();
  }
  return c;
}
void deque::erase(deque::iterator first, deque::iterator last) {
  int l = first.pos(), r = last.pos();
  assert(l <= r);
  if (first == last) {
    return;
  }
  
  iterator c, v;
  
  if (l < lok_size - r) {
    c = last;
    v = first;
    
    while (v != begin()) {
      --c;
      --v;
      swap(a[c.cur], a[v.cur]);
    }
    for (r -= l; r; r--) {
      pop_front();
    }
  } else {
    v = last;
    c = first;
    
    while (v != end()) {
      swap(a[c.cur], a[v.cur]);
      ++c;
      ++v;
    }
    for (r -= l; r; r--) {
      pop_back();
    }
  }
}


void deque::push_back(std::string const& val) {
  upd_size();
  create(last, val);
  
  lok_size++;
  last++;
  if (last == max_size) {
    last = 0;
  }
}
void deque::pop_back() {
  assert(lok_size != 0);
  last--;
  if (last < 0) {
    last += max_size;
  }
  del(last);
  lok_size--;
}

std::string const& deque::back() const {
  return a[(max_size + last - 1) % max_size];
}
std::string & deque::back() {
  return a[(max_size + last - 1) % max_size];
}

void deque::push_front(std::string const& val) {
  upd_size();
  first--;
  if (first < 0) {
    first += max_size;
  }
  try {
    create(first, val);
  } catch (...) {
    first = (first + 1) % max_size;
  }
  lok_size++;
}
void deque::pop_front() {
  assert(lok_size != 0);

  del(first);
  first++;
  if (first == max_size) {
    first = 0;
  }
  lok_size--;
}

std::string const& deque::front() const {
  assert(lok_size != 0);
  return a[first];
}
std::string & deque::front() {
  assert(lok_size != 0);
  return a[first];
}


size_t deque::size() const{
  return lok_size;
}

std::string& deque::operator[](size_t index) {
  assert(index < lok_size);
  
  int res = first + index;
  if (res >= max_size) {
    res -= max_size;
  }
  return a[res];
}
std::string const& deque::operator[](size_t index) const {
  assert(index < lok_size);
  
  int res = first + index;
  if (res >= max_size) {
    res -= max_size;
  }
  return a[res];
}


bool deque::empty() const{
  return lok_size != 0;
}

void deque::clear() {
  iterator c = begin();
  for (; c != end(); c++) {
    del(c.cur);
  }
  lok_size = 0;
  first = last = 0;
}

deque::~deque() {
  clear();
  operator delete (a);
}