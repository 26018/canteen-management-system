#include "MENU.h"

int  main()
{
	system("title ����ʳ��ϵͳ4.9");
	system("color F0");
	ShowWindow(GetConsoleWindow(), 3);
	Path *path;InitPath(path);
	strcat(path->Systempath,"file\\");  //ϵͳ·��
	strcat(path->Filepath,path->Systempath);  // �ļ�·��
	strcat(path->Filepath,"*"); 
	flag:
	switch(indexMenu())
	{
	case 1:index01(path);goto flag;  //////ѧ����
	case 2:index02(path);goto flag;   /////����Ա��
	case 0:exit(0);
	default:printf("\n\n\t\t\t\t\t\t\t\t\t\t\t\tѡ�����1�������!!!");Sleep(1000);goto flag;
	}
	return 0;
}
