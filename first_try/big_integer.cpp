#include "big_integer.h"

#include <string>
#include <cstdio>
#include <cassert>
#include <ctime>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <memory>


const int RADIX_LEN = 30;
const int RADIX = 1<<RADIX_LEN;

big_integer big_integer::ZERO(0);

void big_integer::delete_zero() {
	while(dig.size() > 1U && dig.back() == 0) {
		dig.pop_back();	
	}
	if (dig.size() == 1 && dig[0] == 0) {
		sign = 1;	
	}
	assert(dig.size());
}

// + for positive
void big_integer::add(big_integer const& b){
	size_t size = std::max(dig.size(), b.dig.size()) + 1;
	dig.resize(size, 0);
	int mod = 0;
	for (size_t i = 0; i < size; i++) {
		mod += dig[i] + ((i < b.dig.size()) ? b.dig[i] : 0);
		dig[i] = mod % RADIX;
		mod /= RADIX;
	}
	delete_zero();
}
// + for positive
void big_integer::inc(){
	dig.push_back(0);
	dig[0]++;
	for (size_t i = 0; dig[i] == RADIX; i++) {
		dig[i] = 0, dig[i + 1]++;
	}
	delete_zero();
}

// a > b and a && b positive
void big_integer::sub(big_integer const& b) {
	int carry = 0;
	int cnt = 0ll;
	for (size_t i = 0; i < dig.size(); i++) {
		cnt = dig[i] - ((i < b.dig.size()) ? b.dig[i] : 0) - carry;
		carry = 0;
		if (cnt < 0) {
			cnt += RADIX;
			carry = 1;
		}
		dig[i] = cnt;	
	}
	delete_zero();
}
	

void big_integer::mul(int b) {
	if (b < 0) {
		b = -b;
		sign ^= 0;				
	}
	long long carry = 0ll;
	for (size_t i = 0; i < dig.size(); i++) {
		carry += (long long)dig[i]*b;
		dig[i] = int(carry % RADIX);
		carry /= RADIX;
	}
	while (carry > 0) {
		dig.push_back(int(carry % RADIX));
		carry /= RADIX;	
	}
	delete_zero();
}

int big_integer::divide(int b) {
	long long carry = 0ll;
	for (int i = (int)dig.size() - 1; i >= 0; i--) {
		carry = carry * RADIX + dig[i];
		dig[i] = int(carry / b);
		carry %= b;	
	}
	delete_zero();
	return carry;	
}

int big_integer::mod2() {
	return dig[0] & 1;
}

int big_integer::divide2() {
	for (int i = (int)dig.size() - 1; i > 0; i--) {
		if (dig[i] & 1) dig[i - 1] += RADIX;
		dig[i] >>= 1;
	}
	int carry = dig[0] & 1;
	dig[0] >>= 1;
	delete_zero();
	return carry;	
}

// < for positive
bool big_integer::comp(big_integer const& b) const { 
	if (dig.size() != b.dig.size()) {
		return dig.size() < b.dig.size();
	}
	int i = (int)dig.size() - 1;
	while (dig[i] == b.dig[i] && i > 0) {
		i--;	
	}
	return dig[i] < b.dig[i];
}

void big_integer::convert() {
	if (sign == 0) {
		sub(1);
		for (size_t i = 0; i < dig.size(); i++) {
			dig[i] = ~dig[i];		
		}
	}
}

void big_integer::back_convert() {
	if (dig.back() < 0) {
		sign = 0;     
		for (size_t i = 0; i < dig.size(); i++) {
			dig[i] = (~dig[i]) & (RADIX - 1);		
		}
		add(1);
	} else {
		sign = 1;	
	}
}

void big_integer::mul_inplace(big_integer &y) {
	big_integer result;
	while (y != ZERO) {
		if (y.mod2() == 1)
			result += *this;
		y.divide2();
		add(*this);
	}
	*this = result;
}     

void big_integer::mul(big_integer const& rhs) {
	big_integer y(rhs);
	if (y.dig.size() == 1) {
		mul(y.dig[0]);	
	} else {
		mul_inplace(y);
	}
}

big_integer big_integer::divide(big_integer const& rhs) {
	if (rhs == ZERO) {
		throw std::runtime_error("Division by zero");	
	}
	if (rhs.dig.size() == 1) {
		return divide(rhs.dig[0]);	
	}
	big_integer k, q;
	k.dig.resize(dig.size());
	int j = RADIX_LEN*dig.size() - 1;
	for (int w = dig.size() - 1; w >= 0; w--) {
		for (int i = RADIX_LEN - 1; i >= 0; i--, j--) {
			q.add(q);
			if ((dig[w] >> i)&1) {
				q.inc();
			}
			if (q >= rhs) {
				k.dig[j / RADIX_LEN] |= 1<<(j % RADIX_LEN);
				q.sub(rhs);			
			}
		}
	}
	*this = k;
	return q;
}



big_integer::big_integer(big_integer const& other) {
	sign = other.sign;
	dig = other.dig;
}


big_integer::big_integer(int a) {
	sign = 1;
	long long aa = a;
	if ( aa < 0) {
		sign = 0;
		aa = -aa;
	}
	if (aa < RADIX) {
		dig.resize(1);
		dig[0] = aa;
	} else {
		while (aa > 0) {
			dig.push_back(aa % RADIX);
			aa /= RADIX;
		}
	}
}

big_integer::big_integer(my_vector d, int s):dig(d), sign(s) {}


big_integer::big_integer(std::string const& str) {
	int s = 1;
	if (str[0] == '-') {
		s = 0;	
	}
	dig.resize(1);
	for (int i = 1 - s; i < (int)str.length(); i++) {
		if (str[i] < '0' || str[i] > '9') {
			 throw std::runtime_error("invalid string");	
		}
		mul(10);
		dig[0] += str[i] - '0';
	}
	*this += 0;
	sign = s;
	delete_zero();
}

big_integer& big_integer::operator=(big_integer const& other) {
	sign = other.sign;
	dig = other.dig;
	return *this;		
}


big_integer& big_integer::operator+=(big_integer const& rhs) {
	if (sign == rhs.sign) {
		add(rhs);
		return *this;	
	}
	if (comp(rhs)) {
		big_integer me = *this;      
		*this = rhs;
		sub(me);
		return *this;
	}
	sub(rhs);
	return *this;
}

big_integer& big_integer::operator-=(big_integer const& rhs) {
	return *this += -rhs;	
}

big_integer& big_integer::operator*=(big_integer const& rhs) {
    int s = (sign == rhs.sign);
    big_integer x(rhs);
    sign = 1;
    x.sign = 1;
    mul(x);
    sign = s;
	return *this;	
}

big_integer& big_integer::operator/=(big_integer const& rhs) {
	int s = (sign == rhs.sign);
	big_integer x(rhs);
    sign = 1;
    x.sign = 1;
    divide(x);                  
    sign = s;
	return *this;	
}

big_integer& big_integer::operator%=(big_integer const& rhs) {
	int s = sign;
	big_integer x(rhs);
    sign = 1;
    x.sign = 1;
    *this = divide(x);
    sign = s;
	return *this;	
}



big_integer& big_integer::operator&=(big_integer const& rhs) {
	convert();
	big_integer x(rhs);
	x.convert();
	for (size_t i = 0; i < std::max(x.dig.size(), dig.size()); i++) {
		int a = (i < dig.size()) ? dig[i] : (sign == 1) ? 0 : -1;
		int b = (i < x.dig.size()) ? x.dig[i] : (sign == 1) ? 0 : -1;
		if (i < dig.size()) {
			dig[i] = a & b;
		} else {
			dig.push_back(a & b);	
		}
	}
	back_convert();
	return *this;	
}

big_integer& big_integer::operator|=(big_integer const& rhs){
	convert();
	big_integer x(rhs);
	x.convert();
	for (size_t i = 0; i < std::max(x.dig.size(), dig.size()); i++) {
		int a = (i < dig.size()) ? dig[i] : (sign == 1) ? 0 : -1;
		int b = (i < x.dig.size()) ? x.dig[i] : (sign == 1) ? 0 : -1;
		if (i < dig.size()) {
			dig[i] = a | b;
		} else {
			dig.push_back(a | b);	
		}
	}
	back_convert();
	return *this;		
}

big_integer& big_integer::operator^=(big_integer const& rhs) {
	convert();
	big_integer x(rhs);
	x.convert();
	for (size_t i = 0; i < std::max(x.dig.size(), dig.size()); i++) {
		int a = (i < dig.size()) ? dig[i] : (sign == 1) ? 0 : -1;
		int b = (i < x.dig.size()) ? x.dig[i] : (sign == 1) ? 0 : -1;
		if (i < dig.size()) {
			dig[i] = a ^ b;
		} else {
			dig.push_back(a ^ b);	
		}
	}
	back_convert();
	return *this;			
}
     
    
  


big_integer& big_integer::operator<<=(int rhs) {
	int k  = rhs / RADIX_LEN;
	my_vector a;
	a.resize(k);
	dig.insert_to_begin(a.begin(), k);
	rhs %= RADIX_LEN;
	mul(1 << rhs);
	return *this;
}

big_integer& big_integer::operator >>=(int rhs) {
	int k = std::min(rhs / RADIX_LEN, (int)dig.size());
	dig.erase_pref(k);
	if (!dig.size()) {
		dig.resize(1);
	}
	rhs %= RADIX_LEN;
	int o = dig[0] & ((1 << rhs)-1); 
	if (rhs) {
		divide(1 << rhs);
	}
	if (o && sign == 0) {
		inc();
	}	 
	return *this;
}

big_integer big_integer::operator+() const{
    return *this;
}

big_integer big_integer::operator-() const{
	if (*this == ZERO) {
		return big_integer(ZERO);
	}
    return big_integer(dig, sign ^ 1);
}

big_integer big_integer::operator~() const{
	big_integer x(*this);
	x++;
	x.sign ^= 1;
	return x;		
}





big_integer& big_integer::operator++() {
	return *this += 1;	
}
big_integer big_integer::operator++(int) {
	big_integer x = *this;
	++*this;
	return x;	
}

big_integer& big_integer::operator--() {
	return *this -= 1;	
}
big_integer big_integer::operator--(int) {
	big_integer x = *this;
	--*this;
	return x;	
}
    

bool operator==(big_integer const& a, big_integer const& b) {
	return (a.dig == b.dig) && (a.sign == b.sign);
}

bool operator!=(big_integer const& a, big_integer const& b) {
	return (a.dig != b.dig) || (a.sign != b.sign);	
	
}

bool operator<(big_integer const& a, big_integer const& b) {
	if (a.sign != b.sign) {
		return a.sign < b.sign;	
	}
	if (a.sign) {	
		return a.comp(b);
	} else {
		return b.comp(a);		
	}
}

bool operator>(big_integer const& a, big_integer const& b) {
	return (b < a);
} 

bool operator<=(big_integer const& a, big_integer const& b) {
	return !(b < a);
}

bool operator>=(big_integer const& a, big_integer const& b) {
	return !(a < b);
}

big_integer operator+(big_integer a, big_integer const& b) {
	return a += b;	
} 

big_integer operator-(big_integer a, big_integer const& b) {
    return a -= b;
}

big_integer operator*(big_integer a, big_integer const& b)
{
    return a *= b;
}

big_integer operator/(big_integer a, big_integer const& b)
{
    return a /= b;
}

big_integer operator%(big_integer a, big_integer const& b)
{
    return a %= b;
}



big_integer operator&(big_integer a, big_integer const& b)
{
    return a &= b;
}

big_integer operator|(big_integer a, big_integer const& b)
{
    return a |= b;
}

big_integer operator^(big_integer a, big_integer const& b)
{
    return a ^= b;
}

big_integer operator<<(big_integer a, int b)
{
    return a <<= b;
}

big_integer operator>>(big_integer a, int b)
{
    return a >>= b;
} 




std::string to_string(big_integer a) {
	std::string s = "", h = "";
	if (a == big_integer::ZERO) {
		return "0";	
	}
	if (!a.sign) {
		h = "-";	
	}
	while (a != big_integer::ZERO) {
		int x = a.divide(10);
		char c = x + '0';
		s = c + s;
	}
	return h + s;
} 

std::ostream& operator<<(std::ostream& s, big_integer const& a) {
	return s << to_string(a);
}            

size_t const& big_integer::my_vector::size() const{
	return my_size;
}

int& big_integer::my_vector::back() {
	return head.get()[my_size - 1];	
}

const std::shared_ptr<int> big_integer::my_vector::begin() const{
	return head;	
}


std::shared_ptr<int> big_integer::my_vector::begin() {
	if (cow) {
		std::shared_ptr<int> other (new int[my_size * 2]);
		copy(head, other, my_size);
		all_size = my_size * 2;
		head = other;
		cow = false;
	}
	return head;	
}

std::shared_ptr<int> big_integer::my_vector::erase_pref(int k) {
	if (my_size - k == 1) {
		x = head.get()[my_size - 1];
		head = std::shared_ptr<int>(&x);
		my_size = 1;
		all_size = 1;
		cow = false;
		return head;	
	}
	
	std::shared_ptr<int> other (new int[(my_size - k) * 2]);
	copy(std::shared_ptr<int>(head.get() + k), other, my_size - k);
	all_size = (my_size - k) * 2;
	head = other;
	cow = false;
	return head;
}

std::shared_ptr<int> big_integer::my_vector::insert_to_begin(std::shared_ptr<int> start, int s) {
	std::shared_ptr<int> other (new int[(my_size + s) * 2]);
	copy(start, other, s);
	copy(head, std::shared_ptr<int> (other.get() + s), my_size);
	all_size = (my_size + s) * 2;
	head = other;
	cow = false;
	return head;	
}


void big_integer::my_vector::pop_back() {
	if (cow) {
		std::shared_ptr<int> other (new int[my_size * 2]);
		copy(head, other, my_size);
		all_size = my_size * 2;
		head = other;
		cow = false;
	}
   	if (my_size == 1) {
		x = 0;
		return;
	}
	my_size--;
	if (my_size == 1) {
		x = head.get()[0];
		head = std::shared_ptr<int>(&x);
		my_size = 1;
		all_size = 1;
	}
}

void big_integer::my_vector::copy(std::shared_ptr<int> a, std::shared_ptr<int> b, size_t s) {
	for (int i = 0; i < (int)s; i++) {
		b.get()[i] = a.get()[i];
	}
}

void big_integer::my_vector::resize(size_t s, int v) {
	if (s <= my_size) {
		my_size = s;
		if (s == 1) {
			x = head.get()[0];
			head = std::shared_ptr<int>(&x);
			my_size = 1;
			all_size = 1;
		}
		return;
	}
	if (all_size >= s && !cow) {
		for (int i = my_size; i < (int)s; i++) {
			head.get()[i] = v;	
		}
		my_size = s;
		return;				
	}
	std::shared_ptr<int> other (new int[s * 2]);
	copy(head, other, my_size);
	for (int i = my_size; i < (int)s; i++) {
		other.get()[i] = v;	
	}
	my_size = s;
	all_size = my_size * 2;
	head = other;
	cow = false;
}

void big_integer::my_vector::push_back(int a) {
	if (all_size != my_size && !cow) {
		head.get()[my_size] = a;
		my_size++;
		return;
	}
	std::shared_ptr<int> other(new int[all_size * 2]);
	copy(head, other, my_size);
	other.get()[my_size] = a;
	all_size = my_size * 2;
	my_size++;
	head = other;
	cow = false;
}

big_integer::my_vector::my_vector() {
	cow = false;
	all_size = 1;
	my_size = 1;
	head = std::shared_ptr<int>(&x);
//	head.get()[0] = 0;
	x = 0; 		
}


big_integer::my_vector::my_vector(my_vector const& other) {
	cow = true;
	all_size = other.all_size;
	my_size = other.my_size;
	head = other.head;
	x = 0;	
}
big_integer::my_vector& big_integer::my_vector::operator=(my_vector const& other) {
	cow = true;
	all_size = other.all_size;
	my_size = other.my_size;
	head = other.head;
	x = 0;	
	return *this;		
}

bool operator==(big_integer::my_vector const& a, big_integer::my_vector const& b) {
	if (a.size() == b.size()) {
		for (int i = 0; i < (int)a.size(); i++) {
			if (a[i] != b[i])
				return false;
		}
		return true;
	}
	return false;
}

bool operator!=(big_integer::my_vector const& a, big_integer::my_vector const& b) {
	if (a.size() == b.size()) {
		for (int i = 0; i < (int)a.size(); i++) {
			if (a[i] != b[i])
				return true;
		}
		return false;
	}
	return true;
}


int const& big_integer::my_vector::operator[](int ind) const{
	return head.get()[ind];	
}   



int& big_integer::my_vector::operator[](int ind) {
	if (cow) {
		std::shared_ptr<int> other (new int[my_size * 2]);
		copy(head, other, my_size);
		all_size = my_size * 2;
		head = other;
		cow = false;
	}
	return head.get()[ind];
}



void stamp( const char *s = "" ) {
	static double start = 0;
	fprintf(stderr, "time = %.2f : %s\n", (clock() - start) / CLOCKS_PER_SEC, s);
	start = clock();
}

int main() {
	int a = 123457, b = 37;
	std::cout << a * b << std::endl;
	std::cout << big_integer(a) * big_integer(b) << std::endl;
	std::cout << big_integer(a * b) / big_integer(b) << std::endl;
/*	std::string x = "1", y = "1";
	int N, M;
	std::cin >> N >> M;
	stamp("start");
	for (int i = 0; i < N; i++) { x += (char)(i % 7 + '0');	}
	for (int i = 0; i < M; i++) { y += (char)(i % 5 + '0');	}
	stamp("gen");
	big_integer a(x), b(y), c;
	stamp("convert");
	c = a * b;
	stamp("mul");
	assert(c / b == a);
	stamp("div");

	std::string x = "1", y = "1";
	int N;
	std::cin >> N;
	stamp("start");
	for (int i = 0; i < N; i++) { 
		x += (char)(i % 7 + '0');
		y += (char)(i % 5 + '0');
		big_integer a(x), b(y), c;
		c = a * b;
		assert(c / b == a);
	}  */
}
