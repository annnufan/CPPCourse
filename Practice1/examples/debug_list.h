#ifndef LIST_H
#define LIST_H
 
#include <string>
#include <list>
 
struct list {
private:
    struct node {
        node *next, *prev;
        std::string data;
 
        node();
        explicit node(std::string data);
    };
 
public:
    struct const_iterator {
        friend list;
 
        const const_iterator& operator=(const_iterator const & other);
        std::string const& operator*() const;
 
        const_iterator& operator++();
        const_iterator& operator--();
 
        const_iterator operator++(int);
        const_iterator operator--(int);
 
        bool operator==(const_iterator other) const;
        bool operator!=(const_iterator other) const;
 
        bool operator==(node* other) const;
        bool operator!=(node* other) const;
   
        ~const_iterator();
    protected:
        const_iterator(const list* owner, node* pos);
 
        bool is_valid() const;
        void make_invalid();
 
        const list* owner;
        node* pos;
        bool valid;
    };
 
    struct iterator : const_iterator {
        friend list;
       
        std::string& operator*() const;
    private:
        iterator(list* owner, node* pos);
    };
 
    struct iter_list {
        iter_list();
 
        void push_front(const_iterator* a);
        void make_invalid(node* pos) const;
        void make_invalid_all() const;
        void erase(const_iterator* a);
 
        struct node {
            node *next, *prev;
            const_iterator* data;
 
            explicit node(const_iterator* data);
        };
 
        node *first, *last;
    };
 
private:
    mutable iter_list iterators;
    node *first, *last;
 
public:
    list();
    list(list const & other);
    const list& operator=(list const & other);
    void swap(list & other);
    ~list();
 
    bool empty() const;
    void clear();
 
    std::string& back();
    std::string const& back() const;
    std::string& front();
    std::string const& front() const;
 
    void pop_back();
    void pop_front();
    void push_back(std::string const& a);
    void push_front(std::string const& a);
 
    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;
 
    iterator insert(iterator pos, std::string const& value);
    iterator erase(iterator pos);
    void splice(iterator pos, list& other, iterator first, iterator last);
};
 
#endif