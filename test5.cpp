#include <iostream>
#include <conio.h>
#include <windows.h>
#include <string>
#include <vector>
#include <time.h>
using namespace std;

void setcolor(int foreground, int background=0){
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, foreground+background*16);
}
void fullscreen(){
	keybd_event(VK_MENU,0x38,0,0);
	keybd_event(VK_RETURN,0x1c,0,0);
	keybd_event(VK_RETURN,0x1c,KEYEVENTF_KEYUP,0);
	keybd_event(VK_MENU,0x38,KEYEVENTF_KEYUP,0);
}

int rnd(int upper, int lower=0){
	return(rand()%(upper-lower)+lower);
}

void gotoxy(int x,int y){
	HANDLE hConsoleOutput; 
	COORD dwCursorPosition; 
	dwCursorPosition.X = x; 
	dwCursorPosition.Y = y; 
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleCursorPosition(hConsoleOutput,dwCursorPosition); 
}

void disptext(int offsetx,int offsety, string text, int textdelay, int color){		
	for(int dt1=0;dt1<text.size();dt1++){		//this displayign a string character by character
		gotoxy(offsetx+dt1,offsety);
		setcolor(color);						//it gives the effect of text being typed out
		cout<<text[dt1]<<flush;
		int temp=0;
		while(1){
			if(temp==textdelay/5) break;
			Sleep(5);
			
			if(kbhit()){ textdelay=0;break;}
			temp++;
		}
	}
}

void manual(){		//instructional manual 
	disptext(17,0,"Welcome to the Standard Operator Training Program",20,10);
	disptext(25,1,"Written by:",20,9);
	disptext(37,1,"Scott Tolksdorf",20,8);
	disptext(0,2,"Press anykey to skip manual",20,7);
	Sleep(200);
	disptext(0,4,"Press",30,10);
	disptext(6,4,"enter",30,11);
	disptext(12,4,"to create a live feed",30,10);	
	disptext(0,6,"The live feed is your main source of communication with the Matrix",30,10);
	Sleep(200);
	disptext(0,7,"Type normally to have the Matrix display what you wish.",30,10);
	disptext(0,8,"Or by inputting certain comands, you can bend the Matrix to your will",30,10);
	Sleep(100);
	disptext(0,10,"Color:",20,11);
	disptext(7,10,"Typing 'color' then a number(1-7), will change the color of the feeds.",20,10);
	disptext(0,12,"Density:",30,11);
	disptext(9,12,"Typing 'density' then a number(1-9), changes how many feeds will be",30,10);
	disptext(9,13,"present on the screen at one time.",30,10);
	disptext(0,15,"Delay:",30,11);
	disptext(7,15,"Typing 'delay' then a number(1-9), changes how fast the feeds fall.",30,10);
	disptext(0,17,"Debug:",30,11);
	disptext(7,17,"Typing 'debug', toggles displaying a debugging tool.",30,10);
	disptext(0,19,"Quit:",30,11);
	disptext(6,19,"Typing 'quit', will end the simulation.",30,10);
	disptext(0,23,"***Remember not to try and run too many feeds at once***",10,12);
	disptext(0,24,"-->This will cause the Matrix to crash",30,4);
	disptext(16,35,"That concludes this manual, enjoy your simulation.",30,15);
	disptext(45,48,"Press anykey to begin simulation...",30,14);
}




struct buffer{
	int code;
	int color;
};
vector< vector<buffer> > m_screen;		//main matrix of the screen
vector< vector<buffer> > b_screen;		//buffer matrix of the screen

//Null Feed Vectors
struct feed_struct{
	bool active;
	int xpos; 
	int len; 
	int delay; 
	int count;
	int head; 
	int drk; 
	int lght;
	int input;
	int code_pos;
	vector<int> code;
}increase_feed;
vector<feed_struct> feed;

//Live Feed Vectors
struct live_feed_struct{
	int xpos;
	int lght;
	int drk;
	vector<int> input;
	bool active;
}lf;

//Create feed variables
int create_pos=-1;
int create_len=-1;
int create_delay=1;
int create_head=-1;
int create_drk=2;
int create_lght=10;
int create_density=1;
vector<int> create_input;

int user_flush;
bool debug=false;
bool quit=false;

int screenx=78;		//screen dimensions
int screeny=49;
vector<int> feed_input;

void core();
void manual();
void create_feed(int create_xpos, int create_len, int create_delay, int create_head, int create_lght, int create_drk, const vector<int>&);
void process_feed();
void handle_livefeed();
void disp_buffer();
void disptext(int offsetx,int offsety, string text, int textdelay, int color);
void screen_fix();
void create_livefeed();

int main(){
	srand((unsigned)time(NULL));		//init random seed

	lf.active=false;
	fullscreen();

	manual();				//run manual
	user_flush=getch();
	while(!kbhit());
	user_flush=getch();
	system("cls");
	
	m_screen.resize(screenx);
	for(int t1=0;t1<screenx;t1++){			//init matricies
		m_screen[t1].resize(screeny);
		for(int t2=0;t2<screeny;t2++){
			m_screen[t1][t2].code=32;
			m_screen[t1][t2].color=0;
		}
	}
	b_screen=m_screen;

	feed_input.push_back(-1);
	core();						//run core
	Sleep(500);
	fullscreen();
	disptext(0,0,"Press any key to terminate simulation",30,10);
	while(!kbhit());
	return(0);
}

void core(){
	int feed_count=create_density;
	while(1){
		if(kbhit()&&(!quit)){			//check for user input
			user_flush=getch();
			if((user_flush==13)&&(!lf.active))
				create_livefeed();
			else if(lf.active)
				handle_livefeed();
		}
		if(lf.active){			//draws the cycling white charcater on a live feed
			m_screen[lf.xpos][lf.input.size()].code=rnd(254,33);
			m_screen[lf.xpos][lf.input.size()].color=15;
		}
		Sleep(5);

		feed_count--;
		if(feed_count==0){
			feed_count=create_density;
			create_feed(create_pos,create_len,create_delay,create_head,create_lght,create_drk,feed_input);	//creates new feed
		}
		if(feed_count<-1) feed_count=create_density;
		process_feed();
		disp_buffer();
		if(quit){
			int temp=0;
			for(int debug=0;debug<feed.size();debug++){
				if(feed[debug].active)
					temp++;
			}
			if(temp==0) break;	//	exits when all feeds leave the screen
		}
	}
}

void create_feed(int feed_pos, int feed_len, int feed_delay, int feed_head, int feed_lcol, int feed_dcol, const vector<int>&temp_feed_code){
	vector<int> feed_code=temp_feed_code;
	int cf1;
	if(feed_pos==-1){
		while(1){
			bool xpos_check=true;			//makes sure feeds don't overlap when their created
			feed_pos=rnd(screenx);		
			if((feed_pos==lf.xpos)&(lf.active))	continue;
			for(int cfx=0;cfx<feed.size();cfx++){
				if((feed[cfx].xpos==feed_pos)&&((feed[cfx].head-feed[cfx].len))<7)
					xpos_check=false;
			}
			if(xpos_check)	break;
		}
	}
	if(feed_len==-1) feed_len=rnd(30,10);
	if(feed_delay==-1) feed_delay=rnd(4);
	if(feed_dcol==-1) {feed_dcol=rnd(6,1);feed_lcol=feed_dcol+8;}
	if(feed_code[0]==-1){
		feed_code.clear();
		for(int t1=0;t1<screeny;t1++)
			feed_code.push_back(rnd(254,33));
	}
	bool old_feed=false;

	for(cf1=0;cf1<feed.size();cf1++){		//checks for an old feed to use it's memory slot for a new feed
		if(!feed[cf1].active){				//this saves on memory, and makes processes more efficient
			old_feed=true;
			break;
		}
	}
	if(!old_feed){
		cf1=feed.size();
		feed.push_back(increase_feed);		//creates a new memory slot if an old one is available
	}
	feed[cf1].active=true;
	feed[cf1].xpos=feed_pos;
	feed[cf1].len=feed_len;
	feed[cf1].delay=feed_delay;
	feed[cf1].count=feed_delay;			//dumps info into new feed
	feed[cf1].head=feed_head;
	feed[cf1].lght=feed_lcol;
	feed[cf1].drk=feed_dcol;
	feed[cf1].code_pos=0;
	feed[cf1].code=feed_code;
}

void process_feed(){
	for(int pf=0;pf<feed.size();pf++){		//This moves ahead the four leading colors of each feed
		if(feed[pf].active){
			feed[pf].count--;
			if(feed[pf].count==0){
				feed[pf].count=feed[pf].delay;
				feed[pf].head++;
				feed[pf].code_pos++;
				if(feed[pf].code_pos==feed[pf].code.size())
					feed[pf].code_pos=0;
				if((feed[pf].head-feed[pf].len)>=screeny){
					feed[pf].active=false;				//turns off feed if it left the screen, so its memory slot can be used later
					continue;
				}
				if(feed[pf].head<screeny){		//moves white
					m_screen[feed[pf].xpos][feed[pf].head].color=15;
					m_screen[feed[pf].xpos][feed[pf].head].code=feed[pf].code[feed[pf].code_pos];
				}

				if(((feed[pf].head-1)>=0)&&((feed[pf].head-1)<screeny))		//moves the light color
					m_screen[feed[pf].xpos][feed[pf].head-1].color=feed[pf].lght;
	
				if(((feed[pf].head-feed[pf].len*.6)>=0)&&((feed[pf].head-feed[pf].len*.6)<screeny))		//moves the dark color
					m_screen[feed[pf].xpos][feed[pf].head-feed[pf].len*.6].color=feed[pf].drk;

				if(((feed[pf].head-feed[pf].len)>=0)&&((feed[pf].head-feed[pf].len)<screeny))		//moves the black
					m_screen[feed[pf].xpos][feed[pf].head-feed[pf].len].color=0;
			}
		}
	}
}


void disp_buffer(){			//this looks for differnces between the buffer and the main screen matrix
	for(int cb1=0;cb1<screenx;cb1++){
		for(int cb2=0;cb2<screeny;cb2++){
			if(m_screen[cb1][cb2].color!=b_screen[cb1][cb2].color){
				setcolor(m_screen[cb1][cb2].color);
				gotoxy(cb1,cb2);
				cout<<char(m_screen[cb1][cb2].code)<<flush;
			}
			else if((m_screen[cb1][cb2].code!=b_screen[cb1][cb2].code)&(m_screen[cb1][cb2].color+b_screen[cb1][cb2].color!=0)){
				setcolor(m_screen[cb1][cb2].color);
				gotoxy(cb1,cb2);
				cout<<char(m_screen[cb1][cb2].code)<<flush;
			}
		}		//it then draws those changes to the screen
	}
	b_screen=m_screen;		//updates the buffer
	if(debug){
		int temp=0;
		for(int debug=0;debug<feed.size();debug++){
			if(feed[debug].active)
				temp++;
		}
		setcolor(15);
		gotoxy(0,0);
		cout<<temp<<" "<<endl<<feed.size()<<" "<<flush;		//draws the debug on the screen if the user has toggled it
	}
}

void create_livefeed(){
	lf.active=true;
	lf.xpos=rnd(screenx);
	lf.lght=14;
	lf.drk=6;
	lf.input.clear();
}

void handle_livefeed(){		//handles all user inputs
	if((user_flush==8)&(lf.input.size()>0)){ //backspace
		m_screen[lf.xpos][lf.input.size()].color=0;
		lf.input.pop_back();
	}
	else if(user_flush==13){
		lf.active=false;
		string s_input;		
		if(lf.input.empty()){
			feed_input.clear();
			feed_input.push_back(-1);
			m_screen[lf.xpos][0].color=0;
		}
		else{		
			for(int t1=0;t1<lf.input.size(); t1++)  s_input+=char(lf.input[t1]);
			if(s_input.find("color")!=string::npos){
				create_drk=atoi(s_input.substr(6,s_input.size()-6).c_str());
				create_lght=create_drk+8;
			}
			else if(s_input.find("delay")!=string::npos){
				create_delay=atoi(s_input.substr(6,s_input.size()-6).c_str());
			}
			else if(s_input.find("density")!=string::npos){
				create_density=atoi(s_input.substr(8,s_input.size()-8).c_str());
			}
			else if((s_input.find("exit")!=string::npos)||(s_input.find("quit")!=string::npos)){
				quit=true;
				create_density=0;
			}
			else if(s_input.find("debug")!=string::npos){
				if(debug){
					debug=false;
					gotoxy(0,0);
					cout<<"   "<<endl<<"    "<<flush;
				}
				else debug=true;
			}
			else if(s_input.find("char")!=string::npos){
				feed_input.clear();
				feed_input.push_back(atoi(s_input.substr(5,s_input.size()-5).c_str()));
			}
			else feed_input=lf.input;
			m_screen[lf.xpos][0].color=0;
			create_feed(lf.xpos,lf.input.size(),2,lf.input.size(),14,6,lf.input);
		}
	}
	else{
		lf.input.push_back(user_flush);
		m_screen[lf.xpos][lf.input.size()-1].code=user_flush;
		m_screen[lf.xpos][lf.input.size()-1].color=lf.lght;
	}
}