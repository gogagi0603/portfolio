#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

int main(void){
	int n,k;
	
	cin >> n >> k;
	
	vector<int> integer(n-1, -1);
	vector<bool> isPrime(n-1,true);
	
	for (int i = 2; i <= n; i ++){
		integer[i-2] = i; 
	}
	/*
	for (int i = 0; i <= n - 2;i ++){
		cout << integer[i] << " ";
	}
	*/
	int count = 0;
	for (int i = 2; i <= n; i++){
		int mul = i;		
		for (int j = i ; j <= n; j+=mul ){
			
			if (isPrime[j-2] == true){
				count++;
				isPrime[j-2] = false;
				if (count == k){
					cout << j << '\n';
				}
			}
			
		}
	}
	
}
