#include <iostream>
#include <cmath>
using namespace std;
using Pair = pair<int,int>;

int n;
int map[15];
const int INF = 1e9+7; 
//인덱스 = y , 매개변수 = x 
int index = 1;
int count = 0;

void print(){
	for (int i = 1; i <= n ; i++){
		cout << map[i] << " ";
	}
	cout << '\n';
}
bool check(int x, int y){
	
	for (int i = 1; i <= n ; i++){
		if (abs(map[i] - x) == abs(i - y)){
			return false;
		} 
	}
	
	return true;
}

void start(int x, int size){
	
	if (x>n){
		return;
	}
	
	
	for (int i =1 ; i <= n ; i++){
		// 퀸을 둘 수 있을때
		// 퀸을 두고 다음 함수. y는 더 이상 할 필요 없고 x를 증가 
		if (check(x ,i) == true && map[i] == INF){
			
			map[i] = x;
			size ++;
			if (size ==n){
				count ++;
			}
			start(x+1, size);
			
			
			map[i] = INF;
			size--;
			
		}
		// 퀸을 둘 수 없을때
		// 백트래킹  
	}
}

int main(void){
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	cin >> n;
	for (int i = 1 ; i <= n ; i++){
		map[i] = INF;
	}
	
	start(1, 0);
	cout << count << '\n';
}
