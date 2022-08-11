#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);


/*
 * Complete the 'closestStraightCity' function below.
 *
 * The function is expected to return a STRING_ARRAY.
 * The function accepts following parameters:
 *  1. STRING_ARRAY c
 *  2. INTEGER_ARRAY x
 *  3. INTEGER_ARRAY y
 *  4. STRING_ARRAY q
 */
 
 
/*
c = {fastcity, bigbanana, xyz}
x = {23,       23       , 23 }
y = {1         ,10      , 20}
q = {fastcity, bigbanana, xyz}
*/
using Pair = pair<int, int>; //y, x
double distance(Pair a, Pair b){
    return sqrt(pow(a.first - b.first,2) + pow(a.second - b.second, 2));
}

int min(int a,int b){
    return a > b ? b : a;
}

vector<string> closestStraightCity(vector<string> c, vector<int> x, vector<int> y, vector<string> q) {
    map<string, int> m; //name -> index
    vector<string> ms(x.size());  //index -> name
    vector<Pair> lotation(x.size());
    vector<string> res;
    for (int i = 0 ; i < (int)x.size(); i++){
        m.insert(make_pair(c[i], i));
        //ms[i] = c[i];
        lotation[i] = {y[i], x[i]};
    }
    
    // q의 도시들을 검색
    for (int i = 0 ; i < (int)q.size(); i++){
        // 도시 이름을 이용해서 map에서 검색
        int index = m[q[i]];
        // 검색해서 나온 index를 lotation에서 좌표 검색
        Pair q_p = lotation[index];
        // lotation y,x가 하나라도 같은 도시들을 찾음
        int min_res = 1e9+7;
        int min_index = -1;
        for (int j = 0 ; j < x.size(); j++){
            if (j==index){
                continue;
            }
            if (lotation[j].first == q_p.first){
                int dis = distance(lotation[j], q_p);
                if (dis < min_res){
                    min_res = dis;
                    min_index = j;
                }
            }
            if (lotation[j].second == q_p.second){
                int dis = distance(lotation[j], q_p);
                if (dis < min_res){
                    min_res = dis;
                    min_index = j;
                }
            }
        }
        if (min_index == -1){
            res.push_back("NONE");
            continue;
        }
        res.push_back(c[min_index]);
        // 그 도시들과 q[i]의 거리 계산
        // 가장 거리가 작은 도시를 답으로
        
    }
    
    
    return res;
}
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string c_count_temp;
    getline(cin, c_count_temp);

    int c_count = stoi(ltrim(rtrim(c_count_temp)));

    vector<string> c(c_count);

    for (int i = 0; i < c_count; i++) {
        string c_item;
        getline(cin, c_item);

        c[i] = c_item;
    }

    string x_count_temp;
    getline(cin, x_count_temp);

    int x_count = stoi(ltrim(rtrim(x_count_temp)));

    vector<int> x(x_count);

    for (int i = 0; i < x_count; i++) {
        string x_item_temp;
        getline(cin, x_item_temp);

        int x_item = stoi(ltrim(rtrim(x_item_temp)));

        x[i] = x_item;
    }

    string y_count_temp;
    getline(cin, y_count_temp);

    int y_count = stoi(ltrim(rtrim(y_count_temp)));

    vector<int> y(y_count);

    for (int i = 0; i < y_count; i++) {
        string y_item_temp;
        getline(cin, y_item_temp);

        int y_item = stoi(ltrim(rtrim(y_item_temp)));

        y[i] = y_item;
    }

    string q_count_temp;
    getline(cin, q_count_temp);

    int q_count = stoi(ltrim(rtrim(q_count_temp)));

    vector<string> q(q_count);

    for (int i = 0; i < q_count; i++) {
        string q_item;
        getline(cin, q_item);

        q[i] = q_item;
    }

    vector<string> result = closestStraightCity(c, x, y, q);

    for (int i = 0; i < result.size(); i++) {
        fout << result[i];

        if (i != result.size() - 1) {
            fout << "\n";
        }
    }

    fout << "\n";

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

