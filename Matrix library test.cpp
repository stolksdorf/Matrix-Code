#include<iostream>
#include<windows.h>
#include<conio.h>

using namespace std;

void gotoxy(int x,int y){ 
	HANDLE hConsoleOutput; 
	COORD dwCursorPosition; 
	dwCursorPosition.X=x; 
	dwCursorPosition.Y=y; 
	hConsoleOutput=GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleCursorPosition(hConsoleOutput,dwCursorPosition); 
}


void main(){
	for(int t1=0; t1<256;t1++){
		gotoxy(0,0);
		cout<<t1<<"   "<<char(t1)<<flush;
		while(!kbhit());
		int user=getch();
	}
}