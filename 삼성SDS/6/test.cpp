#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main(void){
	int a = 12345;
	string b = "123456";
	stringstream sp(b);
	int tmp;
	sp >> tmp;
	cout << to_string(a) << '\n';
	cout << tmp;
}
