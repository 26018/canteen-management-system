#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <io.h>
#define FOOD 21   ////���������ݿռ�
#define FIGURE 10 ////�������ݿռ�

#define NUM 5  //���Ԥ������
#define PATHLENGTH 100

typedef struct Node//(����)  ////��Ʒ�ṹ��
{
	int Sequence;  //���
	char name[FOOD]; 
	char price[FIGURE];
	char Inventory[FIGURE]; //���
	char Sell[FIGURE];
	char num[FIGURE]; ///����
	struct Node* next;
}base;

typedef struct xx   ///////��ȡ�ļ��Ľṹ��
{
	int num;
	char name[50];
	struct xx *next;
	struct xx *front;
}file;

typedef struct yy     //////����·���ṹ��
{
	char Systempath[PATHLENGTH];
	char Filepath[PATHLENGTH];
	char Empty[PATHLENGTH];
	char str[2];
}Path;

typedef struct Data
{
	char year[5];
	char month[3];
	char day[3];
	char data_day[20];
	char data_mon[20];
}DATA;

/*****************************************************************
��������
*/
template<typename T>
T Del(T L)
{
	T pre = L, p = L->next;
	while (p != NULL)
	{
		free(pre);
		pre = p;
		p = pre->next;
	}
	free(pre);
	return L;
}

void Getdata(DATA &data)  //////���ʱ�� 
{
	memset(data.data_day,'\0',sizeof(data.data_day));
	strcat(data.data_day,data.year);
	strcat(data.data_day,"��");
    strcat(data.data_day,data.month);
	strcat(data.data_day,"��");
    strcat(data.data_day,data.day);
	strcat(data.data_day,"��");

	memset(data.data_mon,'\0',sizeof(data.data_mon));
	strcat(data.data_mon,data.year);
	strcat(data.data_mon,"��");
    strcat(data.data_mon,data.month);
	strcat(data.data_mon,"��");
}

void Time(DATA &data)   ////��ȡ��ǰʱ�� 
{
	time_t timep;
    struct tm *p;
    time (&timep);
    p=gmtime(&timep);
    
    sprintf(data.year,"%4d",1900+p->tm_year);
    sprintf(data.month,"%02d",1+p->tm_mon);
    sprintf(data.day,"%02d",p->tm_mday);
    Getdata(data); 
}

void InitPath(Path *&path)
{
	path = (Path*)malloc(sizeof(Path));
	memset(path,'\0',sizeof(Path));
}

void InitList(base*& p);
void DestroyList(base*& L);
bool Load(base*& head, char p[],int mode);
void Write_in(base*& head, char p[],char mode[],int num);
int Disp(base* head, int x,int num);
int GetElem(base*& head, int i, int x);
void Buy(base*& head);
void Sort(char x[],int mode,int Max);
void Recommend(char path[],char p[]);
void Initstr(base *&head);
void Goto(base *&head,int i,int x);
void new_file(char datapath[]);
void printDir( const char* path,file *&x,int mode);
bool Modify(int x,char datapath[]);

//ȥ���ļ����ظ���Ԫ��
void fun(char path[])
{
	base *head;
	InitList(head);
	Load(head,path,1);

	base *xp = head->next;
	int numb=0,total=0;
	while(xp->next != NULL)
	{
		strcat(xp->num,"1");
		numb+=atoi(xp->num);
		total+=atoi(xp->price);
		xp = xp->next;
	}
	base *i=head->next,*j;
	while(i->next!=NULL)
	{
		j=i;
		while(j->next->next!=NULL)
		{
			if(strcmp(j->next->name,i->name)==0)
			{
				base *p = j->next;
				j->next = j->next->next;
				free(p); 
				_itoa(atoi(i->num) + 1, i->num, FIGURE);
				continue;
			} 
			j=j->next;
		}
		i = i->next;
	}
	printf("\t\t\t\t\t\t\t\t\t\t____________________________________________________________________\n\n");
	Disp(head,0,3);
	printf("\t\t\t\t\t\t\t\t\t\t��%d����Ʒ,�ܶ�%dԪ\n",--numb,total);
	printf("\n\t\t\t\t\t\t\t\t\t\t�����������");
	Del(head); 
	i= j = NULL; 
	getch();
 } 


void Tip(int num)
{
	if(num != 3)
		printf("\t\t\t\t\t\t\t\t\t\t________________________________________________________\n\n");
	if(num == 3)
		printf("\t\t\t\t\t\t\t\t\t\t���\t%-21s%-10s%-10s%-10s\t%s\n\n","��Ʒ","�۸�","ʣ��","������","����");
	if(num != 1 && num!=3)
		printf("\t\t\t\t\t\t\t\t\t\t���\t%-21s%-10s%-10s%-10s\n\n","��Ʒ","�۸�","ʣ��","������");
}

/*****************************************************************
��ʼ��   ����һ�����
*/
void InitList(base *&p)      /////��ʼ����Ʒ�ṹ��
{
	p = (base*)malloc(sizeof(base));
	memset(p, '\0', sizeof(base));
	p->next = NULL;
}
void initArray(char p[],int length)  ////�������
{
	for(int i=strlen(p);i<length;i++)
		p[i] = ' ';
	p[length-1] = '\0';
}

void Initstr(base *&head)  //�ṹ�����
{
	initArray(head->name,FOOD);
	initArray(head->price,FIGURE);
	initArray(head->Inventory,FIGURE);
	initArray(head->Sell,FIGURE);
	initArray(head->num,FIGURE);
}
void Initfile(file *&p)  /////�ļ���ʼ��
{
	p = (file*)malloc(sizeof(file));
	memset(p->name,'\0',sizeof(p->name));
	p->next = NULL;p->front = NULL;
}


void Free(file *&L)
{
	file* pre = L, *p = L->next;
	L->front->next = NULL;
	while (p = NULL)
	{
		free(pre);
		pre = p;
		p = pre->next;
	}
	free(pre);
}

/*****************************************************************
˵�������ļ��ڶ�ȡ����
����1����Ʒ����
����2���ļ�·��
����3��ģʽ��0ģʽ���ظ��� 2ģʽ����ʾ����
*/
bool Load(base *&head, char p[],int mode)
{
	int i = 0;
	base* s = head;
	FILE* fp = fopen(p, "r");
	if (fp == NULL)
	{
		system("cls");
		if(mode != 2)
		{
			printf("\n\n\t\t\t\t\t\t\t\t\t\t\t��ȡ���ݴ���\n\n\t\t\t\t\t\t\t\t\t\t\tû��%s����ļ�\n\n\t\t\t\t\t\t\t\t\t\t\t2����˳�",p);
			Sleep(2000);
		}
		return false;
	}
	do
	{
		i++;
		InitList(s->next);
		s = s->next;
		s->Sequence = i;
		if(mode == 0)
			fgets(s->num,FIGURE,fp);
		fgets(s->name, FOOD, fp);
		fgets(s->price, FIGURE, fp);
		fgets(s->Inventory, FIGURE, fp);
		fgets(s->Sell, FIGURE, fp);
		fgetc(fp);
	} while (strlen(s->name) == FOOD-1);
	InitList(s->next);
	fclose(fp);
	return true;
}

/*****************************************************************
�������е�����д�뵽�ļ���
����1����Ʒ����
����2���ļ�·��
����3��ģʽ����ָ��ģʽ��д���ļ�
����4��1ģʽд�����
*/
void Write_in(base*& head, char p[],char mode[],int num)
{
	base* s = head->next;
	FILE* fp = fopen(p, mode);
	if (fp == NULL)
	{
		printf("\t\t\t\t\t\t\t\t\t\tд������ʧ��\n\n\t\t\t\t\t\t\t\t\t\t��������˳�����");
		getch();
	}
	while (strlen(s->name) == FOOD-1)
	{
		fputs(s->name, fp);
		fputs(s->price, fp);
		fputs(s->Inventory, fp);
		fputs(s->Sell, fp);
		if(num == 1)
			fputs(s->num,fp);
		fputc('\n', fp);
		s = s->next;
	}
	fclose(fp);
}

/*****************************************************************
��ʾ����������Ԫ�ظ���
����1����Ʒ����
����2��ģʽ��1ģʽ���Խ����ظ���������ʾ����
*/
int Disp(base* head, int x,int num)
{
	base* p = head;p = p->next;
	int i = 0;
	if (x != 1)
	{
		if(num == 1 || num == 3)
			Tip(num);
		else
		Tip(0);
	}
	while (p->next != NULL && strlen(p->name) == FOOD-1)
	{
		i++; 
		if (x != 1)
		{
			if(num == 1 || num == 3)
				printf("\t\t\t\t\t\t\t\t\t\t%d\t%-21s%-10s%-10s%-10s\t%s\n\n",i,p->name, p->price, p->Inventory, p->Sell,p->num);
			else
				printf("\t\t\t\t\t\t\t\t\t\t%d\t%-21s%-10s%-10s%-10s\n\n",i,p->name, p->price, p->Inventory, p->Sell);
			Sleep(17);
		}
		p = p->next;
	}
	return i;
}


//�˵�д��
void bill_check(char data_arr[],base *&p,char sys_path[])
{
	char path[40];
	memset(path,'\0',sizeof(path));
	strcat(path,sys_path);
	strcat(path,data_arr);
	FILE *fp = fopen(path,"a");   
	fputs(p->name, fp);
	fputs(p->price, fp);
	fputs(p->Inventory, fp);
	fputs(p->Sell, fp);
	fputc('\n', fp);
	fclose(fp);
}


/*****************************************************************
����1����Ʒ����
����2����ֵ��λ��
����3��
		0.ģʽ����ӡָ��λ��Ԫ����Ϣ
		1.ģʽ������ָ��Ԫ�ؼ۸�
		2.ģʽ���޸�ָ��Ԫ�ؿ��
*/
int GetElem(base*& head, int i, int mode)    //�����Ա��е�ĳ��λ�õ�Ԫ��ֵ 
{
	int j = 0;
	base* p = head->next;
	if (i <= 0)
		return false;
	while (j < i - 1 && p != NULL)
	{
		j++;
		p = p->next;
	}
	if (p == NULL)
		return false;
	else
	{
		if (mode == 0)       //0ģʽ����ӡ��Ϣ
		{
			printf("\t\t\t\t\t\t\t\t\t\t%d\t%-21s%-10s%-10s%-10s\n",i,p->name, p->price, p->Inventory, p->Sell);
			return true;
		}
		else if (mode == 1)  //1ģʽ�����ؼ۸�
			return atoi(p->price);
		else if (mode == 2)   //2ģʽ���޸Ŀ��
		{
			int m = atoi(p->Inventory) - 1;
			if(m<0)  ////�������������㣬�򷵻�
				return -1;
			_itoa(m, p->Inventory, FIGURE); ///�������-1
			initArray(p->Inventory,FIGURE); 
			_itoa(atoi(p->Sell) + 1, p->Sell, FIGURE);	///��������+1
			initArray(p->Sell,FIGURE);
			
			////���ﱳ��
			DATA data;
			Time(data);
			bill_check(data.data_day,p,"data_day\\");
			bill_check(data.data_mon,p,"data_mon\\");
			
			FILE *fp2 = fopen("temp.txt","a");   
			fputs(p->name, fp2);
			fputs(p->price, fp2);
			fputs(p->Inventory, fp2);
			fputs(p->Sell, fp2);
			fputc('\n', fp2);
			fclose(fp2);
		}
	}
	return true;
}

/*****************************************************************
�����Ʒ
����1����Ʒ����
*/
void Buy(base*& head)
{
	base* p = head->next;
	bool flag = true;
	int num = Disp(head,0,0),i ;
	printf("\t\t\t\t\t\t\t\t\t\t________________________________________________________\n");
	do 
	{
		xxx:
		printf("\t\t\t\t\t\t\t\t\t\t");
		fflush(stdin);
		if (scanf("%d", &i) && i > 0 && i <= num)
		{
			if(GetElem(head, i, 2) == -1)
			{
				printf("\t\t\t\t\t\t\t\t\t\t��治�㣬�޷�ѡ����\n");
				goto xxx;
			}
			GetElem(head, i, 0);
		}
		else
		{
			if (i != 0)
			{
				printf("\n\t\t\t\t\t\t\t\t\t\tѡ�񳬳���Χ��2����Զ��˳�...");
				Sleep(2000);
			}
			flag = false;
		}
	} while (flag);
}

/*****************************************************************
������ʾ
����1���ļ�·��
����2���Ƿ�ָ��������� ģʽ��0����ָ��Max��ֵ
����3���������
*/
void Sort(char x[],int mode,int Max)
{
	
	base* head;InitList(head);
	Load(head, x,1);
	int fi = Disp(head, 1, 0);
	fi >= 5 ? (Max = 5) : (Max = fi);
	int num[20] = { 0 }, i = 0;
	while (i < Max)
	{
		base* t = head;
		base* p = head->next;
		int max = 0;
		while (strlen(p->name) == FOOD-1)
		{
			if (max <= atoi(p->Sell) && atoi(p->Inventory)>0 )
			{
				max = atoi(p->Sell);  //��ȡ�����������
				num[i] = p->Sequence;  ///��ȡ����������Ĳ�Ʒ���
			}
			p = p->next;
		}
		while (num[i] != t->next->Sequence)   //�ƶ���㵽���ֵ��
			t = t->next;
		t->next = t->next->next;  //ɾ�����ֵ
		i++;
	}
	Del(head);
	//��ʾ����������
	base* he;
	InitList(he);
	Load(he, x,1);
	Tip(0);
	for (int z = 0; z < Max; z++)
	{
		base* hp = he;
		while (hp->Sequence != num[z])
			hp = hp->next;
		printf("\t\t\t\t\t\t\t\t\t\t%d\t%-21s%-10s%-10s%-10s\n\n",hp->Sequence,hp->name, hp->price, hp->Inventory, hp->Sell);
	}
	Del(he);
}

//�����Ƽ�
void Recommend(char path[],char p[])
{
	printf("\n\n\t\t\t\t\t\t\t\t\t\t��ʾ: ѡ���Ʒǰ�ı�ż���ѡ�ͣ�ѡ��0�˳�\n");
	HANDLE hOut;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	printf("\n\t\t\t\t\t\t\t\t\t\t");
	printf("[");
	SetConsoleTextAttribute(hOut, 244);
	printf(p);
	printf("��ϵ");
	SetConsoleTextAttribute(hOut,240);
	printf("]\n");
}

//��ʾָ��Ŀ¼���ļ��б�
//0ģʽ������ӡ
void printDir( const char* path , file *&head,int mode)
{
	Initfile(head->next);
	file *p = head;int i=0,e=0;
    struct _finddata_t data;
    long hnd = _findfirst( path, &data );    // �����ļ�����������ʽchRE��ƥ���һ���ļ�
    if ( hnd < 0 )
        perror( path );
    int  nRet = (hnd <0 ) ? -1 : 1;
	printf("\n\n\t\t\t\t\t\t\t\t\t");
    while ( nRet >= 0 )
    {
        if( data.attrib != _A_SUBDIR)  // �������Ŀ¼
		{
			if(mode != 0)
			{
				printf("\t[%d]\t[%s]",i+1, data.name );	
				if(i%2 == 1)
					printf("\n\n\t\t\t\t\t\t\t\t\t");
				else if(strlen(data.name)<6)
					printf("\t");
			}
			strcat(p->name,data.name);
			p->num = ++i;
			Initfile(p->next);
			p->next->front = p;
			p = p->next;
		}
        nRet = _findnext( hnd, &data );
    }
	printf("\n");
    _findclose( hnd );     // �رյ�ǰ���e68a84e799bee5baa6e79fa5e9819331333337616632
	p = p->front;free(p->next);p->next = head,head->front = p;  //β���ָ��ڶ�����㣬��һ�����ݽ��
}

//�޸����ơ��۸񡢿��
void Goto(base *&head,int i,int x)
{
	fflush(stdin);
	base *s = head;
	base *xp = NULL;
	if(x == 2)
	{
		for(int m=0;m<i-1;m++)
			s = s->next;
		printf("\t\t\t\t\t\t\t\t\t\t\t%-21s%-10s%-10s%-10s\n",s->next->name, s->next->price, s->next->Inventory, s->next->Sell);
		printf("\n\t\t\t\t\t\t\t\t\t\t��Ʒ�¼���...");
		xp = s->next;s->next = s->next->next;free(xp);Sleep(800);goto flag;
	}
	for(int m=0;m<i;m++)
		s = s->next;	
	printf("\t\t\t\t\t\t\t\t\t\t\t%-21s%-10s%-10s%-10s\n",s->name, s->price, s->Inventory, s->Sell);
	if(x == 3)
	{
		printf("\n\t\t\t\t\t\t\t\t\t\t�������޸ĺ�����֣�");
		scanf("%s",s->name);
	}
	else if(x == 4)
	{
		printf("\n\t\t\t\t\t\t\t\t\t\t�������޸ĺ�ļ۸�");
		scanf("%s",s->price);
	}
	else if(x == 5)
	{
		printf("\n\t\t\t\t\t\t\t\t\t\t�������޸ĺ�Ŀ�棺");
		scanf("%s",s->Inventory);
	}
	flag:
	Initstr(s);
	system("cls");	
	printf("\n\t\t\t\t\t\t\t\t\t\t��������0���沢�˳��������ż����޸ģ��޸ĺ�\n");
	Disp(head,0,0);
}

//�����µĲ�ϵ
void new_file(Path *path,char mode[],char x[])    
{
	char strName[FOOD];   ////��ϵ����
	memset(path->Empty,'\0',sizeof(path->Empty));
	strcat(path->Empty,path->Systempath);
	memset(strName,'\0',sizeof(strName));
	if(strcmp(mode,"w")==0)
	{
		printf("\n\t\t\t\t\t\t\t\t\t\t\t�������ϵ���ƣ�");
		fflush(stdin);
		scanf("%s",strName);
		strcat(path->Empty,strName);
	}
	base *head;InitList(head);base *s = head;
	InitList(s->next);s = s->next;
	
	while(1)////д�����ݵ�����
	{
		char c; 
		printf("\n\t\t\t\t\t\t\t\t\t\t\t�����������");
		
		scanf("%s",s->name);
		printf("\n\t\t\t\t\t\t\t\t\t\t\t�۸�");
		while ((c = getchar()) != EOF && c != '\n');
		scanf("%s",s->price);
		printf("\n\t\t\t\t\t\t\t\t\t\t\t��棺");
		while ((c = getchar()) != EOF && c != '\n');
		scanf("%s",s->Inventory);
		strcat(s->Sell,"0");
		Initstr(s);
		InitList(s->next);
		s = s->next;
		printf("\n\t\t\t\t\t\t\t\t\t\t\t�س�����������0�˳�\n\t\t\t\t\t\t\t\t\t\t\t");
		while ((c = getchar()) != EOF && c != '\n');//��ͣ��ʹ��getchar()��ȡ�������ַ���ֱ����ȡ��c�ǡ�\n�����ļ���β��EOFΪֹ
		if(getch()=='0')
			break;
	}
	InitList(s->next);/////�ഴ��һ����㣬Ϊ����ӦDisp
	system("cls");
	if(strcmp(mode,"w")==0)
		printf("\n\n\n\t\t\t\t\t\t\t\t\t\t�½�%s��ϵ\n",strName);
	Disp(head,0,0);
	printf("\t\t\t\t\t\t\t\t\t\t��������������棡����0������");
	if(getch()!='0')
	{
		if(strcmp(mode,"a")==0)
			Write_in(head,x,mode,0); //д�����ݵ��ļ�
		else
			Write_in(head,path->Empty,mode,0); //д�����ݵ��ļ�
	}
	printf("\n\n\t\t\t\t\t\t\t\t\t\t2����˳�...");
	Sleep(2000);
	Del(head);
}

//�޸�ԭ�в˵���Ϣ
bool Modify(int x,Path *path)    
{
	system("cls");
	switch(x)
	{
	case 0:return false;
	case 1:printf("\n\n\n\t\t\t\t\t\t\t\t\t\t\t��ѡ���˲�Ʒ�ϼܣ�");break;
	case 2:printf("\n\n\n\t\t\t\t\t\t\t\t\t\t\t��ѡ���˲�Ʒ�¼ܣ�");break;
	case 3:printf("\n\n\n\t\t\t\t\t\t\t\t\t\t\t��ѡ�����޸Ĳ�Ʒ���ƣ�");break;
	case 4:printf("\n\n\n\t\t\t\t\t\t\t\t\t\t\t��ѡ�����޸Ĳ�Ʒ�۸�");break;
	case 5:printf("\n\n\n\t\t\t\t\t\t\t\t\t\t\t��ѡ�����޸Ĳ�Ʒ��棡");break;
	case 7:printf("\n\n\n\t\t\t\t\t\t\t\t\t\t\t��ѡ���˲�ϵ�¼ܣ�");break;
	default:printf("\n\n\n\t\t\t\t\t\t\t\t\t\t\tѡ�񳬳���Χ�������ԣ�����");Sleep(1000);return true;
	}
	int i,h = 1;
	base *head;InitList(head);base *s = head->next;
	file *data;Initfile(data);file *p = data;
	file *pathName;Initfile(pathName);file *xp = pathName;
	printf("��������0�˳��˲˵�\n\n\t\t\t\t\t\t\t\t\t\t\t������Ҫ�޸ĵĲ�ϵ���");
	printDir(path->Filepath,data,0);  ///�������·�����ļ�������data��
	data->front->next = NULL;Initfile(data->front->next); ////�⿪˫����
	while(p->next != NULL)
	{
		strcat(xp->name,path->Systempath);//����ϵͳ·��
		strcat(xp->name,p->name);  //�����ļ���
		xp->num = h++;
		printf("\n\t\t\t\t\t\t\t\t\t\t\t%d\t[%s]\n",p->num,p->name);
		Initfile(xp->next);
		xp = xp->next;
		p = p->next;
	}
	printf("\n\t\t\t\t\t\t\t\t\t\t\t");
	h = getch()-48;    ///�����޸Ĳ˵��ı��
	if(h == 0)
		return false;
	xp = pathName;
	while(h != xp->num && h >0 && xp->next != NULL)
		xp = xp->next;
	if(Load(head,xp->name,1) == false)
		return true;
	if(x == 7)
	{
		strcat(path->Empty,"del ");
		strcat(path->Empty,xp->name);
		system(path->Empty);
		memset(path->Empty,'\0',sizeof(path->Empty));
		printf("\n\n\t\t\t\t\t\t\t\t\t\t\t�¼ܳɹ���");Sleep(1000);
		return true;
	}
	system("cls");
	printf("\n\n\n\t\t\t\t\t\t\t\t\t\t��������0�˳���������Ҫ�޸Ĳ�Ʒ�ı��\n\n");
	int num = Disp(head,0,0);
	if(x == 1)
	{
		base *add;InitList(add);
		new_file(path,"a",xp->name);
		return true;
	}
	do
	{
		printf("\n\t\t\t\t\t\t\t\t\t\t");
		if(scanf("%d",&i) && 0<i && i<=num)  ////����Ϸ����
			Goto(head,i,x);
		else
		{
			if(i != 0)  ///0�������˳�
				printf("\t\t\t\t\t\t\t\t\t\tѡ�񳬳���Χ���Զ��˳���\n");
			printf("\n\t\t\t\t\t\t\t\t\t\t��������...");
			Sleep(1000);
			break;
		}
	}while(i != 0);
	Write_in(head,xp->name,"w",0);
	Del(head);Del(data);Del(pathName);
	return true;
}

//��治�㷴��
void Send(Path *path)
{
	system("cls");
	file *head;Initfile(head);file *p = head;  //�����ļ���
	char filepath[100];  //�����ļ�·��
	printDir(path->Filepath,head,0);     ///�ϳ��ļ�·��
	head->front->next = NULL;
	base *xp;bool flag = true;
	while(p->next != NULL)
	{
		memset(filepath,'\0',sizeof(filepath));
		strcat(filepath,path->Systempath);
		strcat(filepath,p->name);
		base *data;InitList(data);xp = data;
		Load(xp,filepath,1);
		xp = xp->next;
		while(xp->next != NULL && strlen(xp->name) == FOOD-1)
		{
			if(atoi(xp->Inventory)<NUM)
			{
				if(flag == true)
				{
					printf("\n\t\t\t\t\t\t\t\t\t\t\t              ���²�Ʒ��治�㣬�뼰ʱ���䣡����\n\a"); //Ԥ������
					printf("\t\t\t\t\t\t\t\t\t\t\t_____________________________________________________________\n\n");
					printf("\t\t\t\t\t\t\t\t\t\t\t  %-19s%-10s%-10s%-10s%-10s\n\n","[��Ʒ]","[�۸�]","[ʣ��]","[������]"," [��ϵ]");
				}
				flag = false;
				printf("\t\t\t\t\t\t\t\t\t\t\t   %-19s%-10s%-10s%-9s%s\n",xp->name, xp->price, xp->Inventory, xp->Sell,p->name);
			}
			xp = xp->next;
		}
		Del(data);
		p = p->next;
	}
	printf("\t\t\t\t\t\t\t\t\t\t\t_____________________________________________________________\n");
	Del(head);
}
