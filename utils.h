/*		This is the Utilities Header file

  To include these useful functions into your program use the code:
  #include "utils.h"

  Gotoxy
  This is the standard gotoxy command, it takes the parameters of x and y, respectively
  This function then goes to that position on the screen allowing you to draw anywhere on the screen

  Menu
  The menu function is useful for quickly making a menu the user can interact with multiple inputs.
  It takes a minimum of three parameters; and x and y coordinate of where you wnat to the menu to be drawn, 
  and a vector of strings as your options. The menu function is an integer meanign that the function will 
  return a number. This number will be the element value of which option they chose. 
  Example:
	int temp;
	temp=menu(0,0,choices);
	if(temp==0) cout<<"you chose option one!";

  Optional parameters are firstly a title, which is a string, that will be displayed above your menu. And an 
  integer value of which option the menu cursor will intialing start at

  Strget
  Strget is a useful function for gettign around an error in Microsoft's string library. The problem is that 
  getline and cin do not work properly for getting strings from the user. I designed this function to get 
  around this so you can input string from the console. The way it works is that the function runs an infinite loop.
  everytime the user presses any key, it converts the keystroke into an integer,stores it into a vector, and outputs 
  what they typed on the screen. When the user hits the delimiter (the key that will end the input, usually enter) the
  loop will end, and the function will convert the vector on integers back into characters and build a new string. The
  function will then return this string. 
  Strget has 2 parameters, and x and y coordinate of where on the screen to draw the users output. It also has 2 optional
  parameters as well. The first optional parameter is the delimiter. You can change which keypress will exit the input. 
  32, for example, would the the space bar, if you wnated the uder to only enter a single word. The second optional 
  parameter  is what the function will output to the screen. If you change this to "*" for example, it will only output
  "*" for each key press they enter. 
  Example:
	string temp=strget(0,1);
	string temp=strget(4,5,32);
	string temp=strget(19,8,13,"*");


  Setcolor
  Some of you already know how to change individual colors of characters on the screen, some of you don't. This function 
  will make it alot easier to change colors. I used an enumeral to decalre all the colors possible for the function.
  There are 16 colors in all, and you can check up the function to see how to access them. The use of the function is
  really simple. 
  setcolor(blue);  //the text is now set to blue
  there is also a secondary parameter which is optional. This secondary parameter will set the background color.
  setcolor(drk_red,white);   //this will have dark red text on a white background

  If you have any question or comments, email me at scott.tolksdorf@gmail.com. Hopefully this will be useful!
*/


#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include<string>
#include<vector>
#include<math.h>
#include<fstream>
#include<time.h>
using namespace std;

void gotoxy(int x,int y){				
	HANDLE hConsoleOutput; 
	COORD dwCursorPosition; 
	dwCursorPosition.X = x; 
	dwCursorPosition.Y = y; 
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleCursorPosition(hConsoleOutput,dwCursorPosition); 
} 

int menu(int offsetx, int offsety, vector<string> &options, string title="", int menu_pos=0){
	gotoxy(offsetx,offsety);
	cout<<title<<flush;
	if(title=="") offsetx++;offsety++;
	for(int menu_t=0;menu_t<options.size();menu_t++){
		gotoxy(offsetx+2,offsety+menu_t);
		cout<<options[menu_t];
	}
	gotoxy(offsetx,offsety+menu_pos);
	cout<<">"<<flush;
	while(1){
		if(kbhit()){
			int user_input=getch();
			gotoxy(offsetx,offsety+menu_pos); cout<<" ";
			if(user_input==80) menu_pos++;
			if(user_input==72) menu_pos--;
			if(menu_pos==options.size()) menu_pos=0;
			if(menu_pos<0) menu_pos=options.size()-1;
			gotoxy(offsetx,offsety+menu_pos);
			cout<<">"<<flush;
			if(user_input==13) break;
		}
	}
	return(menu_pos);
}
string strget(int x, int y, int delimiter=13, string disp=""){
	string temp;
	vector<int> strget_cache;
	while(1){
		if(kbhit()){
			int strget_user=getch();
			if(strget_user==delimiter) break;
			if(strget_user==8){
				strget_cache.pop_back();
				x--;
				gotoxy(x,y); cout<<" ";
				
			}
			else{
				strget_cache.push_back(strget_user);				
				gotoxy(x,y);
				if(disp=="") cout<<char(strget_user);
				else cout<<disp;
				x++;
			}
		}
		cout<<flush;
	}
	for(int strget_temp=0;strget_temp<strget_cache.size();strget_temp++)
		temp+=strget_cache[strget_temp];
	return(temp);
}

string itos(int convert, int radix=10){
	string tochar="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int temp=convert%radix;
	string temp_str;
	temp_str=tochar[temp];
	if(convert-temp==0) return(temp_str);
	else return (itos((convert-temp)/radix,radix)+temp_str);
}

int rnd(int upper, int lower=0){
	srand((unsigned)time(NULL));
	return(rand()%(upper-lower)+lower);
}

string strupper(string convert){
	string result;
	for(int t1=0;t1<convert.size();t1++){
		if((int(convert[t1])>96)&&(int(convert[t1])<123)) result+=char(int(convert[t1])-32);
		else result+=convert[t1];
	}
	return(result);
}
string strlower(string convert){
	string result;
	for(int t1=0;t1<convert.size();t1++){
		if((int(convert[t1])>64)&&(int(convert[t1])<91)) result+=char(int(convert[t1])+32);
		else result+=convert[t1];
	}
	return(result);
}

void fullscreen(){
    keybd_event(VK_MENU,0x38,0,0);
    keybd_event(VK_RETURN,0x1c,0,0);
    keybd_event(VK_RETURN,0x1c,KEYEVENTF_KEYUP,0);
    keybd_event(VK_MENU,0x38,KEYEVENTF_KEYUP,0);
}


enum color_type {black,drk_blue,drk_green,drk_cyan,drk_red,drk_fushia,drk_yellow,lght_grey,drk_grey,blue,green,cyan,red,fushia,yellow,white};
void setcolor(color_type foreground, color_type background=black){
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, foreground+background*16);
}

void setcolor(int foreground, int background=0){
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, foreground+background*16);
}