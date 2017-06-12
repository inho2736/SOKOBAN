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
void move(void);
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
void move(void)
{
    x1=cx[p], y1=cy[p]; // 처음 커서위치를 배열에서 불러옴

		char k;// 커서이동과 옵션 입력받는 변수
    begin_undo();//

		while (1){

			k = getch();

			if (k=='h'){

				if (stage[l1][y1][x1-1]=='#')
					continue;// 벽이 옆일경무 무시

				else if (stage[l1][y1][x1-1]==' '){// 빈공간이 옆일경우

					if (map[l1][y1][x1]=='O'){ // 창고지기의 자리가 보관장소였을때
						stage[l1][y1][x1-1]='@';// 창고지기가 왼쪽으로 이동
						stage[l1][y1][x1]='O';// 원래 창고지기의 자리에 보관장소 다시저장
						x1--;// 창고지기의 가로좌표축 -1
						print();}// 새맵 출력
					else {// 창고지기의 자리가 보관장소가 아니면

					stage[l1][y1][x1-1]='@'; // 왼쪽으로 창고지기 이동
					stage[l1][y1][x1]=' ';// 원래자리에 빈칸 다시저장
					x1--;// 창고지기의 가로좌표축 -1
					print();}//새맵 출력
				}

				else if (stage[l1][y1][x1-1]=='$'){// 창고지기 왼쪽이 상자(금)이면

					if (stage[l1][y1][x1-2]==' '){// 그 왼쪽이 빈칸이면

						stage[l1][y1][x1-2]='$';
						stage[l1][y1][x1-1]='@';
						stage[l1][y1][x1]=' ';
						x1--;
					print();
				} //' ' < '$' < '@' 이런식으로 이동

					else if (stage[l1][y1][x1-2]=='O'){// 그왼쪽이 보관상자이면

						stage[l1][y1][x1-2]='$';//
						stage[l1][y1][x1-1]='@';
						stage[l1][y1][x1]=map[l1][y1][x1];
						x1--;
						//위와같이이동하되 원래 창고지기의 자리에 맵에 원래 저장된 보관상자 저장
					print();
					}

					else
						continue;// 그 왼쪽이 벽일경우 무시
				}
				else {// 그 왼쪽이 보관장소일 경우

					stage[l1][y1][x1-1]='@';// 보관장소자리에 창고지기 두기
					stage[l1][y1][x1]=map[l1][y1][x1];// 원래 창고지기자리에 원래 맵 출력
					x1--;
					print();
				}}


			if (k=='l'){// 오른쪽으로 이동시(위와 동일)

				if (stage[l1][y1][x1+1]=='#')
					continue;

				else if (stage[l1][y1][x1+1]==' '){


					if (map[l1][y1][x1]=='O'){
						stage[l1][y1][x1+1]='@';
						stage[l1][y1][x1]='O';
						x1++;
						print();}
					else

					{stage[l1][y1][x1+1]='@';
					stage[l1][y1][x1]=' ';
					x1++;
					print();}
				}

				else if (stage[l1][y1][x1+1]=='$'){

					if (stage[l1][y1][x1+2]==' '){

						stage[l1][y1][x1+2]='$';
						stage[l1][y1][x1+1]='@';
						stage[l1][y1][x1]=' ';
						x1++;
					print();
					} //' ' < '$' < '@'


					else if (stage[l1][y1][x1+2]=='O'){

						stage[l1][y1][x1+2]='$';
						stage[l1][y1][x1+1]='@';
						stage[l1][y1][x1]=map[l1][y1][x1];
						x1++;
						print();
					}

				}
				else {				stage[l1][y1][x1+1]='@';
					stage[l1][y1][x1]=map[l1][y1][x1];
					x1++;

					print();
				}

			}

			if (k=='k'){// 위쪽으로 이동시(위와 동일)

				if (stage[l1][y1-1][x1]=='#')
					continue;

				else if (stage[l1][y1-1][x1]==' '){

					if (map[l1][y1][x1]=='O'){
						stage[l1][y1-1][x1]='@';
						stage[l1][y1][x1]='O';
						y1--;
						print();}
					else

					{stage[l1][y1-1][x1]='@';
					stage[l1][y1][x1]=' ';
					y1--;
					print();}}

				else if (stage[l1][y1-1][x1]=='$'){

					if (stage[l1][y1-2][x1]==' '){

						stage[l1][y1-2][x1]='$';
						stage[l1][y1-1][x1]='@';
						stage[l1][y1][x1]=' ';
						y1--;

					print();
					} //' ' < '$' < '@'

					else if (stage[l1][y1-2][x1]=='O'){

						stage[l1][y1-2][x1]='$';
						stage[l1][y1-1][x1]='@';
						stage[l1][y1][x1]=map[l1][y1][x1];
						y1--;
					print();
					}

					else
						continue;
				}
				else if (stage[l1][y1-1][x1]=='O'){
					stage[l1][y1-1][x1]='@';
					stage[l1][y1][x1]=map[l1][y1][x1];
					y1--;
					print();}

			}
			if (k=='j'){// 아랫쪽으로 이동시(위와 동일)

				if (stage[l1][y1+1][x1]=='#')
					continue;

				else if (stage[l1][y1+1][x1]==' '){

					if (map[l1][y1][x1]=='O'){
						stage[l1][y1+1][x1]='@';
						stage[l1][y1][x1]='O';
						y1++;
						print();}
					else

					{stage[l1][y1+1][x1]='@';
					stage[l1][y1][x1]=' ';
					y1++;
					print();}}

				else if (stage[l1][y1+1][x1]=='$'){

					if (stage[l1][y1+2][x1]==' '){

						stage[l1][y1+2][x1]='$';
						stage[l1][y1+1][x1]='@';
						stage[l1][y1][x1]=' ';
						y1++;
					print();
					} //' ' < '$' < '@'

					else if (stage[l1][y1+2][x1]=='O'){

						stage[l1][y1+2][x1]='$';
						stage[l1][y1+1][x1]='@';
						stage[l1][y1][x1]=map[l1][y1][x1];
						y1++;
					print();

					}

					else
						continue;
				}
				else if (stage[l1][y1+1][x1]=='O'){
					stage[l1][y1+1][x1]='@';
					stage[l1][y1][x1]=map[l1][y1][x1];
					y1++;
					print();
				}
			}



