#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

typedef struct solider   //士兵的结构，num为编号 
{
	int num;
	struct solider* next;
}sol,*pSol;

pSol creatList(int n);        //创建链表 
bool isNumber(char* str); 	  //判断是否为数字 
void task(int n,int x,int y); //核心算法 
void type_n(int n);           //三种选择，只输入n，或者输入n和x，或者输入n和y 
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
		while(!isNumber(str))           //判断是否为数字 
		{
			printf("input again(only number):");
			scanf("%s",str);
		}	
		choose = atoi(str);
	}while(choose!=1 && choose!=2 &&choose!=3);		//判断是否为三个选项之一

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
	while(x<=n)      //遍历所有可能情况 
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
	
	printf("please input x:");	//输入x并检测是否处于范围内 
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
	
	printf("please input y:");	//输入y并检测是否处于范围内 
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
	for(i=0;i<n-1;i++)                 //创建链表 
	{
		temp->num = i+1;
		temp->next = (pSol)malloc(sizeof(sol));
		temp = temp->next;
	}
	temp->num = n; 		//最后一个结点 
	temp->next = p;		//构成循环链表 
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
	for(i=1;i<x;i++)	list = list->next;   //找到开始的那个 
	while(list->next!=list)		//剩下的人数大于1 
	{
		for(i=1;i<y;i++)   //找到数到的士兵的前一个 士兵 
		{
			pre = list;
			list = list->next;
		}
		pre->next = list->next;  //将数到的士兵的前一个 士兵的后继改为数到的士兵后面的哪个 
		if(list->num == 1)	 	 //如果是加里森，便将整个链表free了，并结束该次task 
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
		free(list);		  		// free数到的士兵 
		list = pre->next;		//进行下次数数 
	}
	printf("X: %d Y: %d\n",x,y);  //while循环结束，加里森是最后一个，输出当前x和y 
}//task
