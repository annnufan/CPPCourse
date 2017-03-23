#ifndef OPTIMIZED_VECTOR_H
#define OPTIMIZED_VECTOR_H

#include <vector>
#include <memory>


struct optimized_vector {

	optimized_vector();

	~optimized_vector();

    optimized_vector(optimized_vector const& other);

	optimized_vector& operator=(optimized_vector const& other);

    friend bool operator==(optimized_vector const& a, optimized_vector const& b);
    friend bool operator!=(optimized_vector const& a, optimized_vector const& b);

    void resize(std::size_t count);
    void resize (std::size_t n, const int& val);

    void insert_begin(optimized_vector other);

	void erase_begin(std::size_t count);

	void clear();

	std::size_t size() const;

	int& back();
	const int& back() const;

	void pop_back();

	void push_back (int val);

	int& operator[] (std::size_t n);
	const int& operator[] (std::size_t n) const;

private:
	bool is_small, is_empty;

	int small_data;

	std::shared_ptr<std::vector<int>> *big_data;

	void make_unique();
};

bool operator==(optimized_vector const& a, optimized_vector const& b);
bool operator!=(optimized_vector const& a, optimized_vector const& b);

#endif // OPTIMIZED_VECTOR_H