#ifndef DEQUE_H
#define DEQUE_H

#include <string>

struct deque {
  private:
  
  void del(int pos);
  void create(int pos, std::string const& val);
  
  static const int START_SIZE = 4;
  
  std::string *a;
  
  size_t lok_size;
  int first, last, max_size;
  
  void upd_size();
  
  public:
  
  friend void swap(deque& c, deque &v);
  
  struct iterator {
    private:
    deque *deq;
    int cur;
    iterator(deque* deq, int cur);
    int pos() const;
    public:
    iterator(): deq(0), cur(0) {}
    
    iterator &operator++ ();
    iterator operator++ (int);
    
    iterator &operator-- ();
    iterator operator-- (int);
    
    iterator &operator+= (int shift);
    iterator &operator-= (int shift);
    
    
    std::string const& operator *();
    
    friend bool operator==(iterator const& c, iterator const& v);
    friend bool operator!=(iterator const& c, iterator const& v);
    
    friend iterator operator+ (iterator c, int shift);
    friend iterator operator- (iterator c, int shift);
    
    friend int operator- (iterator const& c, iterator const& v);
    friend struct deque;
  };
  
  struct const_iterator {
    private:
    deque const* deq;
    int cur;
    const_iterator(deque const* deq, int cur);
    int pos() const;
    public:
    const_iterator(): deq(0), cur(0) {}
    
    const_iterator &operator++ ();
    const_iterator operator++ (int);
    
    const_iterator &operator-- ();
    const_iterator operator-- (int);
    
    const_iterator &operator+= (int shift);
    const_iterator &operator-= (int shift);
    
    std::string const& operator *() const;
    
    friend bool operator==(const_iterator const& c, const_iterator const& v);
    friend bool operator!=(const_iterator const& c, const_iterator const& v);
    
    const_iterator friend  operator+ (const_iterator c, int shift);
    const_iterator friend  operator- (const_iterator c, int shift);
    
    friend int operator- (const_iterator const& c, const_iterator const& v);
    friend struct deque;
  };
  
  
  deque();
  deque(deque const& v);
  deque& operator=(deque const&);
  
  iterator begin();
  iterator end();
  
  const_iterator begin() const;
  const_iterator end() const;
  
  
  iterator insert(iterator pos, std::string const& val);
  iterator erase(iterator);
  void erase(iterator first, iterator last);
  
  void push_back(std::string const&);
  void pop_back();
  
  std::string const& back() const;
  std::string & back();
  
  void push_front(std::string const&);
  void pop_front();
  
  std::string const& front() const;
  std::string & front();
  
  
  size_t size() const;
  std::string& operator[](size_t index);
  std::string const& operator[](size_t index) const;
  bool empty() const;
  
  void clear();
  ~deque();
};

#endif // DEQUE_H