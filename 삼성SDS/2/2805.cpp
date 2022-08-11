#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
long long N,M;
int main(void){
	cin >> N >> M;
	
	vector<long long> trees(N);
	
	for (int i = 0 ; i < N ; i++){
		cin >> trees[i];
	}
	
	sort(trees.begin() , trees.end());
	
	long long start = 0 , end = trees[N-1];
	
	long long result = -1;
	long long sum, mid;
	for (;start <= end;){
		sum = 0;
		mid = (start + end) / 2;
		
		
		for (int i = 0; i < N ; i++){
			if (trees[i] - mid > 0){
				sum+= trees[i] - mid;
			}
		}
		
		if (sum >= M){
			start = mid + 1;
			if (mid > result){
				result = mid;
			}
		}
		else{
			end = mid - 1;
		}
	}
	
	cout << result <<endl;
}
