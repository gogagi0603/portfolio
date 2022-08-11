#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<long long> A, B, C, D;

int n;

bool compare(long long a, long long b){
	return a > b;
}
int main(void){
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	cin >> n;
	B = C = D = A = vector<long long> (n + 1);
	
	for (int i = 1; i <= n ; i++){
		cin >> A[i] >> B[i] >> C[i] >> D[i];
	}
	
	vector<long long> AB(1,1e9+7);
	vector<long long> CD(1,1e9+7);
	
	for (int i = 1; i <= n ; i++){
		for (int j = 1; j <= n ; j++){
			AB.push_back(A[i] + B[j]);
			CD.push_back(C[i] + D[j]);
		}
	}
	
	sort(AB.begin()+1, AB.end());
	sort(CD.begin()+1, CD.end(), compare);
	long long count = 0;
	int i = 1, j = 1;
	

	while (i < (int)AB.size() && j < (int)CD.size()){
		long long sum = AB[i] + CD[j];
		if (sum == 0){
			count ++;
			i++;
		}
		else if (sum < 0){
			i++;
		}
		else if (sum > 0){
			j++;
		}
		if (i >= (int)AB.size()){
			i = (int)AB.size()-1;
			j++;
			
		}
		if (j >= (int)AB.size()){
			i++;
			j = (int)CD.size()-1;
		}
	}
	
	cout << count << '\n';
	
	
}
