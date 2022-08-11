#include <iostream>
#include <queue>
#include <cmath>
#include <sstream>
#include <string>
#include <set>
using namespace std;

string n;
int N,M;
int length = 0;

queue<int> q;

int swap(int number, int i, int j){
   
   if (i >= length || j >= length ){
      cout << "error" << '\n';
      return -1;
   }
   
   int tmpI;
   int powI = (int)pow(10,i);
	tmpI = number % (powI*10);
	tmpI = tmpI / powI;
	number = number - tmpI * powI;
	
	
	int tmpJ;
	int powJ = (int)pow(10,j);
	tmpJ = number % (powJ * 10);
	tmpJ = tmpJ / powJ;
	number = number - tmpJ * powJ;
	
	
	number = number + tmpI * powJ;
	number = number + tmpJ * powI;
	if (number < pow(10,length - 1)){
		return -1;
	}
	return number;
}


int max(int a, int b){
   return a > b ? a:b;
}

int main(void){
   cin >> n >> M;
   stringstream(n) >> N;
   length = n.size();
   
   int max_result = -1;
   queue<int> next_queue;
   next_queue.push(N);
   
   
   if (length == 1){
		cout << -1 << '\n';
		return 0;
   }
   
   
   for (int itr = 0; itr < M; itr++){
      q = next_queue;
      next_queue = queue<int> ();
      set<int> s;
      while (!q.empty()){
         
         int target = q.front();
         q.pop();
         
         if (s.count(target) != 0){
         	continue;
		 }
		 s.insert(target);
         for (int i = 0 ; i < length ; i ++){
            for (int j = i+1 ; j < length ; j++){
               if (i!=j){
                  
                  int swap_result = swap(target,i,j);
                  if (swap_result != -1)
                  next_queue.push(swap_result);
                  //cout << swap_result << '\n';
                  
                  //max_result = max(swap_result,  max_result);
               }
            }
         }
      }
      
   }
   
   while (!next_queue.empty()){
      max_result = max(max_result, next_queue.front());
      next_queue.pop();
   }
   
   
   cout << max_result << '\n'; 
}
