#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);



/*
 * Complete the 'minMoves' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 *  1. 2D_INTEGER_ARRAY maze
 *  2. INTEGER x
 *  3. INTEGER y
 */
 using Pair = pair<int, int>;
/*
0 : 갈 수 있음
1 : 갈수 없음
2 : 갈 수 있고 금화있음

후보 1. 다익스트라
후보 2. 플로이드 워셜
후보 3. 벨만포드
후보 4. A*
후보 5. 그냥 BFS
*/
int x1[4] = {0,0,1,-1};
int y1[4] = {1,-1,0,0};

int minMoves(vector<vector<int>> maze, int x, int y) {
    Pair finish = {x,y};
    //BFS를 재귀로 돌리면서 도착했을때 만난 골드양이 목표 골드양일 경우 count++;
    
    return 0;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string maze_rows_temp;
    getline(cin, maze_rows_temp);

    int maze_rows = stoi(ltrim(rtrim(maze_rows_temp)));

    string maze_columns_temp;
    getline(cin, maze_columns_temp);

    int maze_columns = stoi(ltrim(rtrim(maze_columns_temp)));

    vector<vector<int>> maze(maze_rows);

    for (int i = 0; i < maze_rows; i++) {
        maze[i].resize(maze_columns);

        string maze_row_temp_temp;
        getline(cin, maze_row_temp_temp);

        vector<string> maze_row_temp = split(rtrim(maze_row_temp_temp));

        for (int j = 0; j < maze_columns; j++) {
            int maze_row_item = stoi(maze_row_temp[j]);

            maze[i][j] = maze_row_item;
        }
    }

    string x_temp;
    getline(cin, x_temp);

    int x = stoi(ltrim(rtrim(x_temp)));

    string y_temp;
    getline(cin, y_temp);

    int y = stoi(ltrim(rtrim(y_temp)));

    int result = minMoves(maze, x, y);

    fout << result << "\n";

    fout.close();

    return 0;
}

string ltrim(const string &str) {
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );

    return s;
}

string rtrim(const string &str) {
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
        s.end()
    );

    return s;
}

vector<string> split(const string &str) {
    vector<string> tokens;

    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = str.find(" ", start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));

        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}

