#include "BUG.h"

/*****************************************************************
				  ʳ��ϵͳ   &&   ����Աϵͳ
******************************************************************/
//�˵��˵�
void function(file *&head,int mode)
{
		xm:
		Initfile(head);
		int m = -1;system("cls");printf("\n\n");
		printf("\t\t\t\t\t\t\t\t\t\t����0�˳���");
		if(mode == 8)
			printDir("data_day\\*",head,1);
		if(mode == 9)
			printDir("data_mon\\*",head,1);
		head->front->next = NULL;Initfile(head->front->next);
		file *p = head;printf("\n\t\t\t\t\t\t\t\t\t\t��ѡ��");
		char str[40];fflush(stdin);
		int xs = scanf("%d",&m);
		while(p->next != NULL)
		{
			if(xs == 0)
			{
				printf("\n\t\t\t\t\t\t\t\t\t\t����Υ��!������");
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
			printf("\n\t\t\t\t\t\t\t\t\t\t���볬����Χ!������");
			Sleep(1000);
			Del(head);
			goto xm;
		}
}

//ʳ�ù��ܲ˵�
void index01(Path *path)     
{
	xy:
	bool flag = true;
	file *head;Initfile(head);
	printDir(path->Filepath,head,0);
	file *p = head;
	base *data;InitList(data);
	x: 
	file *rt;Initfile(rt);  ////ר������ʾ
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
	if(i == 48)  ////0�˳�
	{
		Del(data);
		Free(head);
		base *hp;InitList(hp);
		if(Load(hp,"temp.txt",2)==false)
			return ;
		printf("\n\n\t\t\t\t\t\t\t\t\t\t���������嵥���£�\n");
		int num = 0,total = 0;
		hp = hp->next;
		printf("\n\t\t\t\t\t\t\t\t\t\t%-20s%-10s\n","[��Ʒ]","[�۸�]");
		while(hp->next != NULL)
		{
			printf("\n\t\t\t\t\t\t\t\t\t\t%s %s\n",hp->name,hp->price); 
			num++;
			total+=atoi(hp->price);
			hp = hp->next;
		}
		printf("\t\t\t\t\t\t\t\t\t\t��%d����Ʒ���ϼ�%dԪ ��ѡ�ͽ�����ף���ò���죡\n\n\t\t\t\t\t\t\t\t\t\t��������˳�",--num, total);
		system("del temp.txt");
		Del(hp);
		getchar();
		return ;
	}

	HANDLE hOut;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hOut, 244);
	printf("\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t\t\t�밴'A' ���� 'D'�������л���ϵ,�س���ѡ��ò�ϵ,[0]�˳���\n");
	SetConsoleTextAttribute(hOut,240);
	if(strlen(p->name)>=6)
		printf("\b");
	printDir(path->Filepath,rt,1);  //��ʾ��ϵ
	Free(rt);   ////��ʾ��ϣ�����free
	memset(path->Empty,'\0',sizeof(path->Empty));
	strcat(path->Empty,path->Systempath);
	strcat(path->Empty,p->name);
	Load(data,path->Empty,1);
	if(i == 13) //�س��� 
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
	printf("�Ƽ�");
	SetConsoleTextAttribute(hOut,240);
	printf("]\n");
	Sort(path->Empty,0,5);  ///Ԥ��5����Ʒ
	goto x;
}

//����Ա��ҳ
int admenu(Path *path)
{
	Send(path);  //��治�㷴��
	printf("\n\t\t\t\t\t\t\t\t\t\t\t                       �� ѡ ��\n");
	printf("\n\t\t\t\t\t\t\t\t\t\t\t   [��Ʒ�ϼ�]       [1]\t\t   [��Ʒ�¼�]       [2]\n");
	printf("\n\t\t\t\t\t\t\t\t\t\t\t   [�����޸�]       [3]\t\t   [�۸��޸�]       [4]\n");
	printf("\n\t\t\t\t\t\t\t\t\t\t\t   [����޸�]       [5]\t\t   [��ϵ�ϼ�]       [6]\n");
	printf("\n\t\t\t\t\t\t\t\t\t\t\t   [��ϵ�¼�]       [7]\t\t   [���˵�]         [8]\n");
	printf("\n\t\t\t\t\t\t\t\t\t\t\t   [���˵�]         [9]\t\t   [�˳��ն�]       [0]\n\n\t\t\t\t\t\t\t\t\t\t\t   ");
	return _getch()-48;
}
//����Ա���ܲ˵�
void index02(Path *path)   
{
	flag:
	int i = admenu(path);
	if(i == 6)
	{
		new_file(path,"w",path->Filepath);  ///path->FilepathΪ���������������ﲻ������
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
//��ҳ
int indexMenu()
{
	system("cls");
	HANDLE hOut;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hOut, 244);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t\t\t\t\t��ӭʹ�ð���ʳ�õ��ϵͳ\n\n");
	SetConsoleTextAttribute(hOut, 240);
	printf("\n\t\t\t\t\t\t\t\t\t\t\t\t[�û���½]            [1]\n");
	printf("\n\t\t\t\t\t\t\t\t\t\t\t\t[����Ա��½]          [2]\n");
	printf("\n\t\t\t\t\t\t\t\t\t\t\t\t[�˳�ϵͳ]            [0]");
	printf("\n\n\t\t\t\t\t\t\t\t\t\t\t\t��ѡ�� ��");fflush(stdin);
	printf("");
	return _getch()-48;
}

