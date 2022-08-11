#include <iostream>
#include <queue>

using namespace std;

int n;

struct cmp{
	bool operator()(int& a, int& b){
		return a > b;
	}
};

int main(void){
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	
	cin >> n;
	
	priority_queue <int,vector<int>, cmp> q;
	for (int i = 0 ; i < n ; i++){
		int input;
		cin >> input;
		
		if (input == 0){
			if (q.empty()){
				cout << 0 << '\n';
				continue;
			}
			cout << q.top() << '\n';
			q.pop();	
		}
		else{
			q.push(input);
		}
	}
}
