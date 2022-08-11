#include <iostream>
// 플로이드는 인접행렬이 유리 
using namespace std;
int N, M;
int AM[101][101];
constexpr int INF = 1e9+7;
int main(void){
	cin >> N;
	cin >> M;
	
	for (int i = 1 ; i <= N ; i++){
		for (int j = 1;j <= N ; j++){
			if (i!=j){
				AM[i][j] = INF;
			}
		}
	}
	
	int a,b,c;
	for (int i = 1; i <= M ; i++){
		cin >> a >> b >> c;
		AM[a][b] = c;
		AM[a][b] = c < AM[a][b] ? c:AM[a][b];
	}
	for (int k = 1 ; k <= N ; k++){ //경유지 
		for (int i = 1; i <= N ; i++){ //츌발지 
			for (int j = 1; j <= N ; j++){ // 도착지 
				if (AM[i][j] > AM[i][k] + AM[k][j]){
					AM[i][j] = AM[i][k] + AM[k][j];
				} 
			}
		}
	}
	
	for (int i = 1 ; i<=N; i++){
		for (int j = 1; j <= N ; j++){
			cout << AM[i][j] << " ";
		}
		cout << '\n';
	}
	return 0;
}
