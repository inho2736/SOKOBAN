#include<stdio.h>
#include <termios.h> //
#include<stdlib.h>//
#include<time.h> // 시간을 잴 때 사용
char map[5][30][30] = {0};// 맵 입력받기
char stage[5][30][30] = {0};//맵 출력하기
char undo[6][5][30][30] = {0};// 커서 이동전 맵 저장> undo기능
char name[10],r_name[5][6][11];// 이름입력받기, 랭킹에 사용되는 배열
int ranking[5][6];//랭킹에 사용되는 배열
int l1 = 0, p=0, q = 0, x1, y1, u = 0, u_count = 0;// ㅣ1은 레벨을 나타내는 변수, p
int cx[5]={13,8,15,9,15};//커서의 시작위치의 가로축 저장
int cy[5]={9,5,2,11,8};//커서의 시작위치의 세로축 저장
int count1=0,count2=0;
float time_gap, loadTime_gap;//
time_t startTime = 0, endTime = 0, pre_endTime = 0;
int getch(void);
void mapscan(void);
void mapopen(void);
void stagescan(void);
int getch(void)//엔터없이 방향키와 옵션을 입력받는데 쓰는 getch함수
{
	int ch;
	struct termios buf;
    struct termios save;
    tcgetattr(0, &save);
    buf = save;
    buf.c_lflag&=~(ICANON|ECHO);
    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] = 0;
    tcsetattr(0, TCSAFLUSH, &buf);
    ch= getchar();
    tcsetattr(0, TCSAFLUSH, &save);
    return ch;
}
void mapscan(void)//맵파일에서 맵을 읽어와서 배열에 저장
{
	FILE *p;
    p=fopen("map.txt","rt");
   char tmp;
   int l=-1,x=0,y=0;//?

   while(fscanf(p,"%c",&tmp) != EOF){

    if(tmp=='m'){// 맵파일에서 map이라는 글자를 만났을때마다 레벨을변경하며  x와 y 좌표를 저장
         l++;
         x = 0;
         y = 0;
        continue;
       }
     if(tmp=='a' || tmp=='p'){
        continue;
       }
     if(tmp=='\n'){
         y++; // 다음줄로 넘어갈때 y값 증가
         x = 0;
       }
     if(tmp=='e')// end 를 만나면 종료
        break;

      map[l][y][x]=tmp;
      x++;//하나 저장할때마다 x값 증가
    }
    fclose(p);
}

void mapopen(void)// 입력받은 맵을 출력함
{
for(int i=0;i<5;i++){
      for(int j=0;j<30;j++){
         for(int k=0;k<30;k++){
            printf("%c",map[i][j][k]);
		 }
	  }
   }
}
void stagescan(void)
	{
		for(int i=0;i<5;i++){
			for(int j=0;j<30;j++){
				for(int k=0;k<30;k++){
					stage[i][j][k] = map[i][j][k];
			        if(map[i][j][k]=='O')// 보관장소를 기억함
						count1++;
					if(map[i][j][k]=='$')// 상자(금)의 위치를 기억함
						count2++;
		 }
	  }
}

}




