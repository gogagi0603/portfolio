#include <iostream>
#include <cmath>
using namespace std;
using Pair = pair<int,int>;

int n;
int map[15];
const int INF = 1e9+7; 
//�ε��� = y , �Ű����� = x 
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
		// ���� �� �� ������
		// ���� �ΰ� ���� �Լ�. y�� �� �̻� �� �ʿ� ���� x�� ���� 
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
		// ���� �� �� ������
		// ��Ʈ��ŷ  
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
