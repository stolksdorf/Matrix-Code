#include<iostream>
#include<string>
#include<time.h>
#include<vector>
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

//Matrix Buffer Vectors
vector< vector<int> > m_code;
vector< vector<int> > m_color;
vector< vector<int> > b_code;
vector< vector<int> > b_color;



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
};
vector<feed_struct> feed;



//Live Feed Vectors
struct live_feed_struct{
	int xpos;
	int mod;
	int lght;
	int drk;
	vector<int> input;
	bool active;
}lf;

//Create feed variables
int create_xpos; 
int create_len; 
int create_delay; 
int create_head; 
int create_drk; 
int create_lght;
vector<int> create_input;

int user_flush;

int screenx=77;
int screeny=48;
vector<int> feed_input;


void core();
void create_feed(int create_xpos, int create_len, int create_delay, int create_head, int create_lght, int create_drk, const vector<int>&);
void process_feed();
void handle_livefeed();
void disp_buffer();
void disptext(int offsetx,int offsety, string text, int textdelay, int color);
void screen_fix();
void create_livefeed();



int main(){

	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	srand((unsigned)time(NULL));

	lf.active=false;					//Sets the live feed input as off

	disptext(0,0,"Welcome to the Standard Operator Training Program",20,10);
	Sleep(200);
	disptext(0,1,"Press any key to continue...",20,10);
	while(!kbhit());
	user_flush=getch();
	system("cls");

//	screen_fix();						//Gets the dimensions of the screen
	screenx++;
	screeny++;

	m_code.resize(screenx);
	m_color.resize(screenx);
	b_code.resize(screenx);
	b_color.resize(screenx);
	for(int t1=0;t1<screenx;t1++){			//init's matrix of code, and its buffer
		m_code[t1].resize(screeny);
		m_color[t1].resize(screeny);
		b_code[t1].resize(screeny);
		b_color[t1].resize(screeny);
		for(int t2=0;t2<screeny;t2++){
			m_code[t1][t2]=32;
			m_color[t1][t2]=0;
			b_code[t1][t2]=32;
			b_color[t1][t2]=0;
		}
	}

	feed_input.push_back(-1);			//Sets the matrix code as random

	system("cls");
	core();

	return(0);
}

void core(){
	int temp_feed_count=1;
	while(1){
		if(kbhit()){
			user_flush=getch();
			if((user_flush==13)&&(!lf.active))
				create_livefeed();
			else if(lf.active)
				handle_livefeed();
		}
		if(lf.active){
			m_code[lf.xpos][lf.input.size()]=33+rand()%221;
			m_color[lf.xpos][lf.input.size()]=15;
		}
		Sleep(5);

		temp_feed_count--;
		if(temp_feed_count==0){
			temp_feed_count=1;
			create_feed(-1,-1,1,-1,10,2,feed_input);
		}	
		process_feed();	
		disp_buffer();
	}
}

void create_feed(int feed_pos, int feed_len, int feed_delay, int feed_head, int feed_lcol, int feed_dcol, const vector<int>&temp_feed_code){
	vector<int> feed_code=temp_feed_code;
	if(feed_pos==-1){
		while(1){
			bool xpos_check=true;
			feed_pos=rand()%screenx;
			if((feed_pos==lf.xpos)&(lf.active))	continue;
			for(int cfx=0;cfx<feed.size();cfx++){
				if((feed[cfx].xpos==feed_pos)&&((feed[cfx].head-feed[cfx].len))<7)
					xpos_check=false;
			}
			if(xpos_check)	break;
		}
	}
	if(feed_len==-1)
		feed_len=rand()%20+10;				//handles random parameters
	if(feed_delay==-1)
		feed_delay=(rand()%4);
	if(feed_dcol==-1)
		feed_dcol=rand()%5+1;
	if(feed_lcol==-1)
		feed_lcol=(feed_dcol+8)%16;
	if(feed_code[0]==-1){
		feed_code.clear();
		for(int t1=0;t1<screeny;t1++)
			feed_code.push_back(33+rand()%221);			//creates a random string of code;
	}
	bool old_feed=false;
	int cf1;
	for(cf1=0;cf1<feed.size();cf1++){
		if(!feed[cf1].active){
			old_feed=true;
			break;
		}
	}
	feed_struct increase_feed;
	if(!old_feed){
		cf1=feed.size();
		feed.push_back(increase_feed);
	}
	feed[cf1].active=true;
	feed[cf1].xpos=feed_pos;
	feed[cf1].len=feed_len;
	feed[cf1].delay=feed_delay;
	feed[cf1].count=feed_delay;
	feed[cf1].head=feed_head;
	feed[cf1].lght=feed_lcol;
	feed[cf1].drk=feed_dcol;
	feed[cf1].code_pos=0;
	feed[cf1].code=feed_code;
}

void process_feed(){
	for(int pf=0;pf<feed.size();pf++){
		if(feed[pf].active){
			feed[pf].count--;
			if(feed[pf].count==0){
				feed[pf].count=feed[pf].delay;
				feed[pf].head++;
				feed[pf].code_pos++;
				if(feed[pf].code_pos==feed[pf].code.size())
					feed[pf].code_pos=0;
				if((feed[pf].head-feed[pf].len)>=screeny){
					feed[pf].active=false;
					continue;
				}
				if(feed[pf].head<screeny){
					m_color[feed[pf].xpos][feed[pf].head]=15;
					m_code[feed[pf].xpos][feed[pf].head]=feed[pf].code[feed[pf].code_pos];
				}

				if(((feed[pf].head-1)>=0)&&((feed[pf].head-1)<screeny))
					m_color[feed[pf].xpos][feed[pf].head-1]=feed[pf].lght;
	
				if(((feed[pf].head-feed[pf].len*.6)>=0)&&((feed[pf].head-feed[pf].len*.6)<screeny))
					m_color[feed[pf].xpos][feed[pf].head-feed[pf].len*.6]=feed[pf].drk;

				if(((feed[pf].head-feed[pf].len)>=0)&&((feed[pf].head-feed[pf].len)<screeny))
					m_color[feed[pf].xpos][feed[pf].head-feed[pf].len]=0;
			}
		}
	}
}

void disptext(int offsetx,int offsety, string text, int textdelay, int color){		
	HANDLE  hConsole;									
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for(int dt1=0;dt1<text.size();dt1++){
		gotoxy(offsetx+dt1,offsety);					//Displays text letter by letter with a display
		SetConsoleTextAttribute(hConsole,color);
		cout<<text[dt1]<<flush;
		Sleep(textdelay);
	}
}

void screen_fix(){
	disptext(0,0,"Move the cursor using the arrows keys to the bottom right corner of screen",10,10);
	disptext(0,1,"Press enter when the cursor has reached this point",10,10);
	gotoxy(screenx,screeny);
	cout<<char(32)<<flush;
	while(1){
		if(kbhit()){
			gotoxy(screenx,screeny);
			cout<<" "<<flush;
			user_flush=getch();
			if(user_flush==72)
				screeny--;
			else if(user_flush==80)
				screeny++;
			else if(user_flush==77)
				screenx++;
			else if(user_flush==75)
				screenx--;
			else if(user_flush==13)
				break;
			gotoxy(screenx,screeny);
			cout<<char(32)<<flush;
		}
	}
}
	
void disp_buffer(){
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for(int cb1=0;cb1<screenx;cb1++){
		for(int cb2=0;cb2<screeny;cb2++){
			if(m_color[cb1][cb2]!=b_color[cb1][cb2]){
				SetConsoleTextAttribute(hConsole, m_color[cb1][cb2]);
				gotoxy(cb1,cb2);
				cout<<char(m_code[cb1][cb2])<<flush;
			}
			else if((m_code[cb1][cb2]!=b_code[cb1][cb2])&(m_color[cb1][cb2]+b_color[cb1][cb2]!=0)){
				SetConsoleTextAttribute(hConsole, m_color[cb1][cb2]);
				gotoxy(cb1,cb2);
				cout<<char(m_code[cb1][cb2])<<flush;
			}
		}
	}
	b_color=m_color;
	b_code=m_code;

	int temp=0;
	for(int debug=0;debug<feed.size();debug++){
		if(feed[debug].active)
			temp++;
	}
	SetConsoleTextAttribute(hConsole, 15);
	gotoxy(0,0);
	cout<<temp<<endl<<feed.size()<<flush;
}

void create_livefeed(){
	lf.active=true;
	lf.xpos=rand()%screenx;
	lf.lght=14;
	lf.drk=6;
	lf.mod=0;
	lf.input.clear();
}

void handle_livefeed(){
	if((user_flush==8)&(lf.input.size()>0)){ //backspace
		m_color[lf.xpos][lf.input.size()]=0;
		lf.input.pop_back();
	}
	else if(user_flush==13){
		lf.active=false;
		if(lf.input.empty()){
			feed_input.clear();
			feed_input.push_back(-1);
			m_color[lf.xpos][0]=0;
		}
		else{
			feed_input=lf.input;
			m_color[lf.xpos][0]=0;
			create_feed(lf.xpos,lf.input.size(),2,lf.input.size(),14,6,lf.input);
		}
	}
	else{
		lf.input.push_back(user_flush);
		m_code[lf.xpos][lf.input.size()-1]=user_flush;
		m_color[lf.xpos][lf.input.size()-1]=lf.lght;
	}
}