#include <iostream>
#include <vector>
using namespace std;

int n;
vector<int> A;
vector<int> op;


// 0 : µ¡¼À
// 1 : »¬¼À
// 2 : °ö¼À
// 3 : ³ª´°¼À

void print(vector<int> v){
	for (int i = 0 ; i < (int)v.size(); i++){
		cout << v[i] << " ";
	}
	cout << '\n';
}
int max_result = -1000000000;
int min_result = 1000000000;

int max(int a, int b){
	return a > b ? a : b;
}

int min (int a, int b){
	return a > b ? b : a;
}

void start(int index, vector<int> v, vector<bool> visited){
	
	if (visited[index] == false){
		visited[index] = true;
		v.push_back(op[index]);
	}
	
	if ((int)v.size() >= n-1){
		int result = A[0];
		for (int i = 0 ; i < (int)v.size(); i++){
			
			if (v[i] == 0){
				//cout << "sum " << result << " " << A[i+1] << '\n';
				result = result +A[i+1];
			}
			else if (v[i] == 1){
				//cout << "sub " << result << " " << A[i+1] << '\n';
				result = result -A[i+1];
			}
			else if (v[i] == 2){
				//cout << "mul " << result << " " << A[i+1] << '\n';
				result = result *A[i+1];
			}
			else if (v[i]== 3){
				//cout << "div " << result << " " << A[i+1] << '\n';
				result = result /A[i+1];
			}
		}
		//cout << "result :: " << result << '\n';
		max_result = max(result, max_result);
		min_result = min(result, min_result);
		return;
	}
		
	
	for (int i = 0 ; i < n-1 ; i++){
		if (visited[i] == false){
			
			start(i, v, visited);
		}
	}
	visited[index] = false;
}

int main(void){
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	
	cin >> n;
	A = vector<int> (n);

	for (int i = 0 ; i < n ; i++){
		cin >> A[i];
	}
	
	for (int i = 0 ; i < 4; i ++){
		int count;
		cin >> count;
		for (int j = 0 ; j < count ; j++){
			op.push_back(i);
		}
	}
	vector<bool> visited(n-1, false);
	for (int i = 0; i < n-1; i++){
		start(i, vector<int>(), visited);
	}
	cout << max_result << '\n' << min_result << '\n';
}
