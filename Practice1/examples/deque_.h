#ifndef DEQUE_H
#define DEQUE_H

#include <iostream>

struct deque {
    struct iterator {
        friend deque;

        private:
            std::string* ptr;
            std::string *mostl, *mostr;

        public:
            
            iterator();            
            iterator(std::string*, std::string*, std::string*);
            iterator(iterator const&);
            std::string& operator*();
            iterator& operator=(iterator const&);
            iterator& operator++();
            iterator& operator--();
            iterator operator++(int);
            iterator operator--(int);
            iterator& operator+=(int);
            iterator& operator-=(int);
            
            friend bool operator==(iterator const& it1, iterator const& it2);
            friend bool operator!=(iterator const& it1, iterator const& it2);
            friend int operator-(iterator const& it1, iterator const& it2);        
    };

    struct const_iterator {
        friend deque;

        private:
            std::string* ptr;
            std::string *mostl, *mostr;
            
        public:
            
            const_iterator();            
            const_iterator(std::string*, std::string*, std::string*);
            const_iterator(const_iterator const&);
            std::string const& operator*() const;
            const_iterator& operator=(const_iterator const&);
            const_iterator& operator++();
            const_iterator& operator--();
            const_iterator operator++(int);
            const_iterator operator--(int);
            const_iterator& operator+=(int);
            const_iterator& operator-=(int);
            
            friend bool operator==(const_iterator const& it1, const_iterator const& it2);
            friend bool operator!=(const_iterator const& it1, const_iterator const& it2);        
            friend int operator-(const_iterator const& it1, const_iterator const& it2);
    };

    private:
        
        size_t capacity;
        size_t sz;
        size_t from;
        std::string* storage;

        void resize_x2();
        deque(deque const&, size_t);
    public:
        
        deque();
        deque(deque const&);
        ~deque();
        deque& operator=(deque const&);
        bool empty() const;
        void clear();
        
        std::string& back();
        std::string const& back() const;
        void pop_back();
        void push_back(std::string const&);
        
        std::string& front();
        std::string const& front() const;
        void pop_front();
        void push_front(std::string const&);
        
        size_t size() const;
        std::string& operator[](size_t);
        std::string const& operator[](size_t) const;
        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
        iterator erase(iterator);
        iterator insert(iterator, std::string const&);
        void erase(iterator, iterator);

        friend void swap(deque& d1, deque& d2);
};

bool operator==(deque::iterator const& it1, deque::iterator const& it2);
bool operator!=(deque::iterator const& it1, deque::iterator const& it2);
bool operator==(deque::const_iterator const& it1, deque::const_iterator const& it2);
bool operator!=(deque::const_iterator const& it1, deque::const_iterator const& it2);
int operator-(deque::iterator const&, deque::iterator const&);
int operator-(deque::const_iterator const&, deque::const_iterator const&);
deque::iterator operator+(deque::iterator, int);
deque::iterator operator-(deque::iterator, int);
deque::const_iterator operator+(deque::const_iterator, int);
deque::const_iterator operator-(deque::const_iterator, int);

void swap(deque& d1, deque& d2);

#endif