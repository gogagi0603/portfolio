#include <iostream>
#include <utility>
#include <string>
#include <vector>
using Pair = std::pair<int, int>;


int row, col;
std::vector<std::string> map;
bool** bool_map;
int max_result = 0;
int** dp;

int max(int a, int b){
	return a>b ? a:b;
}

// 범위안이냐? H 냐? 
bool isRange(Pair rotation){
	if (rotation.first >= 0 && rotation.second >= 0 && rotation.first < col && rotation.second < row){
			
			if (map[rotation.second][rotation.first] == 'H'){
			return false;
		
		}
		
		return true;
	}
	else{
		
		return false;
	}
}
void print_bool(){
	for (int i = 0 ; i < 3; i ++){
		for (int j = 0 ; j < 7 ; j++){
			std::cout << bool_map[i][j]; 
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void print(std::vector<Pair> vec){
	for (int i = 0 ; i < (int)vec.size(); i ++){
		std::cout << "{ " << vec[i].first << ", " << vec[i].second << " } ";
	}
	std::cout << std::endl;
}
bool isRecursive = false;

// DFS
bool search(Pair rotation, std::vector<Pair>& stack){
	
	if (!isRange(rotation)){
		return false;
	}
	//print(stack);
	while(stack.size() != 0){
		int y = rotation.first;
		int x = rotation.second;
		int move_point = map[x][y] - '0';
		
		//가로 방향 이동 
		int dy[4] = {move_point, -move_point, 0 ,0};
		// 세로 방향 이동 
		int dx[4] = {0, 0, move_point, -move_point};
		
		for (int i = 0 ; i < 4; i ++){
			int nx = x + dx[i];
			int ny = y + dy[i];
			
			if (isRange({ny,nx}) == true){
				if (bool_map[nx][ny] == true){
					isRecursive = true;
					return false;
				}
				
				Pair nrotation = {ny,nx};
				stack.push_back(nrotation);
				bool_map[nx][ny] = true;
				
				if (dp[nx][ny] < (int)stack.size()){
					dp[nx][ny] = (int)stack.size();
					max_result = max(max_result,(int)stack.size());
					search(nrotation, stack);
				}
				else{
					
				}
				
			}
			
		}
		//print(stack);
		bool_map[stack[stack.size()-1].second][stack[stack.size()-1].first] = false;
		
		//print_bool();
		//print(stack);
		stack.pop_back();
		//print(stack);
		
		return false;
	}
	
}

int main(void){
	std::cin >> row >> col;
	
	
	bool_map = new bool*[row]; 
	for (int i = 0; i < row; i++){
		bool_map[i] = new bool[col];
	}
	
	for (int i = 0 ; i < row; i ++){
		for (int j = 0 ; j < col; j++){
			bool_map[i][j] = false;
		}
	}
	
	dp = new int*[row];
	
	for (int i = 0 ; i < row; i ++){
		dp[i] = new int [col];
	}
	
	for (int i = 0 ; i < row ; i++){
		for (int j = 0 ; j < col; j++){
			dp[i][j] = 0;
		}
	}
	
	std::vector<Pair> stack;
	
	for (int i = 0 ; i < row ; i ++){
		std::string input;
		std::cin >> input;
		map.push_back(input);
	}
	

	if (row == 1 && col == 1){
		std::cout << 1;
		return 0;
	}
	
	int max_count = 0;
	stack.push_back({0,0});
	Pair rotation = {stack[stack.size()-1].second, stack[stack.size()-1].first}; 
	
	search(rotation, stack);
	
	for (int i = 0; i < row ; i++){
		delete[] bool_map[i];
	}
	delete[] bool_map;
	
	if (!isRecursive){\
		std::cout << max_result;	
	}
	else{
		std::cout << -1;
	}
}
