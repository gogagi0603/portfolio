#include <iostream>
#include <algorithm>
 
using namespace std;

//크루스칼
//1. sort
//2. edge 선택 : 서로소 집합 이용
 
int n,m;

struct e_t{
	int a;
	int b;
	int c;
	bool operator<(const e_t &ref) const{
		return this->c < ref.c;
	}
} Edge[100000];

int U[1001];

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
	cin >> n >> m;
	
	for (int i = 0 ; i < m ; i++){
		cin >> Edge[i].a >> Edge[i].b >> Edge[i].c;
	}
	
	sort (Edge, Edge + m);
	
	for (int i = 1; i <= n ; i++){
		U[i] = i;
	}
	
	int result = 0;
	
	for (int i = 0 ; i < m ; i++){
		// 연결 확인 -> 안되어있으면 join
		if( grp(Edge[i].a) != grp(Edge[i].b)){
			join(Edge[i].a, Edge[i].b);
			result += Edge[i].c;
		}
	}
	
	cout << result << '\n';
}
