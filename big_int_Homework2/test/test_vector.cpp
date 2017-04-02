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

int main() {
	int n = 1e6;
	vector<int> v, zero(1, 0);
	for (int i = 0; i < n; i++) {
		v.resize(i + 1);
	}
	stamp("resize");
	for (int i = 0; i < n; i++) {
		if (v == zero) {
			puts("ok");
		}
		zero[0]++;
	}
	stamp("== zero");
}
