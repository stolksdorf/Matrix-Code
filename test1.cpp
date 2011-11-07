#include<iostream>
#include<string>
#include<time.h>
#include<conio.h>
#include<windows.h>
using namespace std;

void gotoxy(int x,int y){ 
	HANDLE hConsoleOutput; 
	COORD dwCursorPosition; 
	dwCursorPosition.X=x; 
	dwCursorPosition.Y=y; 
	hConsoleOutput=GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleCursorPosition(hConsoleOutput,dwCursorPosition); 
}	

int main(){
	srand((unsigned)time(NULL));
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	int xpos=5;
	int bgreen_len=3+rand()%2;
	int dgreen_len=5+rand()%5+bgreen_len;
	int total_len=49;
	int bgreen_col=14;
	int dgreen_col=6;
	int color,color_int=0;
	int delay=40;
	string code;
	
	while(1){;
		for(int t1=0;t1<code.size();t1++){		//clear old feed
			gotoxy(xpos,t1);
			cout<<" "<<flush;
		}

		if(code.size()==total_len){
			code="";
			xpos=rand()%79;
			dgreen_col=rand()%5+1;

		}

		code+=char(33+rand()%221);

		color=15;
		color_int=0;
		for(int t2=code.size()-1;t2>0;t2--){
			color_int++;
			gotoxy(xpos,t2);
			SetConsoleTextAttribute(hConsole, color);
			cout<<code[t2]<<flush;
			color=dgreen_col+8;
			if(color_int>bgreen_len)
				color=dgreen_col;
			if(color_int>dgreen_len)
				color=0;
		}
		Sleep(delay);
	}

	int t1=0;

	while(t1!=300){
		cout<<char(t1);
		if((t1%60)==0)
			cout<<endl;
		t1++;
	}
	cout<<endl<<endl;


	return(0);
}




		
