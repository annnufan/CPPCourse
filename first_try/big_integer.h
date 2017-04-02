#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <iosfwd>
#include <memory>

struct big_integer
{
	struct my_vector{
		size_t const& size() const;
		int& back();
		
		void pop_back();
		
		void push_back(int x);
		
		void resize(size_t s, int v = 0);
		
		my_vector();
		my_vector(my_vector const& other);
		my_vector& operator=(my_vector const& other);
		int const& operator[](int ind)const;
		
		int& operator[](int ind);
		friend bool operator==(my_vector const& a, my_vector const& b);	
		friend bool operator!=(my_vector const& a, my_vector const& b);	
		
		std::shared_ptr<int> head;

		const std::shared_ptr<int> begin() const;

		std::shared_ptr<int> begin();
		std::shared_ptr<int> erase_pref(int k);

		std::shared_ptr<int> insert_to_begin(std::shared_ptr<int> start, int s);	
	private:
	    int x;
	    std::shared_ptr<int> my_own;
	    size_t my_size, all_size;
	    bool cow;
	    void copy(std::shared_ptr<int> x, std::shared_ptr<int> y, size_t s);
	};

    big_integer(big_integer const& other);
    big_integer(my_vector d, int s);
    big_integer(int a = 0);
    explicit big_integer(std::string const& str);
    

    big_integer& operator=(big_integer const& other);

    big_integer& operator+=(big_integer const& rhs);
    big_integer& operator-=(big_integer const& rhs);
    big_integer& operator*=(big_integer const& rhs);
    big_integer& operator/=(big_integer const& rhs);
    big_integer& operator%=(big_integer const& rhs);

    big_integer& operator&=(big_integer const& rhs);
    big_integer& operator|=(big_integer const& rhs);
    big_integer& operator^=(big_integer const& rhs);

    big_integer& operator<<=(int rhs);
    big_integer& operator>>=(int rhs);  

    big_integer operator+() const;
    big_integer operator-() const;
    big_integer operator~() const;

    big_integer& operator++();
    big_integer operator++(int);
    
    big_integer& operator--();
    big_integer operator--(int); 

    friend bool operator==(big_integer const& a, big_integer const& b);
    friend bool operator!=(big_integer const& a, big_integer const& b);
    friend bool operator<(big_integer const& a, big_integer const& b);
    friend bool operator>(big_integer const& a, big_integer const& b);
    friend bool operator<=(big_integer const& a, big_integer const& b);
    friend bool operator>=(big_integer const& a, big_integer const& b);

    friend std::ostream& operator<<(std::ostream& s, big_integer const& a); 

    friend std::string to_string(big_integer a); 
    
	static big_integer ZERO;

	void pr();
private:	

	my_vector dig;
	int sign;
		
	void delete_zero();
	void add(big_integer const& b);
	void sub(big_integer const& b);
	void mul(int b);
	void convert();
	void back_convert();
	big_integer divide(big_integer const& rhs);
	big_integer divide_inplace(big_integer &rhs);
	int divide(int b);
	int divide2();
	int mod2();
	void mul_inplace(big_integer &y);
	void inc();
	void mul(big_integer const& rhs);
	bool comp(big_integer const& b) const; 
};

big_integer operator+(big_integer a, big_integer const& b);
big_integer operator-(big_integer a, big_integer const& b);
big_integer operator*(big_integer a, big_integer const& b);
big_integer operator/(big_integer a, big_integer const& b);
big_integer operator%(big_integer a, big_integer const& b);

big_integer operator&(big_integer a, big_integer const& b);
big_integer operator|(big_integer a, big_integer const& b);
big_integer operator^(big_integer a, big_integer const& b);

big_integer operator<<(big_integer a, int b);
big_integer operator>>(big_integer a, int b); 

bool operator==(big_integer const& a, big_integer const& b);
bool operator!=(big_integer const& a, big_integer const& b);
bool operator<(big_integer const& a, big_integer const& b);
bool operator>(big_integer const& a, big_integer const& b);
bool operator<=(big_integer const& a, big_integer const& b);
bool operator>=(big_integer const& a, big_integer const& b);  

std::string to_string(big_integer a);     
std::ostream& operator<<(std::ostream& s, big_integer const& a);   

#endif // BIG_INTEGER_H