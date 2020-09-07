//转专业
//17052106
//张润生
//2018/1/7
//菜单驱动的学生信息管理系统
//Windows 10/Visual Studio 2017下编译运行通过

#define _CRT_SECURE_NO_WARNINGS//Visual Studio 2017使用scanf()等函数需加的宏

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

enum//各种数据最大长度
{
	NAME_SIZE = 50,//姓名长度
	CLASS_SIZE = 50,//班号长度
	MAJOR_SIZE = 50,//专业长度
	NUM_SIZE = 50,//电话号码长度
	EMA_SIZE = 50,//电子邮件地址长度
	fNAME_SIZE = 50//文件名长度
};



typedef struct
{
	unsigned long	m_stuID;//学号,唯一
	char			m_Name[NAME_SIZE];//姓名
	char			m_Class[CLASS_SIZE];//班号
	char			m_Major[MAJOR_SIZE];//专业
	float			m_Gpa;//加权平均成绩
	char			m_PhoNum[NUM_SIZE];//电话号码
	char			m_EMA[EMA_SIZE];//电子邮件地址
}Info;//链表数据域

typedef struct LINKNODE
{
	Info  m_data;//数据域
	struct LINKNODE *m_next;//指针域
}LinkNode;//链表节点

typedef struct
{
	LinkNode	*m_head;//链表头结点
	int			 m_changed;//是否更改过信息
	char		 m_fname[fNAME_SIZE];//关联的文件名
}List;//学生信息链表


//提前声明各个函数，方便互相调用
int Pswd(const char * pswd);
void WelScr();
void Project();
int Menu(List * This);
int Read(List * This);
int Save(List * This);
void Continue();
void DelAll(List * This);
void Header();
int  In(List * This, FILE * fp);
int Out(List * This, FILE * fp);
int InLine(FILE *fp, Info * p);
int OutLine(FILE *fp, Info * p);
LinkNode * Insert(List * This, Info * pinf);
int SearchMenu(List * This);
LinkNode * Search(LinkNode * head, unsigned long stuID);
void Delete(List * This, LinkNode * p);
void Update(List * This, LinkNode ** pp);


//主函数
int main(void)
{
	if (!Pswd("12345678"))//密码错误退出
		return 0;
	WelScr();//欢迎界面

	List *This = (List *)calloc(1, sizeof(List));//创建链表
	if (!This)
	{
		puts("内存不足！");
		return 0;
	}
	This->m_head = (LinkNode *)calloc(1, sizeof(LinkNode));//创建链表头结点，此节点不存数据，便于统一操作
	if (!This->m_head)
	{
		puts("内存不足！");
		return 0;
	}

	int choice = 0;//列表选项
	do
	{
		Project();//项目界面

		scanf("%d", &choice);
		system("cls");

		switch (choice)
		{
		case 0://退出程序
			free(This->m_head);//释放main中函数创建的动态内存
			free(This);//释放main中函数创建的动态内存
			return 0;
			break;
		case 1://新建项目
			Menu(This);//直接进入项目界面
			break;
		case 2://打开已有项目并排序
			Read(This);//进入调用扩展项目界面的函数
			break;
		default:
			puts("输入选择错误！");
			break;
		}
		Continue();
	} while (choice);
	return 0;
}


//口令验证，参数为设定的密码，返回是否通过验证
int Pswd(const char * pswd)
{
	int i = 3;//密码输入次数，共三次
	char inp = 0;//临时存储输入的密码字符
	puts("请输入密码，共有三次机会");
	while (1)
	{
		int charWrong = 0;//字符对应出现错误，假设为无错
		int sizeWrong = 1;//输入密码长度出现错误，假设为有错
		int j = -1;//输入密码对应的数组下标
		while (1)
		{
			inp = _getch();//无回显式获取字符
			j++;
			if (inp == '\r')//若是回车符
			{
				if (pswd[j] == '\0')//若键入Enter时对应密码数组的结束符
					sizeWrong = 0;//密码长度无错
				break;
			}
			putchar('*');//回显'*'，用于输入反馈
			if (inp != pswd[j])//若键入密码字符与密码数组对应元素不同
				charWrong = 1;
		}
		putchar('\n');//密码输入结束，换行（上一个键入的换行符被_getch()读了）
		i--;


		if (charWrong || sizeWrong)//若字符错误或长度错误
		{
			if (i >= 1)
				printf("密码错误，还有%d次输入机会\n", i);//显示剩余次数
			else
			{
				puts("输入密码3次错误！");
				system("PAUSE");
				break;
			}
		}
		else//通过验证
		{
			puts("密码输入正确！");
			Continue();
			return 1;
		}
	}
	return 0;//未通过验证
}

//欢迎界面
void WelScr()
{
	puts("        |*********************************************************|");
	puts("        |                                                         |");
	puts("        |                    学生信息管理系统                     |");
	puts("        |                      Version 1.0                        |");
	puts("        |                                                         |");
	puts("        |                                                         |");
	puts("        |       Copyright(c) by 张润生.All rights reserved.       |");
	puts("        |                       2018/12/25                        |");
	puts("        |                                                         |");
	puts("        |*********************************************************|");

	Continue();
}

//选择项目界面
void Project()
{
	puts("        |*********************************************************|");
	puts("        |                                                         |");
	puts("        |                输入选择，创建项目：                     |");
	puts("        |                      1：新建项目                        |");
	puts("        |                      2：打开已有项目并排序              |");
	puts("        |                      0：退出程序                        |");
	puts("        |                                                         |");
	puts("        |*********************************************************|");
}

//项目内部菜单，参数为链表指针，成功运行则返回0
int Menu(List * This)
{
	int choice = 0;//选择数字
	do
	{
		puts("        |*********************************************************|");
		puts("        |                                                         |");
		puts("        |                输入选择功能：                           |");
		puts("        |                      1：输入全部学生数据并排序存储      |");
		puts("        |                      2：插入学生数据                    |");
		puts("        |                      3：查找编辑                        |");
		puts("        |                      4：输出全部学生信息                |");
		puts("        |                      5：保存                            |");
		puts("        |                      0：退出项目                        |");
		puts("        |                                                         |");
		puts("        |*********************************************************|");

		scanf("%d", &choice);
		system("cls");

		Info inf;//临时存储学生信息
		int save_choice = 0;//是否保存更改选项
		switch (choice)
		{
		case 0://退出项目
			if (This->m_changed)//若做过更改
			{
				puts("是否保存所做更改？（0：否，非0数字：是）");
				scanf("%d", &save_choice);
				if (save_choice)//要保存更改
					Save(This);//保存
			}
			DelAll(This);//删除链表信息
			return 0;
			break;
		case 1://输入全部学生数据并排序存储
			puts("请输入全部学生信息,直到输入的电子邮件地址尾字符为'\\'为止（不读入'\\'）");
			Header();//表头
			In(This, stdin);//输入全部学生的数据
			This->m_changed = 1;//进行了更改
			break;
		case 2://插入学生数据 
			puts("请输入一个学生的信息。");
			Header();
			InLine(stdin, &inf);//输入
			Insert(This, &inf);//插入
			This->m_changed = 1;//进行了更改
			break;
		case 3://查找编辑
			SearchMenu(This);//进入搜索编辑界面
			break;
		case 4://输出全部学生信息
			Header();
			Out(This, stdout);
			break;
		case 5://保存
			Save(This);
			break;
		default:
			puts("输入的选择不正确！");
			break;
		}
		Continue();
	} while (choice);
	return 0;
}

//读取文件并打开项目内部菜单，参数为链表指针，返回是否打开成功
int Read(List * This)
{
	puts("打开已有文件，请输入文件名：");
	scanf("%s", This->m_fname);

	FILE *fp = fopen(This->m_fname, "r");//打开文件
	if (!fp)
	{
		puts("Read打开文件错误！");
		return 0;
	}
	if (In(This, fp) < 0)//若读取失败
	{
		puts("In读取文件错误！");
		return 0;
	}
	fclose(fp);//关闭文件
	puts("打开成功！");

	Continue();

	Menu(This);//进入选择功能界面
	return 1;//打开成功
}

//保存文件，参数为链表指针，返回是否保存成功
int Save(List * This)
{
	if (!strlen(This->m_fname))//若无文件名
	{
		puts("请输入要保存的文件名,若无此文件则新建");
		scanf("%s", This->m_fname);
	}
	FILE *fp = fopen(This->m_fname, "w");//打开文件
	if (!fp)
	{
		puts("Save打开文件错误！");
		return 0;
	}
	if (Out(This, fp) < 0)//使用Out将全部数据输出至文件
	{
		puts("Out保存文件错误！");
		return 0;
	}

	fclose(fp);//关闭文件
	puts("保存成功！");

	This->m_changed = 0;//是否更改的标识符清零
	return 1;//打开成功
}

//选择功能完成后的切换
void Continue()
{
	system("PAUSE");
	system("cls");
}





//保留链表结构，删除链表全部链接数据，清空链表数据，参数为链表指针
void DelAll(List * This)
{
	while (This->m_head->m_next)//删除链表全部链接数据
		Delete(This, This->m_head->m_next);
	for (size_t i = 0; i < fNAME_SIZE; i++)//文件名置空
		This->m_fname[i] = 0;
	This->m_head->m_next = NULL;//头结点指针置空
}

//表头
void Header()
{
	printf("%-16s%-16s%-16s%-16s%-16s%-16s%-16s\n",
		"学号",
		"姓名",
		"班号",
		"专业",
		"加权平均",
		"电话号码",
		"电子邮件地址");
}

//输入全部学生的数据，成功则返回1
int  In(List * This, FILE * fp)
{
	while (!feof(fp))
	{
		Info inf;//保存输入的节点
		char *p = NULL;
		if (InLine(fp, &inf) == EOF)//若遇到文件末尾跳出循环
			break;
		if (p = strrchr(inf.m_EMA, '\\'))//若输入的最后电子邮箱地址一个含'\\'
		{
			*p = 0;
			Insert(This, &inf);
			break;
		}
		else//直到结束符为止一直输入
			Insert(This, &inf);
	}
	return 1;
}

//输出全部学生的数据，返回是否有输出数据
int Out(List * This, FILE * fp)
{
	if (!This->m_head->m_next)//表空
		return 0;
	LinkNode *p = This->m_head->m_next;
	while (p)//遍历输出
	{
		OutLine(fp, &p->m_data);
		p = p->m_next;
	}
	return 1;
}

//输入一个学生的数据，参数为文件指针，学生信息指针，返回 fscanf()返回值。
int InLine(FILE *fp, Info * p)
{
	return fscanf(fp, "%lu %s %s %s %f %s %s",
		&p->m_stuID,
		p->m_Name,
		p->m_Class,
		p->m_Major,
		&p->m_Gpa,
		p->m_PhoNum,
		p->m_EMA);
}

//输出一个学生的数据，参数为文件指针，学生信息指针，返回fprintf()返回值
int OutLine(FILE *fp, Info * p)
{
	return fprintf(fp, "%-16lu%-16s%-16s%-16s%-16f%-16s%-16s\n",
		p->m_stuID,
		p->m_Name,
		p->m_Class,
		p->m_Major,
		p->m_Gpa,
		p->m_PhoNum,
		p->m_EMA);
}

//按顺序插入节点，参数为学生信息链表指针和要插入的学生信息指针
LinkNode * Insert(List * This, Info * pinf)
{
	if (!This->m_head)//发生错误
	{
		puts("This->m_head为nullptr！");
		return NULL;
	}
	LinkNode * prepeat = Search(This->m_head, pinf->m_stuID);//与插入的学号重复的节点的指针

	if (prepeat)//重复
	{
		puts("发现重复");
		puts("是保留原来的学生信息（按0）");
		Header();
		OutLine(stdout, &prepeat->m_data);
		puts("还是保留更新的学生信息（按非0数字键）");
		Header();
		OutLine(stdout, pinf);
		int choice = 0;
		scanf("%d", &choice);
		if (choice)//保留更新的学生信息
			prepeat->m_data = *pinf;
		return prepeat;//退出此次插入
	}

	LinkNode *p = (LinkNode *)calloc(1, sizeof(LinkNode));//要插入的节点

	if (!p)
	{
		puts("内存不足！");
		return p;
	}
	p->m_data = *pinf;

	LinkNode *pr = This->m_head;//当前遍历的指针
	LinkNode *temp = This->m_head;//保存当前遍历的的节点的前面一个节点的指针

	while (pr->m_next&&pr->m_data.m_stuID < pinf->m_stuID)//找到插入位置，保存此位置和前一位置的指针，或到尾节点跳出
	{
		temp = pr;
		pr = pr->m_next;
	}

	if (pr->m_data.m_stuID >= pinf->m_stuID)//此条件作用是在尾节点跳出时判断，选择插入尾节点前还是尾节点后
	{
		pr = temp;
		p->m_next = pr->m_next;
		pr->m_next = p;
	}
	else
		pr->m_next = p;
	return p;
}

//搜索编辑的选择菜单
int SearchMenu(List * This)
{
	unsigned long stuID = 0;//查找的学号
	int choice = 0;//选择数字

	puts("请输入要查找学生的学号");
	scanf("%lu", &stuID);

	LinkNode *sresult = Search(This->m_head->m_next, stuID);//查找
	if (!sresult)
	{
		puts("未找到该学生！");
		return 0;
	}

	do
	{
		puts("        |*********************************************************|");
		puts("        |                                                         |");
		puts("        |                查找编辑：                               |");
		puts("        |                      1：删除此学生信息                  |");
		puts("        |                      2：更改此学生信息                  |");
		puts("        |                      0：退出编辑                        |");
		puts("        |                                                         |");
		puts("        |*********************************************************|");
		printf("%8c", ' ');//begin 搜索结果，与上面界面对齐
		Header();
		printf("%8c", ' ');
		OutLine(stdout, &sresult->m_data);//end

		scanf("%d", &choice);
		system("cls");

		switch (choice)
		{
		case 0://退出编辑
			return 1;
			break;
		case 1://删除此学生信息
			Delete(This, sresult);
			This->m_changed = 1;//进行了更改
			return 0;//删除后直接退出
			break;
		case 2://更改此学生信息
			Update(This, &sresult);
			This->m_changed = 1;//进行了更改
			break;
		default:
			puts("输入选择错误！");
			break;
		}
		Continue();
	} while (choice);
	return 1;
}






//按学号进行搜索，参数为开始寻找的节点指针和学号，返回查找到的节点指针
LinkNode * Search(LinkNode * head, unsigned long stuID)
{
	LinkNode *p = head;//遍历用指针
	if (!head)
		return NULL;
	while (stuID != p->m_data.m_stuID)//找到则退出循环
	{
		if (!p->m_next)
			return NULL;
		p = p->m_next;
	}
	return p;
}

//删除单个节点，参数为链表指针和要删除的节点的指针
void Delete(List * This, LinkNode * p)
{
	LinkNode *pr = This->m_head;
	while (pr->m_next != p)
		pr = pr->m_next;
	pr->m_next = p->m_next;
	free(p);
}

//更新学生信息，参数为学生信息链表指针和指向要更改的学生的指针的二级指针
void Update(List * This, LinkNode ** pp)
{
	Info inf;//更改后的学生信息
	LinkNode *pr;//临时指针，用于删除原信息

	puts("请输入一位学生的信息");
	Header();
	InLine(stdin, &inf);

	if (inf.m_stuID == (*pp)->m_data.m_stuID)//若不更改学号
		(*pp)->m_data = inf;//直接对现在信息修改
	else//若学号更改，则重新插入，并更改搜索编辑的指针
	{
		pr = *pp;
		*pp = Insert(This, &inf);
		Delete(This, pr);
	}
}