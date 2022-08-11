#include <iostream>
#include <queue>
#include <vector>

using namespace std;

int main(void){
	ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

	int itr;
	cin >> itr;
	
	priority_queue<int> max;
	priority_queue<int, vector<int>, greater<int>> min;
	
	while(itr--){
		int a, size;
		cin >> a;
		if (max.size() == min.size()){
			max.push(a);
		}
		else{
			min.push(a);
		}
		
		if (!max.empty() && !min.empty() && max.top() > min.top()){
			int max_tmp, min_tmp;
			
			max_tmp = max.top();
			min_tmp = min.top();
			
			max.pop();
			min.pop();
			max.push(min_tmp);
			min.push(max_tmp);
		}
		cout << max.top() << '\n';
		//itr = itr-1;
	}
}
