#include <iostream>
#include <vector>
#include <algorithm> 
using namespace std;

int n;
//first == 속도, second == 들어온 순서 
using Pair = pair<long long, int>;
vector<Pair> players;
int tree[524288*2];

void print(){
	for (int i = 1; i <=n; i++){
		
		cout << "{ " << players[i].first << " " << players[i].second << " }";
	}
	cout << '\n';
}

bool compare(Pair a, Pair b){
	return a.first < b.first;
}

bool compare_second(Pair a, Pair b){
	return a.second < b.second;
}

int get_sum(int a, int b){
	int result = 0;
	
	while (a <= b){
		
		if (a & 1 == 1){
			result += tree[a];
		}
		if (b & 1 == 0){
			result += tree[b];
		}
		a = (a+1)/2;
		b = (b-1)/2;	
	}
	return result;
}

int main(void){
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	
	cin >> n;
	players = vector<Pair>(n+1,{0,0});
	int maxL = 1;
	for (; maxL<n ; maxL*=2);
	
	for (int i = 1 ; i <= n ; i++){
		int speed;
		cin >> speed;
		players[i].first = speed;
		players[i].second = i;	
	} 

	sort(players.begin()+1, players.begin()+n+1, compare);
	for (int i = 1; i<=n; i++){
		players[i].first = i;
	}

	sort(players.begin()+1, players.begin()+n+1, compare_second);
	for (int i = 1; i <= n ; i++){
		int target = players[i].first + maxL - 1; 
		tree[target] +=1;
		// 업데이트
		for (int j = target/2 ; j >= 1; j/=2){
			tree[j] = tree[j*2] + tree[j*2+1];
		}
	// 여기부터 끝까지 구간합 출력
		cout << get_sum(target,maxL*2-1) << '\n';
	}
	
}
