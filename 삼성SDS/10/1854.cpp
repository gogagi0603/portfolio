#include <iostream>
#include <queue>
#include <vector>

using namespace std;

struct n_t{
	int node;
	int cost;
	bool operator<(const n_t &ref)const{
		return this->cost > ref.cost;
	}
};

int N,M,K;

vector<n_t> AL[1001];


int main(void){
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	
	cin >> N >> M >> K;
	
	int a,b,c;
	
	for (int i = 1; i<= M ; i++){
		cin >> a>>b >> c;
		AL[a].push_back({b,c});
		
	}
	
}
