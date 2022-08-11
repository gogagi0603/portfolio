#include <iostream> 
#include <vector>

using namespace std;
int main(void){
	int M, K;
	
	double colors[50];
	
	cin >> M;
	int sum = 0;
	
	for (int i = 0 ; i < M ; i++){
		cin >> colors[i];
		sum += colors[i];
	}
	cin >> K;
	
	double a = 0, b = 0,tmp = 0;
	
	for (int i = 0 ; i < M; i++){
		tmp = 1.0;
		for (int j = 0; j < K ; j++){
			tmp *=colors[i] - j;
		}
		a += tmp;
	}
	
	b = 1.0;
	for (int i = 0 ; i < K ; i ++){
		b *= sum - i;
	}
	cout.precision(10);
	cout << (double)a/b << '\n';
}
