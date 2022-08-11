#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;
using Pair = pair<int, int>;
int n,m, depth;
int Map[50][50];
int yy[4] = {1,-1,0,0};
int xx[4] = {0,0,1,-1};
bool visited[50][50];

bool isRange(Pair a){
    if (a.first < m && a.first >=0 && a.second < n && a.second >=0){
        return true;
    }
    return false;
}
int max_res = 0;

int max(int a, int b){
	return a > b ? a : b;
}



void start(Pair curr, int dept, int sum){
    if (dept > depth){
		for (int i = 0 ; i < n ; i++){
	    		for (int j = 0 ; j < m ; j++){
	    			cout << visited[i][j] << " ";
				}
				cout << '\n';
			}
		cout << '\n';
		cout << sum << '\n';	
    	max_res = max(max_res, sum);
    	
		cout << '\n';
        return;
    }
    visited[curr.second][curr.first] = true;
	sum = sum + Map[curr.second][curr.first];
	
    for (int i = 0 ; i < 4; i++){
        Pair move_point = {curr.first + yy[i], curr.second + xx[i]};
        if (isRange(move_point) && visited[move_point.second][move_point.first] ==false){
            start(move_point, dept+1, sum);
        }
    }
    visited[curr.second][curr.first] = false;
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    // 세그먼트 트리 or 냅다 브루트포스
    // element : 2500, TL : 2sec, depth:10
    
    cin >> n >> m >> depth;
    for (int i = 0; i < n ; i++){
        for (int j = 0 ; j < m ; j++){
            char tmp;
            cin >> tmp;
            Map[i][j] = tmp-'0';
            visited[i][j] = false;
        }
    }
    // 브루트포스 + 백트래킹으로 될것 같은데?
    // 하.. 근데 코로나 너무 아프다
    vector<int> test;
    int sum = 0;
    for (int i = 0 ; i < n ; i++){
        for (int j = 0 ; j < m ; j++){
        	cout << "start : " << j << " " << i << '\n';
            start({j,i}, 1, 0);
        }
    }  
    cout << max_res;
    return 0;
}
