#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

void stamp( const char *s = "" ) {
	static double start = 0;
	fprintf(stderr, "time = %.2f : %s\n", (clock() - start) / CLOCKS_PER_SEC, s);
	start = clock();
}

const int N = 1e5, TN = 1e3, RADIX = 1 << 30;

int a[N];//, b[N];
//vector<int> a(N), b(N);

unsigned R() {
	return (rand() << 15) ^ rand();
}

void add1( int n ) {
	for (int i = 0; i < n; i++) {
		if ((a[i] += a[i]) >= RADIX) {
			a[i] -= RADIX;
			a[i + 1]++;
		}
	}
}
void add2( int n ) {
	int mod = 0;
	for (int i = 0; i < n; i++) {
		mod += a[i] + a[i];
		a[i] = mod % RADIX;
		mod /= RADIX;
	}
}

void init( int n ) {
	for (int i = 0; i < n; i++) {
		a[i] = R() & (RADIX - 1);
		//b[i] = R() & (RADIX - 1);
		if (i + 100 > n)
			a[i] = 0;//b[i] = 0;
	}
}

int main() {
	int n;
	scanf("%d", &n);
	assert(n <= N);
	stamp("start");
	init(n);
	for (int i = 0; i < TN; i++)
		add1(n);	
	stamp("add1");
	init(n);
	for (int i = 0; i < TN; i++)
		add2(n);	
	stamp("add with mod");
}
