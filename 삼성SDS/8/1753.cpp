#include <iostream>
#include <queue>
#include <vector>

using namespace std;


/*

struct e_t{
	int node;
	int cost;
	e_t(int _node, int_cost) : node(_node), cost(_cost){
	}
	bool operator < (const e_t &ref)const{
		return this->cost > ref.cost;
	}
};

*/


using Pair = pair<int,int>;
constexpr int INF = 1e9+7;
int V, E;
vector<Pair> AL[20001];


vector<int> d(20001);

int Visited[20001];

struct cmp{
	bool operator()(pair<int, int> &a, pair<int, int> &b){
		if (a.second > b.second){
			return true;
		}
		else{
			return false;
		}
	}
};


int main(void){
	cin >> V >> E;
	int start;
	cin >> start;
	
	int a,b,c;
	
	for (int i = 0 ; i < E ; i ++){
		cin >> a >> b >> c;
		AL[a].push_back({b,c});
	}	
	
	priority_queue<Pair, vector<Pair>, cmp > PQ;
	
	for (int i = 1 ; i <= V ; i ++){
		Visited[i] = INF;
	}
	Visited[start] = 0;
	PQ.push({start,0});
	
	while (!PQ.empty()){
		//cout << PQ.top().first <<" "<< PQ.top().second << '\n';
		Pair curr = PQ.top();
		PQ.pop();
		for (Pair next : AL[curr.first]){
			int cost = curr.second + next.second;
			if(cost < Visited[next.first]){
				Visited[next.first] = cost;
				PQ.push({next.first, cost});
			}
		}
	}
	for (int i = 1; i <= V ; i++){
		if (Visited[i] == INF) cout << "INF" << '\n';
		else cout << Visited[i] << '\n';
	}
}
