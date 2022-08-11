#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;


print(vector<int> v){
	for (vector<int>::iterator itr = v.begin() ; itr != v.end(); itr++){
		cout << *itr << " ";
	}
	cout << '\n';
}





int main(void){
	int n,m;
	
	cin >> n >> m;
	vector<int> v(n);
	vector<int> stack;
	
	for (int i = 0 ; i < n ; i++){
		cin >> v[i];
	}
	sort(v.begin(), v.end());
	
	for (int i = 0 ; i < n ; i++){
		
		if((int)stack.size() != m){
			cout << stack.size() << '\n';
			stack.push_back(v[i]);
		}
		else if((int)stack.size() == m){
			print(stack);
			cout << stack.size() << '\n';
			stack.pop_back();
		}
	}
}
