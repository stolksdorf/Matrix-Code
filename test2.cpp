#include<iostream>
#include<string>
#include<time.h>
#include<vector>
#include<windows.h>
#include<winuser.h>
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
	vector<bool> active;
	vector<int> xpos; 
	vector<int> len; 
	vector<int> delay; 
	vector<int> count;
	vector<int> head; 
	vector<int> drk; 
	vector<int> lght;
	vector<int> input;
}feed;

//Live Feed Vectors
struct live_feed_struct{
	int pos;
	int mod;
	int lght;
	int drk;
	vector<int> input;
	bool active;
}lf;

//Create feed variables
struct create_feed_struct{
	int xpos; 
	int len; 
	int delay; 
	int head; 
	int drk; 
	int lght;
	vector<int> input;
}create;

int user_flush;

int screenx=77;
int screeny=48;


void core();
void create_feed(int create.xpos, create.len, int feed_delay, int feed_head, int feed_lcol, int feed_dcol, const vector<int>&);
void process_feed();
void handle_livefeed();
void disp_buffer();
void copy_buffer();
void disptext(int offsetx,int offsety, string text, int textdelay, int color);
void screen_fix();
void create_livefeed();
void cut_feed();


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

	feed. feed_vector.push_back(-1);			//Sets the matrix code as random

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
			m_code[lf.pos][lf.input.size()]=33+rand()%221;
			m_color[lf.pos][lf.input.size()]=15;
		}
		Sleep(5);

		temp_feed_count--;
		if(temp_feed_count==0){
			temp_feed_count=7;
			create_feed(-1,-1,2,-1,10,2,feed_vector);
		}	
		process_feed();	
		disp_buffer();
		copy_buffer();
	}
}

void create_feed(int feed_pos, int feed_len, int feed_delay, int feed_head, int feed_lcol, int feed_dcol, const vector<int>&feed_code){
	if(feed_pos==-1){
		while(1){
			feed_pos=rand()%screenx;
			if(!lf.active)
				break;
			if(feed_pos!=lf.pos)
				break;
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

	bool old_feed=false;
	int cf1;

	for(cf1=0;cf1<feed.size();cf1++){
		if(feed[cf1]==false){
			old_feed=true;
			break;
		}
	}

	
	if(old_feed){
		feed[cf1]=true;
		init_pos[cf1]=feed_pos;
		stream_len[cf1]=feed_len;
		delay[cf1]=feed_delay;
		count[cf1]=feed_delay;
		head_pos[cf1]=feed_head;
		color_lght[cf1]=feed_lcol;
		color_drk[cf1]=feed_dcol;
	}
	else{
		feed.push_back(true);
		init_pos.push_back(feed_pos);
		stream_len.push_back(feed_len);		
		delay.push_back(feed_delay);
		count.push_back(feed_delay);
		head_pos.push_back(feed_head);
		color_lght.push_back(feed_lcol);
		color_drk.push_back(feed_dcol);
	}	

	if(feed_code[0]==-1){
		for(int t1=0;t1<screeny;t1++)
			m_code[init_pos[init_pos.size()-1]][t1]=33+rand()%221;			//creates a random string of code;
	}
	else{
		int t2=0;
		for(int t1=0;t1<screeny;t1++){
			m_code[init_pos[init_pos.size()-1]][t1]=feed_code[t2];
			t2++;
			if(t2==feed_code.size())
				t2=0;
		}
	}
}

void process_feed(){
	int temp_pos;
	for(int pf=0;pf<feed.size();pf++){

//	/*
		if(feed[pf]){
			count[pf]--;
			if(count[pf]==0){								//If the delay on the feed is up it moves it
				count[pf]=delay[pf];
				head_pos[pf]++;
				temp_pos=head_pos[pf];
				if((head_pos[pf]-stream_len[pf])-1>=screeny){
						feed[pf]=false;
						break;
					}
				if(head_pos[pf]<screeny)
					m_color[init_pos[pf]][head_pos[pf]]=15;			//draws the leading white character
				while(1){
					temp_pos--;
					if(temp_pos==-1)
						break;
					if(temp_pos<screeny){
						if((head_pos[pf]-temp_pos)>(stream_len[pf])){		//Draws black at the end of the stream
							m_color[init_pos[pf]][temp_pos]=0;
							break;
						}
						if((head_pos[pf]-temp_pos)>(stream_len[pf]*.6))		//Draws the dark color on the last part of the stream
							m_color[init_pos[pf]][temp_pos]=color_drk[pf];
						
						if((head_pos[pf]-temp_pos)<(stream_len[pf]*.6))		//Draws the light color on the front of the stream
							m_color[init_pos[pf]][temp_pos]=color_lght[pf];
					}
				}
			}
		}
//		*/

		/*
		if(feed[pf]){
			count[pf]--;
			if(count[pf]==0){
				count[pf]=delay[pf];
				head_pos[pf]++;
				if(head_pos[pf]<screeny)
					m_color[init_pos[pf]][head_pos[pf]]=15;
				if((head_pos[pf]-stream_len[pf]*.6)<0){
					m_color[init_pos[pf]][head_pos[pf]-1]=color_lght[pf];
					continue;
				}
				else if((head_pos[pf]-stream_len[pf]*.6)<screeny)
					m_color[init_pos[pf]][head_pos[pf]-stream_len[pf]*.6]=color_drk[pf];
				if((head_pos[pf]-stream_len[pf])<0)
					m_color[init_pos[pf]][0]=color_drk[pf];
				else if((head_pos[pf]-stream_len[pf])<screeny)
					m_color[init_pos[pf]][head_pos[pf]-stream_len[pf]]=0;
				else
					feed[pf]=false;
			}
		}
//		*/



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
	int temp=0;
	for(int debug=0;debug<feed.size();debug++){
		if(feed[debug])
			temp++;
	}
	SetConsoleTextAttribute(hConsole, 15);
	gotoxy(0,0);
	cout<<temp<<endl<<feed.size()<<flush;
}
void cut_feed(){
	for(int t=0;t<feed.size();t++){
		if(feed[t]){
			color_lght[t]=7;
			color_drk[t]=8;

		}
	}
}

void copy_buffer(){
	b_color=m_color;
	b_code=m_code;
}

void create_livefeed(){
	lf.active=true;
	lf.pos=rand()%screenx;
	lf.lght=14;
	lf.drk=6;
	lf.mod=0;
	lf.input.clear();
}

void handle_livefeed(){
	if(user_flush==31){ 
		lf.mod++;
		if(lf.mod>5)
			lf.mod=0;
	}
	else if((user_flush==8)&(lf.input.size()>0)){ //backspace
		m_color[lf.pos][lf.input.size()]=0;
		lf.input.pop_back();
	}
	else if(user_flush==13){
		lf.active=false;
		if(lf.input.empty()){
			feed_vector.clear();
			feed_vector.push_back(-1);
			m_color[lf.pos][0]=0;
		}
		else{
			feed_vector=lf.input;
			create_feed(lf.pos,lf.input.size(),2,lf.input.size(),14,6,lf.input);
		}
	}
	else{
		lf.input.push_back(user_flush);
		m_code[lf.pos][lf.input.size()-1]=user_flush;
		m_color[lf.pos][lf.input.size()-1]=lf.lght;
	}
}