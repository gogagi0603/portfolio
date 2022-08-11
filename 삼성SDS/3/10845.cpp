#include <iostream>
#include <list> 
#include <string>
// 리스트를 쓰지않고?
using namespace std;

list<int> queue;

void push(int X){
	queue.push_back(X);
}
void pop(){
	if (queue.empty()){
		cout << -1 << endl;
		return;
	}
	cout << queue.front() << endl;;
	queue.pop_front();
}

void size(){
	cout << queue.size() << endl;
}

void empty(){
	cout << queue.empty() << endl;
}

void front(){
	if (queue.empty()){
		cout << -1 << endl;
		return;
	}
	
	cout << queue.front() << endl;
}

void back(){
	if (queue.empty()){
		cout << -1 << endl;
		return;
	}
	cout << queue.back() << endl;
}

int main(void){
	string input;
	int itr;
	cin >> itr;
	
	for (int i = 0 ; i < itr ; i ++){
		cin >> input;
		if (input.compare("push") == 0){
			int X;
			cin >> X;
			push(X);
		}
		else if (input.compare("front") == 0){
			front();
		}
		else if (input.compare("back") == 0){
			back();
		}
		else if (input.compare("size") == 0){
			size();
		}
		else if (input.compare("empty") == 0){
			empty();
		}
		else if (input.compare("pop") == 0){
			pop();
		}
		
	}
}
