#include <iostream>
#include <vector>
#include <algorithm>
#include <list>

using namespace std;
//n == 너비
// m == 높이
// t == 금강석 개수
// k == 정사각형 한변의 길이 

using Pair = pair<int,int>;

int n,m,t,k;
vector<Pair> map;

vector<Pair> square;

Pair slide_left, slide_right;

void print(vector<Pair> vec){
	for (int i = 0 ; i < (int)vec.size() ; i++){
		cout <<"{ "<< vec[i].first << " " << vec[i].second << " }";
	}
	cout << '\n';
}

void printP(Pair p){
	cout << "{ " << p.first << " " << p.second << " }" << '\n';
}

bool isInside(Pair p){
	return (slide_left.second <= p.second && p.second <= slide_right.second) && (slide_left.first <= p.first && p.first <= slide_right.first);
}

bool compare(Pair a, Pair b){
	if (a.first == b.first){
		return a.second < b.second;
	}
	return a.first < b.first;
}

int max(int a, int b){
	return a > b ? a :b;
}

int move_point = 1;

void move_slide(){
	if (slide_right.first >= n && move_point == 1){
			move_point = -1;
			
			slide_right.second ++;
			
			slide_left.second ++;
	}
	else if (move_point == -1 && slide_left.first <= 1){
		move_point = 1;
		slide_left.second++;
		slide_right.second++;
	}
	else{
		slide_right.first += move_point;
		slide_left.first += move_point;
	}
	
}

int main(void){
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	
	cin >> n >> m >> t >> k;
	
	slide_left = {1,1};
	slide_right = {1+k, 1+k};
	
	int a,b;
	for (int i = 0 ; i < t ; i++){
		cin >> a >> b;
		map.push_back({b,a});	
	}
	
	for (int i = 0 ; i < t ; i++){
		if (isInside(map[i])){
			square.push_back(map[i]);		
		}
	}
	int result = 0;
	Pair resultP;
	
	while (true){
		int count = 0;
		
		for (int i = 0 ; i < t ; i++){
			if (isInside(map[i])){
				count ++;		
			}
		}
		
		if (result < count){
			result = count;
			resultP = {slide_left.first, slide_right.second};
			
		}
		if((slide_right.first >= n && slide_right.second >= n) || ((slide_left.first <= 1 && slide_left.second<=1) && move_point==-1)){
			break;
		}
		move_slide();

	}
	
	int max_count = (int)square.size();
	cout << resultP.first << " " << resultP.second << '\n';
	cout <<result << '\n';
	//print(square);
}
