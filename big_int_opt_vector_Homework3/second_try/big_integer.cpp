#include "big_integer.h"

#include <string>
#include <cstdio>
#include <cassert>
#include <ctime>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <algorithm>

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
	for (size_t i = 0; i < other.dig.size(); i++) {
		dig.push_back(other.dig[i]);
	}
	delete_zero();
}


big_integer::big_integer(std::vector<int> d, int s) : dig(d), sign(s) {}
 
big_integer::big_integer(int a) {
	sign = 1;
	long long aa = a;
	if ( aa < 0) {
		sign = 0;
		aa = -aa;
	}
	if (aa < RADIX) {
		dig.clear();
		dig.push_back(aa);
	} else {
		while (aa > 0) {
			dig.push_back(aa % RADIX);
			aa /= RADIX;
		}
	}
}

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

	// dig.resize(other.dig.size());
	// std::copy(other.dig.begin(), other.dig.end(), dig.begin());
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
	std::vector<int> a;
	a.resize(k);
//	dig.insert_begin(a);
	dig.insert(dig.begin(), a.begin(), a.end());
	rhs %= RADIX_LEN;
	mul(1 << rhs);
	return *this;
}

big_integer& big_integer::operator >>=(int rhs) {
	int k = std::min(rhs / RADIX_LEN, (int)dig.size());
	dig.erase(dig.begin(), dig.begin() + k);
	//dig.erase_begin(k);
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

void stamp( const char *s = "" ) {
	static double start = 0;
	fprintf(stderr, "time = %.2f : %s\n", (clock() - start) / CLOCKS_PER_SEC, s);
	start = clock();
}

#ifndef FINAL_TESTING

/**int main() {
	freopen("in", "r", stdin);
	int x;
	std::cin >> x;
	big_integer mulq[x], u(1);
	for(int i = 0; i < x; i++) {
		int y;
		std::cin >> y;
		mulq[i] = y;
		u *= mulq[i];
	}
	for(int i = 1; i < x; i++) {
		u /= mulq[i];
	}
	std::cout << u << ' ' << mulq[0] << std::endl;
//	big_integer ab(a + b);
//	std::cout << ab ; //<< " ab /  b = " << ab /  b << " ab /  a = " << ab / a;	
}

**/
int main() {
	{
		int a = 123457, b = 37;
		std::cout << a * b << std::endl;
		big_integer x(a), y(b);
		std::cout << x*y << std::endl;
		std::cout << big_integer(a * b) / big_integer(b) << std::endl;
	}
	{
		big_integer a = std::numeric_limits<int>::min();
		big_integer b = -1;
		big_integer c = a / b;
		std::cout << a << " / " << b <<  " = " << c << std::endl;
		assert(c == (a / -1));
		assert((c - std::numeric_limits<int>::max()) == 1);
 	}
	std::string x = "1", y = "1";
	// int N, M;
	// std::cin >> N >> M;
	// stamp("start");
	// for (int i = 0; i < N; i++) { x += (char)(i % 7 + '0');	}
	// for (int i = 0; i < M; i++) { y += (char)(i % 5 + '0');	}
	// stamp("gen");
	// big_integer a(x), b(y), c;
	// stamp("convert");
	// c = a * b;
	// stamp("mul");
	// assert(c / b == a);
	// stamp("div");
	/*std::string x = "1", y = "1";
	int N;
	std::cin >> N;
	stamp("start");
	for (int i = 0; i < N; i++) { 
		x += (char)(i % 7 + '0');
		y += (char)(i % 5 + '0');
		big_integer a(x), b(y), c;
		c = a * b;
		assert(c / b == a);
	}*/
}
#endif