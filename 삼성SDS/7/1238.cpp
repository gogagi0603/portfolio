#include <iostream>
#include <vector>
#include <queue>
using namespace std;
constexpr int INF = 1e9+7;
using pii = pair<int, int>;


int n,m,x;


vector<pii> node[2][1001];
vector<int> dist[2];
int resdist[1001];

struct cmp{
	bool operator()(pii a, pii b){
		return a.second > b.second;
	}
};

void digstra(int k){
	dist[k][x] = 0;
	
	priority_queue<pii, vector<pii>, cmp> que;
	que.push({x,0});
	
	
	while(!que.empty()){
		pii curr = que.top();
		que.pop();
		
		if (curr.second > dist[k][curr.first]){
			continue;
		}
		int cost;
		for (pii next : node[k][curr.first]){
			cost = curr.second + next.second;;
			
			if (cost < dist[k][next.first]){
				dist[k][next.first] = cost;
				que.push({next.first, cost});
			}
		}
	}
	
}

int main(void){
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	
	cin >> n >> m >> x;
	
	int a, b,c;
	for (int i = 0 ; i < m ; i++){
		cin >> a >> b >> c;
		node[0][a].push_back({b,c});
		node[1][b].push_back({a,c});
	}
	dist[0].resize(n+1, INF);
	dist[1].resize(n+1, INF);
	
	digstra(1);
	digstra(0);
	
	int res = 0;
	for (int i = 1; i <= n ;i++){
		res = max(res, dist[0][i] + dist[1][i]);
	}
	cout << res << '\n';
}
