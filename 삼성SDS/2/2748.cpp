#include <iostream>
#include <vector>

using namespace std;

void print(vector<long long> vec){
	for (int i = 0; i < (int)vec.size()-1; i++){
		cout << vec[i] << " ";
	}
	cout << endl;
}

int main(void){
	vector<long long> fibonacci = {0,1};
	int n;
	
	cin >> n;
	
	for (int i = 2; i <= n ; i++){
		fibonacci.push_back(fibonacci[i-1] + fibonacci[i-2]);
		//print(fibonacci);
	}
	cout << fibonacci[n];
}
