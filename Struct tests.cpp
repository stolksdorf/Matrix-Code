#include<iostream>
#include<vector>

using namespace std;

struct test{
	int value;
	bool active;
};
vector<test> test2;

void main(){
	test temp;
	test2.resize(90);
	cout<<test2.size()<<endl;
	test2[0].value=3;
	cout<<test2[0].value<<endl;
}

	