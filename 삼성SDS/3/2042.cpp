#include <iostream>
long long tree [1024*1024*2];
int tmpN = 0;
int N,M,K;

using namespace std;

void update(int a, long long b){
	//          3            6
	// a번째 값 > tmpN;
	
	a = a+tmpN-1;
	//3+8-1 = 10
	tree[a] = b;
	
	
	while(a > 1) {
		a = a >> 1; // /2
		tree[a] =  tree[a*2] + tree[a*2+1];
	}
}

long long get_sum(int a, int b){ 
	//                2      5
	a = a + tmpN - 1;
	// a = 9
	b = b + tmpN - 1;
	// b = 12
	
	
	long long sum  = 0 ;
	
	
	//sum <- 7, 11
	
	while (a<=b){
		//5,5
		
		//a가 right child 라면 --> a가 홀수라면? : a 
		
		if( (a & 1) == 1 ){
			sum += tree[a];
			//cout << " a : " << sum << '\n';
		}
		//b가 left child 라면 
		if ((b & 1) == 0){
			sum += tree[b];
			//cout << " b : " << sum << '\n'; 
		}
		a = (a + 1) >> 1;
		b = (b - 1) >> 1;
	}
	return sum;
} 
 



int main(void){
	cin >> N >> M >> K;
	//     5    2     2
	for (tmpN=1 ; tmpN< N; tmpN*=2);
	// tmpN == 8
	for (int i = 0 ; i < tmpN *2; i++){
		tree[i] = 0;
	}
	
	
	for (int i = tmpN ; i < tmpN+N ; i++){
		cin >> tree[i];
	}
	for (int i = tmpN - 1; i >=1 ; i--){
		tree[i] = tree[2*i] + tree[2*i + 1];
	}
	
	//cout << "djflaj";
	
	long long a,b,c;
	         
	         
	
	for (int q = 0 ; q < M + K ;  q++){
		cin >> a >> b >> c;
		if (a == 1){
			//update
			update(b,c);
		}
		else{
			//구간합
			cout << get_sum(b,c) << '\n';
		}
	}
	
	
}
