#include <iostream>
using namespace std;
constexpr int INF = 1e9+7;

int N,M;

struct e_t{
	int a,b,c;
} E[6001];

int Visited[501];

int main(void){
	cin >> N >> M ;
	for (int i = 1; i <= N ; i++){
		Visited[i] = INF;
	}
	
	for (int i = 1; i <= M ; i++){
		cin >> E[i].b >> E[i].c;
	}
	
	Visited[1] = 0;
	bool isNegativeCycle;
	
	for (int i = 1; i <= N ; i++){
		for (int j = 1; j <= M ; j++){
			if (Visited[E[j].a] < INF && Visited[E[j].a] + E[j].c < Visited[E[j].b]){
				Visited[E[j].b] = Visited[E[j].a] + E[j].c;
				
			}
		}
	}
	
	for (int j = 1; j <= M ; j++){
		if (Visited[E[j].a] < INF && Visited[E[j].a] + E[j].c < Visited[E[j].b]){
			isNegativeCycle = true;
			break;		
		}
	}
	if (isNegativeCycle) {
		cout << "-1" << '\n';
		return -1;
	}
	else{
		for (int i = 2; i <= N ; i++){
			int result = Visited[i] == INF ? -1 : Visited[i];
			cout << result;
		}
	}
	return 0;
}
