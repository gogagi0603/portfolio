#include <iostream>
#include <queue>
#include <vector>
using namespace std;
using Pair = pair<int, int>;
constexpr int INF = 1e9+7;


struct cmp{
	bool operator()(Pair& a, Pair& b){
		return a.second > b.second;
	}
};
priority_queue<Pair, vector<Pair>, cmp> pq; 

int n,m;
int visited[1001][1001];
int value[1001];

void printv(){
	for (int i =1; i <= n; i ++){
		for (int j = 1; j <= n; j++){
			cout << visited[i][j] << " ";
		}
		cout << '\n';
	}
	cout << '\n';
}

int main(void){
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	
	cin >> n >> m;
	int a,b,c;
	int start;
	int def;
	
	for (int i = 1 ; i <= n ; i++){
		for (int j = 1; j <= n ; j ++){
			visited[i][j] = INF;
		}
	}
	
	for (int i = 1; i <=n ; i++){
		visited[i][i] = 0;
		value[i] = INF;
	}
	
	for (int i = 0 ; i < m ; i++){
		cin >> a >> b >> c;
		if (visited[a][b] > c){
			visited[a][b] = c;
		}
		//AL[a].push_back({b,c});
	}
	cin >> start >> def;
	value[start] = 0;
	
	pq.push({start,0});
	
	while (!pq.empty()){
		Pair curr = pq.top();
		pq.pop();
		int cost;
		
		for (int i = 1; i <=n ; i++){
			int V = i;
			int W = visited[curr.first][V];
			
			
			if (W == INF){
				continue;
			}
			cost = curr.second + W;
			if (cost < value[V]){
				value[V] = cost;
				pq.push({V, cost});
			}
		}
		
		
	}
	cout << value[def] << '\n';
}
