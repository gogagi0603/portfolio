#include <bits/stdc++.h>
using namespace std;

string ltrim(const string &);
string rtrim(const string &);


/*
 * Complete the 'reciprocal' function below.
 *
 * The function accepts INTEGER N as parameter.
 */
 #include <cstring>
#include <cstdio>
void reciprocal(int N) {
    /*
    순환마디 검사
    */
    int son = 1, mom = N;
    
    int lenC = 0, lenS;
    char s[100000];
    map<int, int> m;
    int k = 0;
    
    sprintf(s,"%d.",son/mom);
    lenS = strlen(s);
    son = son%mom * 10;
    
    while (son != 0){
    
        m[son] = k++;
        sprintf(s+lenS, "%d", son/mom);
        lenS = strlen(s);
        son = son % mom * 10;
        
        if (m.find(son) != m.end()){
            lenC = k - m[son];
            break;
        }
    }
    if (lenC == 0){
        sprintf(s+lenS, "0 0");
        printf("%s\n", s);
        return;
    }
    printf("%.*s%.*s %.*s\n", lenS-lenC, s, lenC, s+lenS-lenC, lenC, s+lenS-lenC);
}
int main()
{
    string N_temp;
    getline(cin, N_temp);

    int N = stoi(ltrim(rtrim(N_temp)));

    reciprocal(N);

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

