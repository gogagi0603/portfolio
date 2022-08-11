#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <list>
std::list<int> stack;

constexpr int INF = 1e9;

void print(){
	std::cout << "현재 스택 : ";
	for (std::list<int>::iterator itr = stack.begin() ; itr!= stack.end() ;itr++){
		std::cout << *itr << " ";
	}
	std::cout << std::endl;
}

void NUM (int X){
	stack.push_back(X);
}

void POP(){
	stack.pop_back();
}

void INV(){
	int n = stack.front() * -1;
	stack.pop_front();
	stack.push_front(n);
}

void DUP(){
	stack.push_back(stack.front());
}

int SWP(){
	if (stack.size() < 2){
		std::cout << "ERROR" << std::endl;
		return -1;
	}
	
	std::list<int>::iterator begin = stack.begin();
	std::list<int>::iterator second = begin++;
	
	int tmp1 = *begin;
	*begin = *second;
	*second = tmp1;
	return 0;
}

int ADD(){
	if (stack.size() < 2){
		
		std::cout << "ERROR" << std::endl;
		return - 1;
	}
	
	std::list<int>::iterator begin = stack.begin();
	std::list<int>::iterator second = begin++;
	
	int tmp = *begin + *second;
	begin = stack.erase(begin);
	begin = stack.erase(begin);
	
	if (std::abs(tmp) > INF){
		
		std::cout << "ERROR" << std::endl;
		return - 1;
	}
	stack.push_back(tmp);
	return 0;
}

int SUB(){
	if (stack.size() < 2){
		std::cout << "ERROR" << std::endl;
		return - 1;
	}
	std::list<int>::iterator begin = stack.begin();
	std::list<int>::iterator second = begin++;
	
	int tmp = *second - *begin;
	begin = stack.erase(begin);
	begin = stack.erase(begin);
	
	if (std::abs(tmp) > INF){
		std::cout << "ERROR" << std::endl;
		return - 1;
	}
	stack.push_back(tmp);
	return 0;
}

int MUL(){
	if (stack.size() < 2){
		std::cout << "ERROR" << std::endl;
		return - 1;
	}
	
	std::list<int>::iterator begin = stack.begin();
	std::list<int>::iterator second = begin++;
	
	
	int tmp = (*begin) * (*second);
	if (std::abs(tmp) > INF){
		std::cout << "ERROR" << std::endl;
		return - 1;
	}
	begin = stack.erase(begin);
	begin = stack.erase(begin);
	
	stack.push_back(tmp);
	return 0;
}

int DIV(){
	
	if (stack.size() < 2 || stack.front() == 0){
		std::cout << "ERROR" << std::endl;
		return - 1;
	}
	
	std::list<int>::iterator begin = stack.begin();
	std::list<int>::iterator second = begin++;
	
	
	int minus_count = 0;
	if (*second < 0){
		minus_count ++;
	}
	if (*begin < 0){
		minus_count ++;
	}
	
	
	int tmp = std::abs(*second) / std::abs(*begin);
	begin = stack.erase(begin);
	begin = stack.erase(begin);
	
	
	if (minus_count == 1 && tmp > 0){
		tmp *= -1;
	}
	if (minus_count != 1 && tmp < 0){
		tmp *= -1;
	}
	if (std::abs(tmp) > INF){
		std::cout << "ERROR" << std::endl;
		return - 1;
	}
	stack.push_back(tmp);
	return 0;
}

int MOD(){
	if (stack.size() < 2  || stack.front() == 0 ){
		std::cout << "ERROR" << std::endl;
		return - 1;
	}
	std::list<int>::iterator begin = stack.begin();
	std::list<int>::iterator second = begin++;
	
	int q = 1;
	if (*second < 0){
		q = -1;
	}
	int tmp = (*second % *begin) * q;
	
	begin = stack.erase(begin);
	begin = stack.erase(begin);
	
	if (std::abs(tmp) > INF){
		std::cout << "ERROR" << std::endl;
		return - 1;
	}
	stack.push_back(tmp);
	return 0;
}





int main(void){
	std::string input;
	while(true){
		
		std::vector<std::string> strings;
		std::vector<int> num;
		std::vector<int> inputs;
		
		while (true){
			std::cin >> input;
			if (input.compare("END") == 0){
				break;
			}
			strings.push_back(input);
			if (input.compare("NUM") == 0){
				int tmp;
				std::cin >> tmp;
				num.push_back(tmp);
				
			}
		}
		
		int iter;
		std::cin >> iter;
		
		for (int i = 0 ; i < iter ; i++){
			int tmp;
			std::cin >> tmp;
			inputs.push_back(tmp);
		}
		
		for (std::vector<int>::iterator iitr = inputs.begin(); iitr != inputs.end() ; iitr++){
			stack.push_back(*iitr);
			int n = 0;
			for (int i = 0 ; i < (int)strings.size() ; i++){
				if (strings[i].compare("DUP") == 0){
					DUP();
				}
				
				if (strings[i].compare("MUL") == 0){
	
					if (MUL() == -1){
						std::cout << "ERROR" << std::endl;
						break;
					}
				}
				
				
				if (strings[i].compare("NUM") == 0){
					int x = num[n];
					NUM(x);
					n++;
				}
				if (strings[i].compare("ADD") == 0){
					if (ADD() == -1){
						std::cout << "ERROR" << std::endl;
						break;
					}
				}
				
				print();
				
				
			}
			if (stack.size() != 1){
				std::cout << "ERROR" << std::endl;
			}
			else{
				std::cout << " result : " << stack.front() << std::endl;	
			}
			stack = {};
		}
		
	}
	
}
