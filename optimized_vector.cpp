#include "optimized_vector.h"

#include <cassert>

shared_data::shared_data() : value(std::make_shared<std::vector<int>>()) {}

shared_data::shared_data(shared_data const &other) : value(other.value) {}

void shared_data::make_unique() {
	if (!this->value.unique())
		this->value = std::make_shared<std::vector<int>>(*this->value);
}

shared_data::~shared_data() {
	if (value != nullptr)
		delete value;
}

optimized_vector::optimized_vector() {
	is_small = true;
	is_empty = true;
	small_data = 0;
}

optimized_vector::optimized_vector(optimized_vector const& other) {
	is_small = other.is_small;
	is_empty = other.is_empty;
	if (is_empty) return;
	if (is_small) {
		small_data = other.small_data;
		return;
	}
	big_data = new shared_data(*other.big_data);
}

optimized_vector& optimized_vector::operator=(optimized_vector const& other) {
    is_small = other.is_small;
    is_empty = other.is_empty;
    if (is_empty) return *this;
    if (is_small) {
        small_data = other.small_data;
        return *this;
    }
    big_data = new shared_data(*other.big_data);
	return *this;	
}

void optimized_vector::resize(std::size_t count) {
	if (is_small) {
		if (count > 1) {
			is_small = false;
			int tmp = small_data;
			big_data = new shared_data;
			if (!is_empty) {
				big_data->value.get()->push_back(tmp);
			}
			big_data->value.get()->resize(count);
		}
		is_empty = (count == 0);
		return;
	}
	if (count == 1) {
		is_small = true;
		int tmp = big_data->value.get()->at(0);
		delete big_data;
		small_data = tmp;
		is_empty = false;
		return;
	}
	make_unique();
	is_empty = false;
	big_data->value.get()->resize(count);
}

void optimized_vector::resize(std::size_t n, const int& val) {
	if (is_small) {
		if (n > 1) {
			is_small = false;
			int tmp = small_data;
			big_data = new shared_data;
			if (!is_empty) {
				big_data->value.get()->push_back(tmp);
			}
			big_data->value.get()->resize(n, val);
		}
		is_empty = (n == 0);
		return;
	}
	if (n == 1) {
		is_small = true;
		int tmp = big_data->value.get()->at(0);
		delete big_data;
		small_data = tmp;
		is_empty = false;
		return;
	}
	make_unique();
	big_data->value.get()->resize(n, val);
	is_empty = false;
}

std::size_t optimized_vector::size() const {
	if (is_empty)
		return 0;
	if (is_small)
		return 1;
	return big_data->value.get()->size();
}

int& optimized_vector::back() {
	assert(size() != 0);
	if (is_small) {
		return this->small_data;
	}
	make_unique();
	return this->big_data->value.get()->back();
}


const int& optimized_vector::back() const {
	assert(size() != 0);
	if (is_small) {		
		return this->small_data;
	}
	return this->big_data->value.get()->back();
}

int& optimized_vector::operator[](std::size_t n) {
	assert(size() > n);
	if (is_small)
		return this->small_data;
	make_unique();
	return this->big_data->value.get()->at(n);
}

const int& optimized_vector::operator[] (std::size_t n) const {
	assert(size() > n);
	if (is_small)
		return this->small_data;
	return this->big_data->value.get()->at(n);
}

void optimized_vector::pop_back() {
	assert(size() != 0);
	if (is_small) {
		is_empty = true;
		small_data = 0;
		return;
	}
	if (size() > 2) {
		make_unique();
		big_data->value.get()->pop_back();
		return;
	}
	is_small = true;
	int tmp = big_data->value.get()->at(0);
	delete big_data;
	small_data = tmp;
}

void optimized_vector::push_back(int val) {
	if (is_empty) {
		is_empty = false;
		small_data = val;
		return;
	}
	if (!is_small) {
		make_unique();
		big_data->value.get()->push_back(val);
		return;
	}
	is_small = false;
	int tmp = small_data;
	big_data = new shared_data;
	big_data->value.get()->push_back(tmp);
	big_data->value.get()->push_back(val);
}

bool operator==(optimized_vector const& a, optimized_vector const& b) {
	if ((a.is_empty != b.is_empty) || (a.is_small != b.is_small))
		return false;
	if (a.is_empty)
		return true;
	if (a.is_small)
		return a.small_data == b.small_data;
	return *(a.big_data->value.get()) == *(b.big_data->value.get());
}

bool operator!=(optimized_vector const& a, optimized_vector const& b) {
	return !(a == b);
}

void optimized_vector::insert_begin(optimized_vector const& other) {
	if (other.is_empty)
		return;
	make_unique();
	if (is_small) {
		if (is_empty && other.is_small) {
			is_empty = false;
			small_data = other.small_data;
			return;
		}
		int tmp = small_data;
		big_data = new shared_data;
		if (!is_empty) {
			big_data->value.get()->push_back(tmp);
		}
		is_small = false;
	}
	if (other.is_small) {
		big_data->value.get()->insert(big_data->value.get()->begin(), other.small_data);
		return; 
	}
	big_data->value.get()->insert(big_data->value.get()->begin(), other.big_data->value.get()->begin(), other.big_data->value.get()->end());
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
		int tmp = big_data->value.get()->back();
		delete big_data;
		small_data = tmp;
		is_small = true;
		return;
	}
	big_data->value.get()->erase(big_data->value.get()->begin(), big_data->value.get()->begin() + count);
}

optimized_vector::~optimized_vector(){
	//if (!is_small)
	delete big_data;
}

void optimized_vector::make_unique(){
	if (!is_small) {
		big_data->make_unique();
	}
}

std::ostream& operator<<(std::ostream& s, optimized_vector const& a) {
	if (a.is_empty)
		return s;
	if (a.is_small){
		return s << a.small_data;
	}
	for (auto x : *(a.big_data->value.get())) {
		s << x << ' ';
	}
	return s;
}