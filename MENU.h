#include "BUG.h"

/*****************************************************************
				  食堂系统   &&   管理员系统
******************************************************************/
//账单菜单
void function(file *&head,int mode)
{
		xm:
		Initfile(head);
		int m = -1;system("cls");printf("\n\n");
		printf("\t\t\t\t\t\t\t\t\t\t输入0退出！");
		if(mode == 8)
			printDir("data_day\\*",head,1);
		if(mode == 9)
			printDir("data_mon\\*",head,1);
		head->front->next = NULL;Initfile(head->front->next);
		file *p = head;printf("\n\t\t\t\t\t\t\t\t\t\t请选择：");
		char str[40];fflush(stdin);
		int xs = scanf("%d",&m);
		while(p->next != NULL)
		{
			if(xs == 0)
			{
				printf("\n\t\t\t\t\t\t\t\t\t\t输入违法!请重试");
				Sleep(1000);
				Del(head);
				goto xm;
			}
			if(m == 0)
			{
				Del(head);
				return ;
			}
			if(p->num == m)
			{
				memset(str,'\0',sizeof(str));
				if(mode == 8)
					strcat(str,"data_day\\");
				if(mode == 9)
					strcat(str,"data_mon\\");
				strcat(str,p->name);
				fun(str);
				goto xm;
			}
			else
				p = p->next;
		}
		if(p->next == NULL)
		{
			printf("\n\t\t\t\t\t\t\t\t\t\t输入超出范围!请重试");
			Sleep(1000);
			Del(head);
			goto xm;
		}
}

//食堂功能菜单
void index01(Path *path)     
{
	xy:
	bool flag = true;
	file *head;Initfile(head);
	printDir(path->Filepath,head,0);
	file *p = head;
	base *data;InitList(data);
	x: 
	file *rt;Initfile(rt);  ////专用于显示
	char i = -1;
	if(flag == false)
	{
		printf("\n\t\t\t\t\t\t\t\t\t\t");
		fflush(stdin);
		i = getch();
	}
	system("cls");
	flag = false;
	if(i == 'd' || i == 'D')
		p = p->next;
	if(i == 'a' || i == 'A')
		p = p->front;
	if(i == 48)  ////0退出
	{
		Del(data);
		Free(head);
		base *hp;InitList(hp);
		if(Load(hp,"temp.txt",2)==false)
			return ;
		printf("\n\n\t\t\t\t\t\t\t\t\t\t您的消费清单如下！\n");
		int num = 0,total = 0;
		hp = hp->next;
		printf("\n\t\t\t\t\t\t\t\t\t\t%-20s%-10s\n","[菜品]","[价格]");
		while(hp->next != NULL)
		{
			printf("\n\t\t\t\t\t\t\t\t\t\t%s %s\n",hp->name,hp->price); 
			num++;
			total+=atoi(hp->price);
			hp = hp->next;
		}
		printf("\t\t\t\t\t\t\t\t\t\t共%d个菜品，合计%d元 ，选餐结束，祝您用餐愉快！\n\n\t\t\t\t\t\t\t\t\t\t按任意键退出",--num, total);
		system("del temp.txt");
		Del(hp);
		getchar();
		return ;
	}

	HANDLE hOut;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hOut, 244);
	printf("\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t\t\t请按'A' 或者 'D'键左右切换菜系,回车键选择该菜系,[0]退出！\n");
	SetConsoleTextAttribute(hOut,240);
	if(strlen(p->name)>=6)
		printf("\b");
	printDir(path->Filepath,rt,1);  //显示菜系
	Free(rt);   ////显示完毕，立马free
	memset(path->Empty,'\0',sizeof(path->Empty));
	strcat(path->Empty,path->Systempath);
	strcat(path->Empty,p->name);
	Load(data,path->Empty,1);
	if(i == 13) //回车键 
	{
		system("cls");
		Recommend(path->Empty,p->name);
		Buy(data);
		Write_in(data,path->Empty,"w",1);
		Del(data);
		Free(head);
		goto xy;
	}
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	printf("\n\t\t\t\t\t\t\t\t\t\t[");
	SetConsoleTextAttribute(hOut, 244);
	printf(p->name);
	printf("推荐");
	SetConsoleTextAttribute(hOut,240);
	printf("]\n");
	Sort(path->Empty,0,5);  ///预览5个菜品
	goto x;
}

//管理员首页
int admenu(Path *path)
{
	Send(path);  //库存不足反馈
	printf("\n\t\t\t\t\t\t\t\t\t\t\t                       请 选 择\n");
	printf("\n\t\t\t\t\t\t\t\t\t\t\t   [菜品上架]       [1]\t\t   [菜品下架]       [2]\n");
	printf("\n\t\t\t\t\t\t\t\t\t\t\t   [菜名修改]       [3]\t\t   [价格修改]       [4]\n");
	printf("\n\t\t\t\t\t\t\t\t\t\t\t   [库存修改]       [5]\t\t   [菜系上架]       [6]\n");
	printf("\n\t\t\t\t\t\t\t\t\t\t\t   [菜系下架]       [7]\t\t   [日账单]         [8]\n");
	printf("\n\t\t\t\t\t\t\t\t\t\t\t   [月账单]         [9]\t\t   [退出终端]       [0]\n\n\t\t\t\t\t\t\t\t\t\t\t   ");
	return _getch()-48;
}
//管理员功能菜单
void index02(Path *path)   
{
	flag:
	int i = admenu(path);
	if(i == 6)
	{
		new_file(path,"w",path->Filepath);  ///path->Filepath为垃圾参数，在这里不起作用
	}
	else if(i == 8 || i == 9)
	{
		file *head_day;
		file *head_mon;
		if(i == 8)
		function(head_day,8);
		if(i == 9)
		function(head_mon,9);
		goto flag;
	}
	else if(Modify(i,path) == false)
		return ;
	goto flag;
}
//首页
int indexMenu()
{
	system("cls");
	HANDLE hOut;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hOut, 244);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t\t\t\t\t欢迎使用爱上食堂点餐系统\n\n");
	SetConsoleTextAttribute(hOut, 240);
	printf("\n\t\t\t\t\t\t\t\t\t\t\t\t[用户登陆]            [1]\n");
	printf("\n\t\t\t\t\t\t\t\t\t\t\t\t[管理员登陆]          [2]\n");
	printf("\n\t\t\t\t\t\t\t\t\t\t\t\t[退出系统]            [0]");
	printf("\n\n\t\t\t\t\t\t\t\t\t\t\t\t请选择 ：");fflush(stdin);
	printf("");
	return _getch()-48;
}

