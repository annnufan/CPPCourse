#include "optimized_vector.h"

#include <cassert>

optimized_vector::optimized_vector() {
	is_small = true;
	is_empty = true;
}

optimized_vector::optimized_vector(optimized_vector const& other) {
	is_small = other.is_small;
	is_empty = other.is_empty;
	if (is_empty) return;
	if (is_small) {
		small_data = other.small_data;
		return;
	}
	big_data = other.big_data;
}

optimized_vector& optimized_vector::operator=(optimized_vector const& other) {
	optimized_vector ex(other);
	is_small = ex.is_small;
	is_empty = ex.is_empty;
	small_data = ex.small_data;
	std::swap(big_data, ex.big_data);
	return *this;	
}

void optimized_vector::resize(std::size_t count) {
	if (is_small) {
		if (count > 1) {
			is_empty = is_small = false;
			big_data = new std::shared_ptr<std::vector<int>>(new std::vector<int>);
			big_data->get()->resize(count);
			if (!is_empty) {
				big_data->get()->at(0) = small_data;
			}
			small_data = 0;
		}
		return;
	}
	if (count == 1) {
		make_unique();
		is_small = true;
		small_data = big_data->get()->at(0);
		delete big_data;
		return;
	}
	big_data->get()->resize(count);
}

void optimized_vector::clear(){
	if (!is_small) {
		delete big_data;
	}
	is_empty = is_small = true;
	small_data = 0;
}

void optimized_vector::resize(std::size_t n, const int& val) {
	if (is_small) {
		if (n > 1) {
			is_empty = is_small = false;
			big_data = new std::shared_ptr<std::vector<int>>(new std::vector<int>);
			big_data->get()->resize(n, val);
			if (!is_empty) {
				big_data->get()->at(0) = small_data;
			}
			small_data = 0;
		}
		return;
	}
	if (n == 1) {
		make_unique();
		is_small = true;
		small_data = big_data->get()->at(0);
		delete big_data;
		return;
	}
	big_data->get()->resize(n, val);


}

std::size_t optimized_vector::size() const {
	if (is_empty)
		return 0;
	if (is_small)
		return 1;
	return big_data->get()->size();
}

int& optimized_vector::back() {
	assert(size() != 0);
	if (is_small) {
		return this->small_data;
	}
	make_unique();
	return this->big_data->get()->back();
}


const int& optimized_vector::back() const {
	assert(size() != 0);
	if (is_small) {		
		return this->small_data;
	}
	return this->big_data->get()->back();
}

int& optimized_vector::operator[](std::size_t n) {
	if (is_small)
		return this->small_data;
	assert(size() > n);
	make_unique();
	return this->big_data->get()->at(n);
}

const int& optimized_vector::operator[] (std::size_t n) const {
	if (is_small)
		return this->small_data;
	assert(size() > n);
	return this->big_data->get()->at(n);	
}

void optimized_vector::pop_back() {
	assert(size() != 0);
	if (is_small) {
		is_empty = true;
		is_small = 0;
		return;
	}
	make_unique();
	if (size() > 2) {
		big_data->get()->pop_back();
		return;
	}
	is_small = true;
	small_data = big_data->get()->at(0);
	delete big_data;
}

void optimized_vector::push_back(int val) {
	if (is_empty) {
		is_empty = false;
		small_data = val;
		return;
	}
	if (!is_small) {
		make_unique();
		big_data->get()->push_back(val);
		return;
	}
	is_small = false;
	big_data = new std::shared_ptr<std::vector<int>>(new std::vector<int>);
	big_data->get()->push_back(small_data);
	big_data->get()->push_back(val);
	small_data = 0;
}

bool operator==(optimized_vector const& a, optimized_vector const& b) {
	if ((a.is_empty != b.is_empty) || (a.is_small != b.is_small))
		return false;
	if (a.is_empty)
		return true;
	if (a.is_small)
		return a.small_data == b.small_data;
	return a.big_data->get() == b.big_data->get();
}

bool operator!=(optimized_vector const& a, optimized_vector const& b) {
	return !(a == b);
}

void optimized_vector::insert_begin(optimized_vector other) {
	if (other.is_empty)
		return;
	make_unique();
	if (is_small) {
		if (is_empty && other.is_small) {
			is_empty = false;
			small_data = other.small_data;
			return;
		}
		big_data = new std::shared_ptr<std::vector<int>>(new std::vector<int>);
		if (!is_empty) {
			big_data->get()->push_back(small_data);
		}
		small_data = 0;
		is_small = false;
	}
	if (other.is_small) {
		std::vector<int> ex(other.small_data);
		big_data->get()->insert(big_data->get()->begin(), ex.begin(), ex.end());
		return; 
	}
	big_data->get()->insert(big_data->get()->begin(), other.big_data->get()->begin(), other.big_data->get()->end());
}

void optimized_vector::erase_begin(std::size_t count) {
	assert(count <= size());
	if (count == 0)
		return;
	make_unique();
	if (count == size()) {
		if (!is_small) {
			delete big_data;
		}
		is_empty = is_small = true;
		small_data = 0;
		return;
	}
	if (size() - count == 1) {
		small_data = big_data->get()->back();
		delete big_data;
		is_small = true;
		return;
	}
	big_data->get()->erase(big_data->get()->begin(), big_data->get()->begin() + count);
}

optimized_vector::~optimized_vector(){
	make_unique();
	if (!is_small)
		delete big_data;
}

void optimized_vector::make_unique(){
	if (!is_small && !big_data->unique()) {
		big_data = new std::shared_ptr<std::vector<int>>(*big_data);
	}
}