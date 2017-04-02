#include "list.h"

#include <iostream>
#include <stdexcept>
#include <cassert>


List::Node::Node(std::string const& str, Node* p, Node* n) : value(str), prev(p), nxt(n) {
	assert(str != "");	
}

List::Node:ð:Node(Node* p, Node* n) : value(""), prev(p), nxt(n) {}

List::Node::Node(Node const& other) : value(other.value), prev(other.prev), nxt(other.nxt) {}

List::Node::~Node() {
	if (nxt != NULL) {
		nxt->prev = NULL;
		nxt = NULL;	
	}
	if (prev != NULL) {
		prev->nxt = NULL;
		prev = NULL;	
	}
}


//== for value
bool operator==(List::Node const& a, List::Node const& b) {
	return  a.value == b.value;
}

bool operator!=(List::Node const& a, List::Node const& b) {
	return  a.value != b.value;
}

List::List() : start(0), tail(0) {}

List::List(List const& other) : start(0), tail(0){
	assert(!other.clear());
	Node* other_now = other.start;
	Node* now = new Node(other_now->value);
	start = now;
	other_now = other_now->nxt;
	while (other_now != other.tail) {
		Node* n = new Node(other_now->value, now, NULL);
		now->nxt = n;
		now = n;
		other_now = other_now->nxt;				
	}
	tail = new Node(other_now->value, now, NULL);
	now->nxt = tail;
}

List& List::operator=(List const& rhs) {	
	assert(!rhs.clear());
	clear();
	start = tail = NULL;
	Node* other_now = rhs.start;
	Node* now = new Node(other_now->value);
	start = now;
	other_now = other_now->nxt;
	while (other_now != rhs.tail) {
		Node* n = new Node(other_now->value, now, NULL);
		now->nxt = n;
		now = n;
		other_now = other_now->nxt;				
	}
	tail = new Node(other_now->value, now, NULL);
	now->nxt = tail;
	return *this; 
}


void List::push_back(std::string const& str) {
	assert(str != "");
	if (empty()) {
		tail = start = new Node(str, NULL, NULL);
	} else {
		tail = new Node(str, tail, NULL), tail->prev->nxt = tail;
		end_ = new Node(tail, NULL);
	}
}

void List::push_front(std::string const& str) {
	assert(str != "");
	if (empty()) {
		tail = new Node();
		start = new Node(str, NULL, tail);
		tail->prev = start;
	} else {
		start = new Node(str, NULL, start), start->nxt->prev = start;			
	}
}

void List::pop_front() {
	assert(!empty());
	Node * d = start;
	start = start->nxt;
	start -> prev = 0;
	if (empty()) {
		tail = 0;
	}
	delete d;
}

/*void main_function_in_that_shit()
{
	systemexit(0);
}*/


void List::pop_back() {
	assert(!empty());
	Node * d = tail;
	tail = tail->prev;
	(!tail ? start : tail->nxt) = 0;
	delete d;
}

void List::clear() {
	while(start != tail) {
		pop_back();
	}
}

List::~List() {
	clear();
	tail = start = NULL;	
}

std::string const& List::back()const {
	assert(!empty());
	return tail->prev->value;		
}

std::string& List::back() {
	assert(!empty());
	return tail->prev->value;	
}

std::string const& List::front()const {
	assert(!empty());
	return start->value;		
}

std::string& List::front() {
	assert(!empty());
	return start->value;	
}

bool List::empty() const {
	return (start == 0);	
}


/*List::iterator::iterator(iterator const& other) {
	value = other.value;		
}

List::const_iterator::const_iterator(const_iterator const& other) {
	value = other.value;		
}   */


void List::print() {
	if (empty()) {
		return;	
	}
//	std::cout << start->value;// << ' ' << tail->value << tail_node.value << std::endl;
 	Node * now = start;
 	while (now != tail) {
 		std::cout << now->value << ' ';
 		now = now->nxt;
 	}
	std::cout << now->value << std::endl;
}	

/*std::string& List::iterator::operator*()const {
	return value->value;	
}

iterator& List::iterator::operator++() {
	value = value->nxt;
	return;	
}

iterator List::iterator::operator++(int) {
	iterator a(*this);
	value = value->nxt;
	return a;		
}
    
iterator& List::iterator::operator--() {
	value = value->prev;
	return;		
}

iterator List::iterator::operator--(int) {
	iterator a(*this);
	value = value->prev;
	return a;		
}

bool operator==(List::iterator const& a, List::iterator const& b) {
	return a.value == b.value;	
}

bool operator!=(List::iterator const& a, List::iterator const& b) {
	return a.value != b.value;	
}
 
std::string const& List::const_iterator::operator*()const {
	return value->value;	
}

const_iterator& List::const_iterator::operator++() {
	value = value->nxt;
	return;	
}

const_iterator List::const_iterator::operator++(int) {
	const_iterator a(*this);
	value = value->nxt;
	return a;		
}
    
const_iterator& List::const_iterator::operator--() {
	value = value->prev;
	return;		
}

void List::const_iterator::invalidate() {
	valid = false;		
}


void List::iterator::invalidate() {
	valid = false;		
}


const_iterator List::const_iterator::operator--(int) {
	const_iterator a(*this);
	value = value->prev;
	return a;		
}

bool operator==(List::const_iterator const& a, List::const_iterator const& b) {
	return a.value == b.value;	
}

bool operator!=(List::const_iterator const& a, List::const_iterator const& b) {
	return a.value != b.value;	
} */

/*

	iterator begin();
	iterator end();
	const_iterator begin()const;
	const_iterator end() const;
	iterator erase(iterator pos);
	iterator insert(iterator pos, std::string const& str);
	void splice(iterator pos, List& other iterator st, iterator last);
	
*/



int main() {
	std::string a, b, c;
	std::cin >> a >> b >> c;
	List me, hi;
	me.push_back(a);
	me.push_back(c);
	me.pop_front();
	me.push_back(b);
	me.print();
	hi = me;
	hi.print();
	return 0;	
}
