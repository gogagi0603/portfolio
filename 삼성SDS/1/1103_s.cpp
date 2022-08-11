#include <iostream>
#include <algorithm>
#include <string>
using namespace std;

char map[50][50];
int dp[50][50];
bool visited[50][50];
int n,m;

const int dx[4] = {1,-1,0,0};
const int dy[4] = {0,0,1,-1};

int dfs(int cy, int cx){
	
	
	if (cy < 0 || cx < 0 || cx>=m ||cy >= n || map[cy][cx] == 'H'){
		return 0;
	}
	if (visited[cy][cx]){
		cout << "-1" << '\n';
		exit(0);
	}
	//cout << cy << " " << cx << '\n';
	int &cache = dp[cy][cx];
	if (cache != -1){
		return cache;
	}
	visited[cy][cx] = true;
	
	cache = 0;
	for (int dir = 0; dir < 4 ; ++dir){
		int ny = cy + dy[dir] * (map[cy][cx] - '0');
		int nx = cx + dx[dir] * (map[cy][cx] - '0');
		
		cache = max(cache, dfs(ny,nx) + 1);
	}
	visited[cy][cx] = false;
	return cache;
}

int main(void){
	cin >> n >> m;
	for (int i = 0; i < 50; i++){
		for (int j = 0 ; j < 50 ; j++){
			dp[i][j] = -1;
		}
	}
	
	
	
	for (int i = 0 ; i < n ; i++){
		cin >> map[i];
	}
	cout << dfs(0,0);
}
