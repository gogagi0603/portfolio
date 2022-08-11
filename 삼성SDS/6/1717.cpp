#include <iostream>
using namespace std;
// Union Find
const int Max = 1000000;

int N, M;
// A[i]: i 의 root
int A[Max];
// Find 함수
int find(int num){
	if(A[num] < 0)
		return num;
	int parent = find(A[num]);
	A[num] = parent;
	return parent
}
// Union 함수
void merge(int a, int b){
	a = find(a);
	b = find(b);
	if(a == b) return;
	A[a] = b;
}
int n,m,l;
int main(){ios_base::sync_with_stdio(false); cout.tie(NULL); cin.tie(NULL);

	cin >> N >> M;
	for (int i = 0; i < N; ++i) A[i] = -1;
	for (int i = 0; i < M; ++i){
		cin >> n >> m >> l;
		if(n == 0)
			merge(m,l);
		else if(n == 1){
			if(find(m) == find(l))
				cout << "YES" << '\n';
			else
				cout << "NO" << '\n';
		}
	}

    return 0;
}
