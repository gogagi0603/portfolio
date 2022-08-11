#include <iostream>
#include <algorithm>
#include <queue>
using namespace std;

using Pair = pair<int,int>;

struct cmp{
	bool operator()(const Pair& a, const Pair& b){
		return a.second > b.second;
	}
};

bool compare(const Pair& a, const Pair& b){
	return a.second < b.second;
}

int main(void){
	priority_queue<Pair, vector<Pair>, cmp> q;
	
	q.push({1,2});
	q.push({1,9});
	q.push({1,4});
	q.push({1,5});
	q.push({1,3});
	
	for (int i = 0; i < 5 ; i++){
		cout << q.top().first <<" "<< q.top().second << '\n';
		q.pop();
	}
	
}
