#include <iostream>
#include <vector> // ��������Ʈ
using namespace std;
int V,E,a,b;
vector<int> AL[10001]; // �������� ������ ���� 

int order; // �湮 ���� 

int dfs(int curr, int parent){
	VisitOrder[curr] = ++order;
	// ���� �湮 ������ ���� Ž�� 
	//�� �θ𿡰� �Ѱ��� low (���� ���� ���ϰ�) �����ϴ� ����
	// ���� ����� ���� �湮 ���� �� �湮�� ���� ���� �湮�ߴ� ���� �� order�� ���� ���� �� 
	int minOrder = VisitOrder[curr]; 
	
	int child = 0;	
	for (int next : AL[curr]){
		 
		 
		if (next == parent){ // �������� �θ����� ��� pass 
			continue;
		}
		if (VisitOrder[next] > 0){ // next�� �̹� �湮�� ������ 
			// DST�� �������� ���� �̷��� ������curr�� ������ ��� �ܿ� ���� �� �� ����
			// �ٽ� ���� next���� ���� dfs�۾��� ���� ���°� �ƴϹǷ�  next ���
			//������ ��ȿ�� orde������ visitorder[next]�� �����ϴ� 
			minOrder = VisitOrder[next] < minOrder ? VisitOrder[next] : minOrder ;
		} 
		else{ //next�� ���� �湮�� ���ߴ°� 
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
