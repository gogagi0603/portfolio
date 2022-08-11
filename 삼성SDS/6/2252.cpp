#include <iostream>
#include <vector>
#include <queue>
#include <set>
using namespace std;
int n,m;
vector<int> topology;
vector<vector<int>> next_node;

void print(){
	cout << "topology : ";
	for (int i = 1; i <= n ; i++){
		cout << topology[i] << " ";
	}
	cout <<'\n' << "next_nodes : " << '\n' << "--------" << '\n';
	
	for (int i = 1; i <= n ; i++){
		for (int j = 0; j < next_node[i].size(); j++){
			cout << next_node[i][j] << " ";
		}	
		cout << '\n' << "---------" << '\n';
	}
	cout << '\n';
}

int main(void){
	cin >> n >> m;
	
	topology = vector<int>(n+1);
	next_node = vector<vector<int>> (n+1);
	
	for (int itr = 0 ; itr < m ; itr++){
		int left, right;
		cin >> left >> right;
		
		topology[right]++;
		next_node[left].push_back(right);
	}
	
	queue<int> q;
	vector<int> T;
	int i = 1;
	
	for (; i <= n; i++){
		if (topology[i] == 0){
			q.push(i);
		}
	}
	//print();
	////
	while(!q.empty()){
	T.push_back(q.front());
		//cout << "while start" << '\n';
		//cout << q.front() << '\n';
		for (int i = 0; i < next_node[q.front()].size() ; i++){
			if (topology[ next_node[q.front()][i] ] >= 1){
				topology[ next_node[q.front()][i] ] = topology[ next_node[q.front()][i] ] - 1;
			//	cout <<"tt : " <<topology[ next_node[q.front()][i] ] << " " << q.front() << '\n';
				if (topology[ next_node[q.front()][i] ] == 0){
					q.push(next_node[q.front()][i]);
					//cout << "push";
				}
			}
		}
		q.pop();
 	}
 	for (int i = 0 ; i < (int)T.size() ; i ++){
 		cout << T[i] << " ";
	 }
	 cout << '\n';
}
