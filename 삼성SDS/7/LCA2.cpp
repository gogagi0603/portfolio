#include <iostream>
#include <vector>
#include <deque>
int N,M;
int a,b;

vector<int> AL[100001];
int Depth[100001];

int LCA(int a, int b){
	// 1: Depth 맞추기
	// 항상 a의 depth가 크도록함
	
	if (Depth[a] < Depth[b]){
		int tmp = a;
		a = b;
		b = tmp;
	} 
	int diff = Depth[a] - Depth[b];
	for (int r=  0 ; diff; r++){
		if (diff & 1){
			a = Parent[r][a];
		}
		diff >>= 1;
	}
	
	//LCA 찾기 
	while (a != b){
		int r;
		for (r=0; r < 18 ; r++){
			if (Parent[r][a] == Parent[r][b]){
				break;
			}
		}
		if (r>0) --r;
		a = Parent[r][a] , b = Parent[r][b];
	}
	return a;
}

int main(void){
	cin >> N;;
	
	for (int i = 1 ; i < N ; i++){
		cin >> a >> b;
		AL[a].push_back(b);
		AL[b].push_back(a);
	}
	// BFS를 이용하여 Depth 구함. 탐색의 시작은 Root 
	deque<int> Q;
	
	for (int i = 1 ; i <= N; i++){
		Depth[i] = -1;
	}
	
	Q.push_back(1);
	Depth[1] = 0;
	
	while(!Q.empty()){
		int curr = Q.front();
		Q.pop_front();
		for(int next : AL[curr]){
			if (Depth[next] == -1){ // 미방문시 탐 
				Q.push_back(next);
				Depth[next] = Depth[curr] + 1;
				Parent[0][next] = curr;
			}
		}
	}
	
	//점핑 테이블 희소 테이블 만들기 
	for (int r = 1 ; r < 18; r++){
		for (int i = 1; i <= N ; i++){
			Parent[r][i] = Parent[r-1][Parent[r-1][i]]; // Parent[r][i] 는 Parent[r-1][i]의  2^(r-1)의 부모 
		}
	} 
	
	cin >> M;
	for (int i = 0 ; i < M ; i++){
		cin >> a >> b;
	}
}

