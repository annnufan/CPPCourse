#ifndef OPTIMIZED_VECTOR_H
#define OPTIMIZED_VECTOR_H

#include <vector>
#include <memory>
#include <iostream>

struct shared_data {
	std::shared_ptr<std::vector<int>> value;

	shared_data();

	shared_data(shared_data const &other);

	void make_unique();
};


struct optimized_vector {

	optimized_vector();

	~optimized_vector();

    optimized_vector(optimized_vector const& other);

	optimized_vector& operator=(optimized_vector const& other);

    friend bool operator==(optimized_vector const& a, optimized_vector const& b);
    friend bool operator!=(optimized_vector const& a, optimized_vector const& b);

    void resize(std::size_t count);
    void resize (std::size_t n, const int& val);

    void insert_begin(optimized_vector const& other);

	void erase_begin(std::size_t count);

	std::size_t size() const;

	int& back();
	const int& back() const;

	void pop_back();

	void push_back (int val);

	int& operator[] (std::size_t n);
	const int& operator[] (std::size_t n) const;

	friend std::ostream& operator<<(std::ostream& s, optimized_vector const& a); 

private:
	bool is_small, is_empty;
	union {
		int small_data;
		shared_data *big_data;
	};

	void make_unique();
};

bool operator==(optimized_vector const& a, optimized_vector const& b);
bool operator!=(optimized_vector const& a, optimized_vector const& b);

std::ostream& operator<<(std::ostream& s, optimized_vector const& a);

#endif // OPTIMIZED_VECTOR_H