#ifndef LIST_H    
#define LIST_H

#include <string> 


struct List{

	struct Node{
		std::string value;
		Node *prev, *nxt;
		Node(std::string const& str, Node* n = NULL, Node* p = NULL);
		Node(Node* n = NULL, Node* p = NULL);
		Node(Node const& other);	
		~Node();
		friend bool operator==(Node const& a, Node const& b);
		friend bool operator!=(Node const& a, Node const& b);
	};
	
	struct iterator {
	 	std::string& operator*()const;
		iterator& operator++();
	    iterator operator++(int);
    
		iterator& operator--();
		iterator operator--(int); 

    	friend bool operator==(iterator const& a, iterator const& b);
    	friend bool operator!=(iterator const& a, iterator const& b);
    	
    	iterator(iterator const& other);
    	
    	void invalidate()
    	 	 	
	private:
		Node * value;
		bool valid;
	};
	
	struct const_iterator {
	 	std::string const& operator*()const;
		const_iterator& operator++();
	    const_iterator operator++(int);
    
		const_iterator& operator--();
		const_iterator operator--(int); 

    	friend bool operator==(const_iterator const& a, const_iterator const& b);
    	friend bool operator!=(const_iterator const& a, const_iterator const& b);
 	 	const_iterator(const_iterator const& other);
		void invalidate()
    	
	private:
		Node * value;
		bool valid;
	};
	


	List();
	List(List const& other);
	List& operator=(List const& rhs);
	void push_back(std::string const& str);
	~List();
	void pop_back();
	void clear();
	std::string const& back()const;
	std::string& back();
	void push_front(std::string const& str);
	std::string const& front() const;
	std::string& front();
	void pop_front();
	bool empty() const;
	
	
	iterator begin();
	iterator end();
	const_iterator begin()const;
	const_iterator end() const;
	iterator erase(iterator pos);
	iterator insert(iterator pos, std::string const& str);
	void splice(iterator pos, List& other,  iterator st, iterator last);
	
	
	void print();
		
private:
	Node* start;
	Node* tail;
};

bool operator==(List::Node const& a, List::Node const& b);	
bool operator!=(List::Node const& a, List::Node const& b);	
bool operator==(List::iterator const& a, List::iterator const& b);	
bool operator!=(List::iterator const& a, List::iterator const& b);	
bool operator==(List::const_iterator const& a, List::const_iterator const& b);	
bool operator!=(List::const_iterator const& a, List::const_iterator const& b);	



#endif // LIST_H