#include <iostream>

using namespace std;

const int BASE = 108576;
const int TSIZE = 2097152;

int N,A,B,C;
int iTree[TSIZE];

int main(void){
	
	cin >> N;
	
	for (int n = 0 ; n < N; n++){
		cin >> A >> B;
		if (A==2){
			cin >> C;
		}
		
		if (A==1){
			int index = 1;
			while (index <TSIZE){
				if (B > iTree[index]){
					B -= iTree[index++];
				}
				index *= 2;
			}
			
			index /= 2;
			B = index - BASE;
			cout << B << '\n';
			C= -1;
		}
		B += BASE;
		
		while (B){
			iTree[B] += C;
			B/= 2;
		}
	}
}
