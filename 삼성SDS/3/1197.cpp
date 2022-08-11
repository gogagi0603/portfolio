#include <iostream>
#include <vector>
#include <queue>

using namespace std;
using Pair = pair<int, int>;
using pPair = pair<Pair, int>;

int v,e;
int U[100001];
struct cmp{
	bool operator()(pPair a, pPair b){
		return a.second > b.second;
	}
};

priority_queue<pPair, vector<pPair>, cmp> q;

int grp(int a){
	if (U[a] == a){
		return a;
	}
	int parent = grp(U[a]);
	U[a] = parent;
	return parent;
}


void join(int a, int b){
	int aRoot = grp(a);
	int bRoot = grp(b);
	
	U[bRoot] = aRoot;
}


int main(void){
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	
	cin >> v >> e;
	int a,b,c;
	for (int i = 0 ; i < e; i ++){
		cin >> a >> b >> c;
		q.push({{a,b},c});
	}
	
	for (int i =1 ; i <= v ; i++){
		U[i] = i;
	}
	int result = 0;
	while (!q.empty()){
		pPair line = q.top();
		q.pop();
		if (grp(line.first.first) != grp(line.first.second)){
			join(line.first.first, line.first.second);
			result += line.second;
		}
	}
	cout << result << '\n';
}
