#include <cassert>

#include "list.h"

list::node::node() : next(nullptr), prev(nullptr), data() {}

list::node::node(std::string data) : next(nullptr), prev(nullptr), data(data) {}

const list::const_iterator& list::const_iterator::operator=(const_iterator const& other) {
	if (this != &other) {
		if (owner) {
			owner->iterators.erase(this);
		}

		pos = other.pos;
		owner = other.owner;
		owner->iterators.push_front(this);
	}

	return *this;
}

list::const_iterator::const_iterator(const list* owner, node* pos) : owner(owner), pos(pos), valid(true) {
	owner->iterators.push_front(this);
}

list::const_iterator::~const_iterator() {
	owner->iterators.erase(this);
}

std::string const& list::const_iterator::operator*() const {
	assert(is_valid());
	assert(pos != owner->last);
	return pos->data;
}

list::const_iterator& list::const_iterator::operator++() {
	assert(is_valid());
	assert(pos->next);
	pos = pos->next;
	return *this;
}

list::const_iterator& list::const_iterator::operator--() {
	assert(is_valid());
	assert(pos->prev);
	pos = pos->prev;
	return *this;
}

list::const_iterator list::const_iterator::operator++(int) {
	const_iterator r = *this;
	--*this;
	return r;
}

list::const_iterator list::const_iterator::operator--(int) {
	const_iterator r = *this;
	--*this;
	return r;
}

bool list::const_iterator::operator==(const_iterator other) const {
	assert(is_valid());
	assert(other.is_valid());
	return pos == other.pos;
}

bool list::const_iterator::operator!=(const_iterator other) const {
	assert(is_valid());
	assert(other.is_valid());
	return pos != other.pos;
}

bool list::const_iterator::operator==(node* other) const {
	return pos == other;
}

bool list::const_iterator::operator!=(node* other) const {
	return pos != other;
}

bool list::const_iterator::is_valid() const {
	return valid;
}

void list::const_iterator::make_invalid() {
	valid = false;
}

list::iterator::iterator(list* owner, node* pos) : const_iterator(owner, pos) {}

list::iter_list::iter_list() {
	last = new node(nullptr);
	first = last;
}

void list::iter_list::push_front(const_iterator* a) {
	first->prev = new node(a);
	first->prev->next = first;
	first = first->prev;
}

void list::iter_list::make_invalid(list::node* pos) const {
	for (node* i = first; i != last; i = i->next) {
		if (i->data->pos == pos)
			i->data->make_invalid();
	}
}

void list::iter_list::make_invalid_all() const {
	for (node* i = first; i != last; i = i->next) {
		i->data->make_invalid();
	}
}

void list::iter_list::erase(const_iterator* a) {
	for (node* i = first; i != last; i = i->next) {
		if (i->data == a) {
			if (i == first) {
				first = i->next;
				first->prev = nullptr;
			} else {
				i->prev->next = i->next;
				i->next->prev = i->prev;
			}
			delete i;
			return;
		}
	}
}

list::iter_list::node::node(const_iterator* data) : next(nullptr), prev(nullptr), data(data) {}

std::string& list::iterator::operator*() const {
	assert(is_valid());
	assert(pos != owner->last);
	return pos->data;
}

list::list() {
	last = new node();
	first = last;
}

list::list(list const& other) : list() {
	for (auto i : other) {
		push_back(i);
	}
}

const list& list::operator=(list const& other) {
	list tmp(other);
	swap(tmp);
	return *this;
}

void list::swap(list& other) {
	std::swap(first, other.first);
	std::swap(last, other.last);
	std::swap(iterators, other.iterators);
}

list::~list() {
	clear();
	delete last;
}

bool list::empty() const {
	return first == last;
}

void list::clear() {
	while (!empty())
		pop_back();
	
	iterators.make_invalid_all();
}

std::string& list::back() {
	assert(last->prev);
	return last->prev->data;
}

std::string const& list::back() const {
	assert(last->prev);
	return last->prev->data;
}

std::string& list::front() {
	assert(!empty());
	return first->data;
}

std::string const& list::front() const {
	assert(!empty());
	return first->data;
}

void list::pop_back() {
	assert(last->prev);
	
	iterators.make_invalid(last->prev);
	
	if (!last->prev->prev) {
		pop_front();
	} else {
		last->prev = last->prev->prev;
		delete last->prev->next;
		last->prev->next = last;
	}
}

void list::pop_front() {
	assert(!empty());
	
	iterators.make_invalid(first);

	node* tmp = first->next;
	delete first;
	first = tmp;
	first->prev = nullptr;
}

void list::push_back(std::string const& a) {
	if (empty()) {
		push_front(a);
	} else {
		last->prev->next = new node(a);
		last->prev->next->next = last;
		last->prev->next->prev = last->prev;
		last->prev = last->prev->next;
	}
}

void list::push_front(std::string const& a) {
	first->prev = new node(a);
	first->prev->next = first;
	first->prev->prev = nullptr;
	first = first->prev;
}

list::iterator list::begin() {
	return iterator(this, first);
}

list::const_iterator list::begin() const {
	return const_iterator(this, first);
}

list::iterator list::end() {
	return iterator(this, last);
}

list::const_iterator list::end() const {
	return const_iterator(this, last);
}

list::iterator list::insert(iterator pos, std::string const& value) {
	assert(pos.is_valid());
	assert(pos.owner == this);
	if (!pos.pos->prev) {
		push_front(value);
		return iterator(this, first);
	} else {
		pos.pos->prev->next = new node(value);
		pos.pos->prev->next->prev = pos.pos->prev;
		pos.pos->prev->next->next = pos.pos;
		pos.pos->prev = pos.pos->prev->next;
		return iterator(this, pos.pos->prev);
	}
}

list::iterator list::erase(iterator pos) {
	assert(pos.is_valid());
	assert(pos.pos->next);
	assert(pos.owner == this);
	if (!pos.pos->prev) {
		pop_front();
		return iterator(this, first);
	} else {
		iterators.make_invalid(pos.pos);

		pos.pos->prev->next = pos.pos->next;
		pos.pos->next->prev = pos.pos->prev;
		auto tmp = iterator(this, pos.pos->next);
		delete pos.pos;
		return tmp;
	}
}

void list::splice(iterator pos, list& other, iterator first, iterator last) {
	assert(pos.is_valid() && first.is_valid() && last.is_valid());
	assert(first.owner == last.owner);
	assert(first.owner == &other);
	assert(pos.owner == this);

	for (auto i = first; i != last; ) {
		if (first.owner == pos.owner)
			assert(i != pos);
		auto temp = i;
		++i;
		other.iterators.make_invalid(temp.pos);
	}

	node* f = first.pos;
	node* l = last.pos;
	node* real_l = l->prev;
	if (!f->prev) {
		other.first = l;
		other.first->prev = nullptr;
	} else {
		f->prev->next = l;
		l->prev = f->prev;
	}

	if (!pos.pos->prev) {
		this->first = f;
		this->first->prev = nullptr;
	} else {
		pos.pos->prev->next = f;
		f->prev = pos.pos->prev;
	}
	pos.pos->prev = real_l;
	real_l->next = pos.pos;
}