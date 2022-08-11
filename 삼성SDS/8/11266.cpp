#include <iostream>
#include <vector> // 인접리스트
using namespace std;
int V,E,a,b;
vector<int> AL[10001]; // 각정점의 단절점 여부 

int order; // 방문 순서 

int dfs(int curr, int parent){
	VisitOrder[curr] = ++order;
	// 다음 방문 예정인 점을 탐색 
	//내 부모에게 넘겨줄 low (현재 나의 리턴값) 저장하는 변수
	// 나와 연결된 점을 방문 했을 때 방문한 연결 점이 방문했던 점들 중 order가 가장 빠른 것 
	int minOrder = VisitOrder[curr]; 
	
	int child = 0;	
	for (int next : AL[curr]){
		 
		 
		if (next == parent){ // 직접적인 부모노드인 경우 pass 
			continue;
		}
		if (VisitOrder[next] > 0){ // next가 이미 방문을 했을때 
			// DST의 관점에서 보면 이러한 노드들은curr의 조상인 경우 외엔 존재 할 수 없다
			// 다시 말해 next노드는 현재 dfs작업이 끝난 상태가 아니므로  next 노드
			//에서의 유효한 ordeㄱ값은 visitorder[next]가 유일하다 
			minOrder = VisitOrder[next] < minOrder ? VisitOrder[next] : minOrder ;
		} 
		else{ //next가 아직 방문을 안했는가 
			++child;
			int low = dfs(next, curr);
			
			if (parent != 0 && VisitOrder[curr] <= low){
				CutVertex[curr] = 1;
			}
			minOrder = low < minOrder ? low : minOrder;
		}
		
	}
	return minOrder;
}

int main(void){
	cin >> V >> E;
	
	for (int i = 0 ; i <E; i++){
		cin >> a >> b ;
		AL[a].push_back(b);
		AL[b].push_back(a);
	}
	
	for (int i = 1; i <= V; i++){
		if (VisitOrder[i] == 0){
			dfs(i);
		}
	}
	
	for (int i = 1; i <= V; i++){
		if (CutVerText[i]){
			Answer[s_Ans++] = i;
		}
	}
	cout << s_Ans << '\n';
	
	for (int i = 0 ; i < s_Ans; i++){
		cout << Answer[i] << " ";
	}
} 
