//תרҵ
//17052106
//������
//2018/1/7
//�˵�������ѧ����Ϣ����ϵͳ
//Windows 10/Visual Studio 2017�±�������ͨ��

#define _CRT_SECURE_NO_WARNINGS//Visual Studio 2017ʹ��scanf()�Ⱥ�����ӵĺ�

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

enum//����������󳤶�
{
	NAME_SIZE = 50,//��������
	CLASS_SIZE = 50,//��ų���
	MAJOR_SIZE = 50,//רҵ����
	NUM_SIZE = 50,//�绰���볤��
	EMA_SIZE = 50,//�����ʼ���ַ����
	fNAME_SIZE = 50//�ļ�������
};



typedef struct
{
	unsigned long	m_stuID;//ѧ��,Ψһ
	char			m_Name[NAME_SIZE];//����
	char			m_Class[CLASS_SIZE];//���
	char			m_Major[MAJOR_SIZE];//רҵ
	float			m_Gpa;//��Ȩƽ���ɼ�
	char			m_PhoNum[NUM_SIZE];//�绰����
	char			m_EMA[EMA_SIZE];//�����ʼ���ַ
}Info;//����������

typedef struct LINKNODE
{
	Info  m_data;//������
	struct LINKNODE *m_next;//ָ����
}LinkNode;//����ڵ�

typedef struct
{
	LinkNode	*m_head;//����ͷ���
	int			 m_changed;//�Ƿ���Ĺ���Ϣ
	char		 m_fname[fNAME_SIZE];//�������ļ���
}List;//ѧ����Ϣ����


//��ǰ�����������������㻥�����
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


//������
int main(void)
{
	if (!Pswd("12345678"))//��������˳�
		return 0;
	WelScr();//��ӭ����

	List *This = (List *)calloc(1, sizeof(List));//��������
	if (!This)
	{
		puts("�ڴ治�㣡");
		return 0;
	}
	This->m_head = (LinkNode *)calloc(1, sizeof(LinkNode));//��������ͷ��㣬�˽ڵ㲻�����ݣ�����ͳһ����
	if (!This->m_head)
	{
		puts("�ڴ治�㣡");
		return 0;
	}

	int choice = 0;//�б�ѡ��
	do
	{
		Project();//��Ŀ����

		scanf("%d", &choice);
		system("cls");

		switch (choice)
		{
		case 0://�˳�����
			free(This->m_head);//�ͷ�main�к��������Ķ�̬�ڴ�
			free(This);//�ͷ�main�к��������Ķ�̬�ڴ�
			return 0;
			break;
		case 1://�½���Ŀ
			Menu(This);//ֱ�ӽ�����Ŀ����
			break;
		case 2://��������Ŀ������
			Read(This);//���������չ��Ŀ����ĺ���
			break;
		default:
			puts("����ѡ�����");
			break;
		}
		Continue();
	} while (choice);
	return 0;
}


//������֤������Ϊ�趨�����룬�����Ƿ�ͨ����֤
int Pswd(const char * pswd)
{
	int i = 3;//�������������������
	char inp = 0;//��ʱ�洢����������ַ�
	puts("���������룬�������λ���");
	while (1)
	{
		int charWrong = 0;//�ַ���Ӧ���ִ��󣬼���Ϊ�޴�
		int sizeWrong = 1;//�������볤�ȳ��ִ��󣬼���Ϊ�д�
		int j = -1;//���������Ӧ�������±�
		while (1)
		{
			inp = _getch();//�޻���ʽ��ȡ�ַ�
			j++;
			if (inp == '\r')//���ǻس���
			{
				if (pswd[j] == '\0')//������Enterʱ��Ӧ��������Ľ�����
					sizeWrong = 0;//���볤���޴�
				break;
			}
			putchar('*');//����'*'���������뷴��
			if (inp != pswd[j])//�����������ַ������������ӦԪ�ز�ͬ
				charWrong = 1;
		}
		putchar('\n');//����������������У���һ������Ļ��з���_getch()���ˣ�
		i--;


		if (charWrong || sizeWrong)//���ַ�����򳤶ȴ���
		{
			if (i >= 1)
				printf("������󣬻���%d���������\n", i);//��ʾʣ�����
			else
			{
				puts("��������3�δ���");
				system("PAUSE");
				break;
			}
		}
		else//ͨ����֤
		{
			puts("����������ȷ��");
			Continue();
			return 1;
		}
	}
	return 0;//δͨ����֤
}

//��ӭ����
void WelScr()
{
	puts("        |*********************************************************|");
	puts("        |                                                         |");
	puts("        |                    ѧ����Ϣ����ϵͳ                     |");
	puts("        |                      Version 1.0                        |");
	puts("        |                                                         |");
	puts("        |                                                         |");
	puts("        |       Copyright(c) by ������.All rights reserved.       |");
	puts("        |                       2018/12/25                        |");
	puts("        |                                                         |");
	puts("        |*********************************************************|");

	Continue();
}

//ѡ����Ŀ����
void Project()
{
	puts("        |*********************************************************|");
	puts("        |                                                         |");
	puts("        |                ����ѡ�񣬴�����Ŀ��                     |");
	puts("        |                      1���½���Ŀ                        |");
	puts("        |                      2����������Ŀ������              |");
	puts("        |                      0���˳�����                        |");
	puts("        |                                                         |");
	puts("        |*********************************************************|");
}

//��Ŀ�ڲ��˵�������Ϊ����ָ�룬�ɹ������򷵻�0
int Menu(List * This)
{
	int choice = 0;//ѡ������
	do
	{
		puts("        |*********************************************************|");
		puts("        |                                                         |");
		puts("        |                ����ѡ���ܣ�                           |");
		puts("        |                      1������ȫ��ѧ�����ݲ�����洢      |");
		puts("        |                      2������ѧ������                    |");
		puts("        |                      3�����ұ༭                        |");
		puts("        |                      4�����ȫ��ѧ����Ϣ                |");
		puts("        |                      5������                            |");
		puts("        |                      0���˳���Ŀ                        |");
		puts("        |                                                         |");
		puts("        |*********************************************************|");

		scanf("%d", &choice);
		system("cls");

		Info inf;//��ʱ�洢ѧ����Ϣ
		int save_choice = 0;//�Ƿ񱣴����ѡ��
		switch (choice)
		{
		case 0://�˳���Ŀ
			if (This->m_changed)//����������
			{
				puts("�Ƿ񱣴��������ģ���0���񣬷�0���֣��ǣ�");
				scanf("%d", &save_choice);
				if (save_choice)//Ҫ�������
					Save(This);//����
			}
			DelAll(This);//ɾ��������Ϣ
			return 0;
			break;
		case 1://����ȫ��ѧ�����ݲ�����洢
			puts("������ȫ��ѧ����Ϣ,ֱ������ĵ����ʼ���ַβ�ַ�Ϊ'\\'Ϊֹ��������'\\'��");
			Header();//��ͷ
			In(This, stdin);//����ȫ��ѧ��������
			This->m_changed = 1;//�����˸���
			break;
		case 2://����ѧ������ 
			puts("������һ��ѧ������Ϣ��");
			Header();
			InLine(stdin, &inf);//����
			Insert(This, &inf);//����
			This->m_changed = 1;//�����˸���
			break;
		case 3://���ұ༭
			SearchMenu(This);//���������༭����
			break;
		case 4://���ȫ��ѧ����Ϣ
			Header();
			Out(This, stdout);
			break;
		case 5://����
			Save(This);
			break;
		default:
			puts("�����ѡ����ȷ��");
			break;
		}
		Continue();
	} while (choice);
	return 0;
}

//��ȡ�ļ�������Ŀ�ڲ��˵�������Ϊ����ָ�룬�����Ƿ�򿪳ɹ�
int Read(List * This)
{
	puts("�������ļ����������ļ�����");
	scanf("%s", This->m_fname);

	FILE *fp = fopen(This->m_fname, "r");//���ļ�
	if (!fp)
	{
		puts("Read���ļ�����");
		return 0;
	}
	if (In(This, fp) < 0)//����ȡʧ��
	{
		puts("In��ȡ�ļ�����");
		return 0;
	}
	fclose(fp);//�ر��ļ�
	puts("�򿪳ɹ���");

	Continue();

	Menu(This);//����ѡ���ܽ���
	return 1;//�򿪳ɹ�
}

//�����ļ�������Ϊ����ָ�룬�����Ƿ񱣴�ɹ�
int Save(List * This)
{
	if (!strlen(This->m_fname))//�����ļ���
	{
		puts("������Ҫ������ļ���,���޴��ļ����½�");
		scanf("%s", This->m_fname);
	}
	FILE *fp = fopen(This->m_fname, "w");//���ļ�
	if (!fp)
	{
		puts("Save���ļ�����");
		return 0;
	}
	if (Out(This, fp) < 0)//ʹ��Out��ȫ������������ļ�
	{
		puts("Out�����ļ�����");
		return 0;
	}

	fclose(fp);//�ر��ļ�
	puts("����ɹ���");

	This->m_changed = 0;//�Ƿ���ĵı�ʶ������
	return 1;//�򿪳ɹ�
}

//ѡ������ɺ���л�
void Continue()
{
	system("PAUSE");
	system("cls");
}





//��������ṹ��ɾ������ȫ���������ݣ�����������ݣ�����Ϊ����ָ��
void DelAll(List * This)
{
	while (This->m_head->m_next)//ɾ������ȫ����������
		Delete(This, This->m_head->m_next);
	for (size_t i = 0; i < fNAME_SIZE; i++)//�ļ����ÿ�
		This->m_fname[i] = 0;
	This->m_head->m_next = NULL;//ͷ���ָ���ÿ�
}

//��ͷ
void Header()
{
	printf("%-16s%-16s%-16s%-16s%-16s%-16s%-16s\n",
		"ѧ��",
		"����",
		"���",
		"רҵ",
		"��Ȩƽ��",
		"�绰����",
		"�����ʼ���ַ");
}

//����ȫ��ѧ�������ݣ��ɹ��򷵻�1
int  In(List * This, FILE * fp)
{
	while (!feof(fp))
	{
		Info inf;//��������Ľڵ�
		char *p = NULL;
		if (InLine(fp, &inf) == EOF)//�������ļ�ĩβ����ѭ��
			break;
		if (p = strrchr(inf.m_EMA, '\\'))//������������������ַһ����'\\'
		{
			*p = 0;
			Insert(This, &inf);
			break;
		}
		else//ֱ��������Ϊֹһֱ����
			Insert(This, &inf);
	}
	return 1;
}

//���ȫ��ѧ�������ݣ������Ƿ����������
int Out(List * This, FILE * fp)
{
	if (!This->m_head->m_next)//���
		return 0;
	LinkNode *p = This->m_head->m_next;
	while (p)//�������
	{
		OutLine(fp, &p->m_data);
		p = p->m_next;
	}
	return 1;
}

//����һ��ѧ�������ݣ�����Ϊ�ļ�ָ�룬ѧ����Ϣָ�룬���� fscanf()����ֵ��
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

//���һ��ѧ�������ݣ�����Ϊ�ļ�ָ�룬ѧ����Ϣָ�룬����fprintf()����ֵ
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

//��˳�����ڵ㣬����Ϊѧ����Ϣ����ָ���Ҫ�����ѧ����Ϣָ��
LinkNode * Insert(List * This, Info * pinf)
{
	if (!This->m_head)//��������
	{
		puts("This->m_headΪnullptr��");
		return NULL;
	}
	LinkNode * prepeat = Search(This->m_head, pinf->m_stuID);//������ѧ���ظ��Ľڵ��ָ��

	if (prepeat)//�ظ�
	{
		puts("�����ظ�");
		puts("�Ǳ���ԭ����ѧ����Ϣ����0��");
		Header();
		OutLine(stdout, &prepeat->m_data);
		puts("���Ǳ������µ�ѧ����Ϣ������0���ּ���");
		Header();
		OutLine(stdout, pinf);
		int choice = 0;
		scanf("%d", &choice);
		if (choice)//�������µ�ѧ����Ϣ
			prepeat->m_data = *pinf;
		return prepeat;//�˳��˴β���
	}

	LinkNode *p = (LinkNode *)calloc(1, sizeof(LinkNode));//Ҫ����Ľڵ�

	if (!p)
	{
		puts("�ڴ治�㣡");
		return p;
	}
	p->m_data = *pinf;

	LinkNode *pr = This->m_head;//��ǰ������ָ��
	LinkNode *temp = This->m_head;//���浱ǰ�����ĵĽڵ��ǰ��һ���ڵ��ָ��

	while (pr->m_next&&pr->m_data.m_stuID < pinf->m_stuID)//�ҵ�����λ�ã������λ�ú�ǰһλ�õ�ָ�룬��β�ڵ�����
	{
		temp = pr;
		pr = pr->m_next;
	}

	if (pr->m_data.m_stuID >= pinf->m_stuID)//��������������β�ڵ�����ʱ�жϣ�ѡ�����β�ڵ�ǰ����β�ڵ��
	{
		pr = temp;
		p->m_next = pr->m_next;
		pr->m_next = p;
	}
	else
		pr->m_next = p;
	return p;
}

//�����༭��ѡ��˵�
int SearchMenu(List * This)
{
	unsigned long stuID = 0;//���ҵ�ѧ��
	int choice = 0;//ѡ������

	puts("������Ҫ����ѧ����ѧ��");
	scanf("%lu", &stuID);

	LinkNode *sresult = Search(This->m_head->m_next, stuID);//����
	if (!sresult)
	{
		puts("δ�ҵ���ѧ����");
		return 0;
	}

	do
	{
		puts("        |*********************************************************|");
		puts("        |                                                         |");
		puts("        |                ���ұ༭��                               |");
		puts("        |                      1��ɾ����ѧ����Ϣ                  |");
		puts("        |                      2�����Ĵ�ѧ����Ϣ                  |");
		puts("        |                      0���˳��༭                        |");
		puts("        |                                                         |");
		puts("        |*********************************************************|");
		printf("%8c", ' ');//begin ���������������������
		Header();
		printf("%8c", ' ');
		OutLine(stdout, &sresult->m_data);//end

		scanf("%d", &choice);
		system("cls");

		switch (choice)
		{
		case 0://�˳��༭
			return 1;
			break;
		case 1://ɾ����ѧ����Ϣ
			Delete(This, sresult);
			This->m_changed = 1;//�����˸���
			return 0;//ɾ����ֱ���˳�
			break;
		case 2://���Ĵ�ѧ����Ϣ
			Update(This, &sresult);
			This->m_changed = 1;//�����˸���
			break;
		default:
			puts("����ѡ�����");
			break;
		}
		Continue();
	} while (choice);
	return 1;
}






//��ѧ�Ž�������������Ϊ��ʼѰ�ҵĽڵ�ָ���ѧ�ţ����ز��ҵ��Ľڵ�ָ��
LinkNode * Search(LinkNode * head, unsigned long stuID)
{
	LinkNode *p = head;//������ָ��
	if (!head)
		return NULL;
	while (stuID != p->m_data.m_stuID)//�ҵ����˳�ѭ��
	{
		if (!p->m_next)
			return NULL;
		p = p->m_next;
	}
	return p;
}

//ɾ�������ڵ㣬����Ϊ����ָ���Ҫɾ���Ľڵ��ָ��
void Delete(List * This, LinkNode * p)
{
	LinkNode *pr = This->m_head;
	while (pr->m_next != p)
		pr = pr->m_next;
	pr->m_next = p->m_next;
	free(p);
}

//����ѧ����Ϣ������Ϊѧ����Ϣ����ָ���ָ��Ҫ���ĵ�ѧ����ָ��Ķ���ָ��
void Update(List * This, LinkNode ** pp)
{
	Info inf;//���ĺ��ѧ����Ϣ
	LinkNode *pr;//��ʱָ�룬����ɾ��ԭ��Ϣ

	puts("������һλѧ������Ϣ");
	Header();
	InLine(stdin, &inf);

	if (inf.m_stuID == (*pp)->m_data.m_stuID)//��������ѧ��
		(*pp)->m_data = inf;//ֱ�Ӷ�������Ϣ�޸�
	else//��ѧ�Ÿ��ģ������²��룬�����������༭��ָ��
	{
		pr = *pp;
		*pp = Insert(This, &inf);
		Delete(This, pr);
	}
}