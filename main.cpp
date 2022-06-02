#include "MENU.h"

int  main()
{
	system("title 爱上食堂系统4.9");
	system("color F0");
	ShowWindow(GetConsoleWindow(), 3);
	Path *path;InitPath(path);
	strcat(path->Systempath,"file\\");  //系统路径
	strcat(path->Filepath,path->Systempath);  // 文件路径
	strcat(path->Filepath,"*"); 
	flag:
	switch(indexMenu())
	{
	case 1:index01(path);goto flag;  //////学生端
	case 2:index02(path);goto flag;   /////管理员端
	case 0:exit(0);
	default:printf("\n\n\t\t\t\t\t\t\t\t\t\t\t\t选择错误，1秒后重试!!!");Sleep(1000);goto flag;
	}
	return 0;
}
