#include <iostream>
using namespace std;

int main(void){
	long a,b,c,d,mom,son,tmom,tson;
	cin >> a >> b >> c >> d;
	tmom = mom = b * d;
	tson = son = (b*c) + (a*d);
	

	long mod = tmom % tson;
	
	while (mod !=0){ 
		tmom = tson;
		tson = mod;
		mod = tmom % tson;
		
	}
	
	son /= tson;
	mom /= tson;
	
	
	
	cout << son << " " << mom << '\n';	
}
