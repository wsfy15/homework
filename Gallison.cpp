#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

typedef struct solider   //ʿ���Ľṹ��numΪ��� 
{
	int num;
	struct solider* next;
}sol,*pSol;

pSol creatList(int n);        //�������� 
bool isNumber(char* str); 	  //�ж��Ƿ�Ϊ���� 
void task(int n,int x,int y); //�����㷨 
void type_n(int n);           //����ѡ��ֻ����n����������n��x����������n��y 
void type_n_x(int n);
void type_n_y(int n);

int main(void)
{
	int n,x,y;
	char str[10];
	int choose;
	
	printf("n = the number of soliders\n");
	printf("x = start from NO.x solider\n");
	printf("y = count y everytime\n");
	printf("1:input n\n");
	printf("2:input n and x\n");
	printf("3:input n and y\n");
	
	do{
		printf("please choose the type(1 or 2 or 3):\n");
		scanf("%s",str);
		while(!isNumber(str))           //�ж��Ƿ�Ϊ���� 
		{
			printf("input again(only number):");
			scanf("%s",str);
		}	
		choose = atoi(str);
	}while(choose!=1 && choose!=2 &&choose!=3);		//�ж��Ƿ�Ϊ����ѡ��֮һ

	do{
		printf("please input the number of soliders(more than 1):");
		scanf("%s",str);
		while(!isNumber(str))
		{
			printf("input again(only number):");
			scanf("%s",str);
		}	
		n = atoi(str);
	}while(n <= 1);
	
	switch(choose)
	{
		case 1:
		 	type_n(n); 
			break;
		case 2:
			type_n_x(n);
			break;
		case 3:
			type_n_y(n);
			break;		
	}			
	return 0;
}

void type_n(int n)
{
	int x=1,y=2;
	printf("X: 2 Y: 1\n");
	while(x<=n)      //�������п������ 
	{
		y = 2;
		while(y<=n)
		{
			task(n,x,y);
			y++;
		}
		x++;
	}
}// type_n

void type_n_x(int n)
{
	int x,y=2;
	char str[10];
	
	printf("please input x:");	//����x������Ƿ��ڷ�Χ�� 
	scanf("%s",str);
	while(!isNumber(str))   
	{
		printf("input again(only number):");
		scanf("%s",str);
	}
	x = atoi(str);	
	while(x<=0 || x>n)
	{
		printf("x<=0 or x>n,please input again(1<=x<=%d)",n);
		scanf("%s",str);
		while(!isNumber(str))
		{
		printf("input again(only number):");
		scanf("%s",str);
		}
		x = atoi(str);
	}
	
	if(x == 2) 
	{
		printf("X: 2 Y: 1\n");
		return;
	}
	
	y = 2;
	while(y<=n)
	{
		task(n,x,y);
		y++;
	}
}//type_n_x 

void type_n_y(int n)
{
	int x,y=2;
	char str[10];
	
	printf("please input y:");	//����y������Ƿ��ڷ�Χ�� 
	scanf("%s",str);
	while(!isNumber(str))
	{
		printf("input again(only number):");
		scanf("%s",str);
	}
	y = atoi(str);	
	while(y<=0 || y>n)
	{
		printf("y<=0 or y>n,please input again(1<=y<=%d)",n);
		scanf("%s",str);
		while(!isNumber(str))
		{
		printf("input again(only number):");
		scanf("%s",str);
		}
		y = atoi(str);
	}
	
	if(y == 1) 
	{
		printf("X: 2 Y: 1\n");
		return;
	}
	
	x = 2;	
	while(x<=n)
	{
		task(n,x,y);
		x++;
	}
}//type_n_y 

pSol creatList(int n)
{
	int i;
	pSol p,temp;
	p = (pSol)malloc(sizeof(sol));
	temp = p;
	for(i=0;i<n-1;i++)                 //�������� 
	{
		temp->num = i+1;
		temp->next = (pSol)malloc(sizeof(sol));
		temp = temp->next;
	}
	temp->num = n; 		//���һ����� 
	temp->next = p;		//����ѭ������ 
	return p;
}//creatList

bool isNumber(char *str)  
{
	while(1)
	{
		for(int i=0;str[i]!='\0';i++)
			if(str[i]>'9' || str[i]<'0')					
					return false;
		return true;
	}
}//isNumber

void task(int n,int x,int y)
{
	int i;
	pSol list = creatList(n);	
	pSol pre = list;
	pSol temp = NULL;
	for(i=1;i<x;i++)	list = list->next;   //�ҵ���ʼ���Ǹ� 
	while(list->next!=list)		//ʣ�µ���������1 
	{
		for(i=1;i<y;i++)   //�ҵ�������ʿ����ǰһ�� ʿ�� 
		{
			pre = list;
			list = list->next;
		}
		pre->next = list->next;  //��������ʿ����ǰһ�� ʿ���ĺ�̸�Ϊ������ʿ��������ĸ� 
		if(list->num == 1)	 	 //����Ǽ���ɭ���㽫��������free�ˣ��������ô�task 
		{			
			temp = pre->next;
			pre->next = NULL;
			while(temp!=NULL)
			{
				pre = temp;				
				temp = temp->next;				
				free(pre);
			}
			return;
		}
		free(list);		  		// free������ʿ�� 
		list = pre->next;		//�����´����� 
	}
	printf("X: %d Y: %d\n",x,y);  //whileѭ������������ɭ�����һ���������ǰx��y 
}//task
