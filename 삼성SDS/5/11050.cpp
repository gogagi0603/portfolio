#include <iostream>

using namespace std;

int factorial (int n){
	int result = 1;
	
	for (int i = n ; i >= 1 ; i--){
		result *= i;
	}
	return result;
}

int main(void){
	int N, K;
	
	cin >> N >> K;
	
	
	int result = 1;
	int tK = K, tN = N;
	int son=  1, mom = 1;
	
	son = factorial(N);
	mom = factorial(K);
	mom = mom * factorial(N-K);
	
	
	result = son / mom;
	cout << result << '\n';
	
}
