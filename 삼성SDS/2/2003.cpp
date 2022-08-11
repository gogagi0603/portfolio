#include <iostream>
#include <vector>

int N,M;
using namespace std;

bool isOver(int a){
	return a>M ? true : false;
}

int main(void){
	cin >> N >> M;
	
	vector<int> A(N);
	
	for (int i = 0; i < N ; i++){
		cin >> A[i];
	}
	
	int count = 0;
	
	for (int i = 0 ; i < N; i ++){
		
		int sum = A[i];
		
		if (sum == M){
			count++;
			continue;
		}
		
		for (int j = i+1 ; j < N ; j++){
			sum = sum + A[j];
			
			if (isOver(sum)){
				break;
			}
			if (sum == M){
				count++;
				break;
			}
		}
	}
	cout << count << endl;
}
