#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <iosfwd>
#include "optimized_vector.h"
//#include <vector>
//typedef std::vector<int> optimized_vector;

struct big_integer
{
    big_integer(big_integer const& other);
    big_integer(optimized_vector d, int s);
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
    optimized_vector dig;
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