#include <iostream>
using namespace std;


long long tree[512 * 512 * 2];

int n,m;
int tmpN = 1;

void get_sum(int a, int b){
	a = a + tmpN - 1;
	b = b + tmpN - 1;
	int sum = 0;
	
	while (a <= b){
		//cout << a << " " << b << '\n';
		if ((a & 1) == 1){
			sum = sum + tree[a];
			//cout << tree[a] << '\n';
			
		}
		if ((b & 1) == 0){
			sum = sum + tree[b];
			//cout << tree[b] << '\n';
		}
		
		a = (a+1) >> 1;
		b = (b-1) >> 1;
	}
	cout << sum << '\n';
}

int main(void){
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	
	cin >> n >> m;
	
	
	for (; tmpN < n ; tmpN*=2);
	
	for (int i = 0; i < tmpN*2 ; i++){
		tree[i] = 0;
	}
	
	for (int i = tmpN ; i < tmpN + n ; i++){
		cin >> tree[i];
	}
	
	for (int i = tmpN - 1; i >= 1 ; i--){
		tree[i] = tree[i*2] + tree[i*2+1];
	}
	
	
	
	int a,b;
	
	for (int i = 0 ; i < m; i++){
		cin >> a >> b;
		get_sum(a,b);
		
	}
}

