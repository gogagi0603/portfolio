#include <iostream>
#include <vector>

using namespace std;
int n, m;

int matrix[1025][1025];;

long long dp [1025][1025];

int main(void){
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);	
	cin >> n >> m;
	
	for (int i = 1; i <= n ; i ++){
		for (int j = 1; j <=n ; j++){
			int input;
			cin >> input;
			dp[i][j] = dp[i-1][j] + dp[i][j-1] - dp[i-1][j-1] + input;	
		}
	}
	
	
	for (int i = 0 ; i < m ; i++){
		int x1,y1,x2,y2;
		cin >> x1 >> y1 >> x2 >> y2;
		//cout << dp[x2][y2] <<" " <<  dp[x1-1][y1-1] << " "<<  dp[x2][y1-1] <<" " <<dp[x1-1][y2] << '\n';
		long long result = dp[x2][y2] + dp[x1-1][y1-1] - dp[x2][y1-1] - dp[x1-1][y2];
		cout << result << '\n';
	}
}
