#include <iostream>
#include <utility>
#include <vector>
using namespace std;
int board[9][9]; // 스도쿠 보드 선언
vector<pair<int, int>> points; // 빈칸의 위치 저장
int cnt = 0;
bool found = false; // 스도쿠 판 완성 플래그
bool check(pair<int, int> p)
{
    int square_x = p.first / 3; // 구역을 나눔
    int square_y = p.second / 3;
    for(int i = 0; i < 9; i++)
    {
        if(board[p.first][i] == board[p.first][p.second] && i != p.second) 
            return false; // 같은 행에 같은 숫자가 있으면 false 반환
        if(board[i][p.second] == board[p.first][p.second] && i != p.first)
            return false; // 같은 열에 같은 숫자가 있으면 false 반환
    }
    for(int i = 3*square_x; i < 3*square_x+3; i++)
        for(int j = 3*square_y; j < 3*square_y+3; j++)
        {
            if(board[i][j] == board[p.first][p.second])
            {
                if(i != p.first && j != p.second)
                    return false; // 같은 구역에 같은 숫자가 있으면 false 반환
            }
        }
    return true; // 모든 조건 만족시 유효성 검사 통과
}
void sudoku(int N) {
    if(N == cnt) // 빈칸의 개수만큼을 채워서 스도쿠 판이 완성되면
    {
        for(int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++)
                cout << board[i][j] << ' ';
            cout << '\n';
        } // 결과 출력
        found = true; // 플래그 true로 변경
        return;
    }
        for(int j = 1; j <= 9; j++)
        {
            board[points[N].first][points[N].second] = j; // 1~9 까지의 숫자를 넣어봄
            if(check(points[N])) // 결과가 유효하면 다음 빈칸을 채우러 감
                sudoku(N+1);
            if(found) // 스도쿠가 완성됬을경우 더 이상 다른 해를 찾을 필요가 없으므로 함수 종료
                return;
        }
    board[points[N].first][points[N].second] = 0;// 최적해를 못찾았을 경우 값 비워주기
     return;
}
int main() {
    pair<int, int> point;
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
        {
            cin >> board[i][j];
            if(board[i][j] == 0)
            {
                cnt++;
                point.first = i;
                point.second = j;
                points.push_back(point); // 빈칸의 좌표 저장
            }
        }
    sudoku(0);
}
