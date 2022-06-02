#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <io.h>
#define FOOD 21   ////菜名的数据空间
#define FIGURE 10 ////数字数据空间

#define NUM 5  //库存预警数量
#define PATHLENGTH 100

typedef struct Node//(类型)  ////菜品结构体
{
	int Sequence;  //编号
	char name[FOOD]; 
	char price[FIGURE];
	char Inventory[FIGURE]; //库存
	char Sell[FIGURE];
	char num[FIGURE]; ///数量
	struct Node* next;
}base;

typedef struct xx   ///////读取文件的结构体
{
	int num;
	char name[50];
	struct xx *next;
	struct xx *front;
}file;

typedef struct yy     //////保存路径结构体
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
销毁链表
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

void Getdata(DATA &data)  //////组合时间 
{
	memset(data.data_day,'\0',sizeof(data.data_day));
	strcat(data.data_day,data.year);
	strcat(data.data_day,"年");
    strcat(data.data_day,data.month);
	strcat(data.data_day,"月");
    strcat(data.data_day,data.day);
	strcat(data.data_day,"日");

	memset(data.data_mon,'\0',sizeof(data.data_mon));
	strcat(data.data_mon,data.year);
	strcat(data.data_mon,"年");
    strcat(data.data_mon,data.month);
	strcat(data.data_mon,"月");
}

void Time(DATA &data)   ////获取当前时间 
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

//去掉文件内重复的元素
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
	printf("\t\t\t\t\t\t\t\t\t\t共%d个菜品,总额%d元\n",--numb,total);
	printf("\n\t\t\t\t\t\t\t\t\t\t按任意键继续");
	Del(head); 
	i= j = NULL; 
	getch();
 } 


void Tip(int num)
{
	if(num != 3)
		printf("\t\t\t\t\t\t\t\t\t\t________________________________________________________\n\n");
	if(num == 3)
		printf("\t\t\t\t\t\t\t\t\t\t编号\t%-21s%-10s%-10s%-10s\t%s\n\n","菜品","价格","剩余","销售量","数量");
	if(num != 1 && num!=3)
		printf("\t\t\t\t\t\t\t\t\t\t编号\t%-21s%-10s%-10s%-10s\n\n","菜品","价格","剩余","销售量");
}

/*****************************************************************
初始化   创建一个结点
*/
void InitList(base *&p)      /////初始化菜品结构体
{
	p = (base*)malloc(sizeof(base));
	memset(p, '\0', sizeof(base));
	p->next = NULL;
}
void initArray(char p[],int length)  ////数组填充
{
	for(int i=strlen(p);i<length;i++)
		p[i] = ' ';
	p[length-1] = '\0';
}

void Initstr(base *&head)  //结构体填充
{
	initArray(head->name,FOOD);
	initArray(head->price,FIGURE);
	initArray(head->Inventory,FIGURE);
	initArray(head->Sell,FIGURE);
	initArray(head->num,FIGURE);
}
void Initfile(file *&p)  /////文件初始话
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
说明：从文件内读取数据
参数1：菜品链表
参数2：文件路径
参数3：模式：0模式加载个数 2模式不显示文字
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
			printf("\n\n\t\t\t\t\t\t\t\t\t\t\t读取数据错误\n\n\t\t\t\t\t\t\t\t\t\t\t没有%s这个文件\n\n\t\t\t\t\t\t\t\t\t\t\t2秒后退出",p);
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
将链表中的数据写入到文件中
参数1：菜品链表
参数2：文件路径
参数3：模式：以指定模式【写】文件
参数4：1模式写入个数
*/
void Write_in(base*& head, char p[],char mode[],int num)
{
	base* s = head->next;
	FILE* fp = fopen(p, mode);
	if (fp == NULL)
	{
		printf("\t\t\t\t\t\t\t\t\t\t写入数据失败\n\n\t\t\t\t\t\t\t\t\t\t按任意键退出程序");
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
显示链表，并返回元素个数
参数1：菜品链表
参数2：模式：1模式可以仅返回个数，不显示文字
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


//账单写入
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
参数1：菜品链表
参数2：求值的位置
参数3：
		0.模式：打印指定位置元素信息
		1.模式：返回指定元素价格
		2.模式：修改指定元素库存
*/
int GetElem(base*& head, int i, int mode)    //求线性表中的某个位置的元素值 
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
		if (mode == 0)       //0模式，打印信息
		{
			printf("\t\t\t\t\t\t\t\t\t\t%d\t%-21s%-10s%-10s%-10s\n",i,p->name, p->price, p->Inventory, p->Sell);
			return true;
		}
		else if (mode == 1)  //1模式，返回价格
			return atoi(p->price);
		else if (mode == 2)   //2模式，修改库存
		{
			int m = atoi(p->Inventory) - 1;
			if(m<0)  ////如果库存数量不足，则返回
				return -1;
			_itoa(m, p->Inventory, FIGURE); ///库存数量-1
			initArray(p->Inventory,FIGURE); 
			_itoa(atoi(p->Sell) + 1, p->Sell, FIGURE);	///销售数量+1
			initArray(p->Sell,FIGURE);
			
			////购物背包
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
购买菜品
参数1：菜品链表
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
				printf("\t\t\t\t\t\t\t\t\t\t库存不足，无法选择此项！\n");
				goto xxx;
			}
			GetElem(head, i, 0);
		}
		else
		{
			if (i != 0)
			{
				printf("\n\t\t\t\t\t\t\t\t\t\t选择超出范围！2秒后自动退出...");
				Sleep(2000);
			}
			flag = false;
		}
	} while (flag);
}

/*****************************************************************
排序并显示
参数1：文件路径
参数2：是否指定排序个数 模式【0】需指定Max的值
参数3：排序个数
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
				max = atoi(p->Sell);  //获取最大销售量，
				num[i] = p->Sequence;  ///获取最大销售量的菜品编号
			}
			p = p->next;
		}
		while (num[i] != t->next->Sequence)   //移动结点到最大值处
			t = t->next;
		t->next = t->next->next;  //删除最大值
		i++;
	}
	Del(head);
	//显示并保存数据
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

//今日推荐
void Recommend(char path[],char p[])
{
	printf("\n\n\t\t\t\t\t\t\t\t\t\t提示: 选择菜品前的编号即可选餐，选择0退出\n");
	HANDLE hOut;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	printf("\n\t\t\t\t\t\t\t\t\t\t");
	printf("[");
	SetConsoleTextAttribute(hOut, 244);
	printf(p);
	printf("菜系");
	SetConsoleTextAttribute(hOut,240);
	printf("]\n");
}

//显示指定目录的文件列表
//0模式，不打印
void printDir( const char* path , file *&head,int mode)
{
	Initfile(head->next);
	file *p = head;int i=0,e=0;
    struct _finddata_t data;
    long hnd = _findfirst( path, &data );    // 查找文件名与正则表达式chRE的匹配第一个文件
    if ( hnd < 0 )
        perror( path );
    int  nRet = (hnd <0 ) ? -1 : 1;
	printf("\n\n\t\t\t\t\t\t\t\t\t");
    while ( nRet >= 0 )
    {
        if( data.attrib != _A_SUBDIR)  // 如果不是目录
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
    _findclose( hnd );     // 关闭当前句柄e68a84e799bee5baa6e79fa5e9819331333337616632
	p = p->front;free(p->next);p->next = head,head->front = p;  //尾结点指向第二个结点，第一个数据结点
}

//修改名称、价格、库存
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
		printf("\n\t\t\t\t\t\t\t\t\t\t菜品下架中...");
		xp = s->next;s->next = s->next->next;free(xp);Sleep(800);goto flag;
	}
	for(int m=0;m<i;m++)
		s = s->next;	
	printf("\t\t\t\t\t\t\t\t\t\t\t%-21s%-10s%-10s%-10s\n",s->name, s->price, s->Inventory, s->Sell);
	if(x == 3)
	{
		printf("\n\t\t\t\t\t\t\t\t\t\t请输入修改后的名字：");
		scanf("%s",s->name);
	}
	else if(x == 4)
	{
		printf("\n\t\t\t\t\t\t\t\t\t\t请输入修改后的价格：");
		scanf("%s",s->price);
	}
	else if(x == 5)
	{
		printf("\n\t\t\t\t\t\t\t\t\t\t请输入修改后的库存：");
		scanf("%s",s->Inventory);
	}
	flag:
	Initstr(s);
	system("cls");	
	printf("\n\t\t\t\t\t\t\t\t\t\t输入数字0保存并退出！输入编号继续修改，修改后：\n");
	Disp(head,0,0);
}

//创建新的菜系
void new_file(Path *path,char mode[],char x[])    
{
	char strName[FOOD];   ////菜系名称
	memset(path->Empty,'\0',sizeof(path->Empty));
	strcat(path->Empty,path->Systempath);
	memset(strName,'\0',sizeof(strName));
	if(strcmp(mode,"w")==0)
	{
		printf("\n\t\t\t\t\t\t\t\t\t\t\t请输入菜系名称：");
		fflush(stdin);
		scanf("%s",strName);
		strcat(path->Empty,strName);
	}
	base *head;InitList(head);base *s = head;
	InitList(s->next);s = s->next;
	
	while(1)////写入数据到链表
	{
		char c; 
		printf("\n\t\t\t\t\t\t\t\t\t\t\t请输入菜名：");
		
		scanf("%s",s->name);
		printf("\n\t\t\t\t\t\t\t\t\t\t\t价格：");
		while ((c = getchar()) != EOF && c != '\n');
		scanf("%s",s->price);
		printf("\n\t\t\t\t\t\t\t\t\t\t\t库存：");
		while ((c = getchar()) != EOF && c != '\n');
		scanf("%s",s->Inventory);
		strcat(s->Sell,"0");
		Initstr(s);
		InitList(s->next);
		s = s->next;
		printf("\n\t\t\t\t\t\t\t\t\t\t\t回车继续，键入0退出\n\t\t\t\t\t\t\t\t\t\t\t");
		while ((c = getchar()) != EOF && c != '\n');//不停地使用getchar()获取缓冲中字符，直到获取的c是“\n”或文件结尾符EOF为止
		if(getch()=='0')
			break;
	}
	InitList(s->next);/////多创建一个结点，为了适应Disp
	system("cls");
	if(strcmp(mode,"w")==0)
		printf("\n\n\n\t\t\t\t\t\t\t\t\t\t新建%s菜系\n",strName);
	Disp(head,0,0);
	printf("\t\t\t\t\t\t\t\t\t\t请输入任意键保存！输入0放弃！");
	if(getch()!='0')
	{
		if(strcmp(mode,"a")==0)
			Write_in(head,x,mode,0); //写入数据到文件
		else
			Write_in(head,path->Empty,mode,0); //写入数据到文件
	}
	printf("\n\n\t\t\t\t\t\t\t\t\t\t2秒后退出...");
	Sleep(2000);
	Del(head);
}

//修改原有菜单信息
bool Modify(int x,Path *path)    
{
	system("cls");
	switch(x)
	{
	case 0:return false;
	case 1:printf("\n\n\n\t\t\t\t\t\t\t\t\t\t\t您选择了菜品上架！");break;
	case 2:printf("\n\n\n\t\t\t\t\t\t\t\t\t\t\t您选择了菜品下架！");break;
	case 3:printf("\n\n\n\t\t\t\t\t\t\t\t\t\t\t您选择了修改菜品名称！");break;
	case 4:printf("\n\n\n\t\t\t\t\t\t\t\t\t\t\t您选择了修改菜品价格！");break;
	case 5:printf("\n\n\n\t\t\t\t\t\t\t\t\t\t\t您选择了修改菜品库存！");break;
	case 7:printf("\n\n\n\t\t\t\t\t\t\t\t\t\t\t您选择了菜系下架！");break;
	default:printf("\n\n\n\t\t\t\t\t\t\t\t\t\t\t选择超出范围！请重试！！！");Sleep(1000);return true;
	}
	int i,h = 1;
	base *head;InitList(head);base *s = head->next;
	file *data;Initfile(data);file *p = data;
	file *pathName;Initfile(pathName);file *xp = pathName;
	printf("输入数字0退出此菜单\n\n\t\t\t\t\t\t\t\t\t\t\t请输入要修改的菜系编号");
	printDir(path->Filepath,data,0);  ///加载这个路径的文件，放在data中
	data->front->next = NULL;Initfile(data->front->next); ////解开双链表
	while(p->next != NULL)
	{
		strcat(xp->name,path->Systempath);//复制系统路径
		strcat(xp->name,p->name);  //复制文件名
		xp->num = h++;
		printf("\n\t\t\t\t\t\t\t\t\t\t\t%d\t[%s]\n",p->num,p->name);
		Initfile(xp->next);
		xp = xp->next;
		p = p->next;
	}
	printf("\n\t\t\t\t\t\t\t\t\t\t\t");
	h = getch()-48;    ///输入修改菜单的编号
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
		printf("\n\n\t\t\t\t\t\t\t\t\t\t\t下架成功！");Sleep(1000);
		return true;
	}
	system("cls");
	printf("\n\n\n\t\t\t\t\t\t\t\t\t\t输入数字0退出！请输入要修改菜品的编号\n\n");
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
		if(scanf("%d",&i) && 0<i && i<=num)  ////输入合法检查
			Goto(head,i,x);
		else
		{
			if(i != 0)  ///0是正常退出
				printf("\t\t\t\t\t\t\t\t\t\t选择超出范围，自动退出！\n");
			printf("\n\t\t\t\t\t\t\t\t\t\t保存数据...");
			Sleep(1000);
			break;
		}
	}while(i != 0);
	Write_in(head,xp->name,"w",0);
	Del(head);Del(data);Del(pathName);
	return true;
}

//库存不足反馈
void Send(Path *path)
{
	system("cls");
	file *head;Initfile(head);file *p = head;  //保存文件名
	char filepath[100];  //保存文件路径
	printDir(path->Filepath,head,0);     ///合成文件路径
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
					printf("\n\t\t\t\t\t\t\t\t\t\t\t              以下菜品库存不足，请及时补充！！！\n\a"); //预警声音
					printf("\t\t\t\t\t\t\t\t\t\t\t_____________________________________________________________\n\n");
					printf("\t\t\t\t\t\t\t\t\t\t\t  %-19s%-10s%-10s%-10s%-10s\n\n","[菜品]","[价格]","[剩余]","[销售量]"," [菜系]");
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
