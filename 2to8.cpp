#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

typedef struct node
{
	char ch;
	struct node* next;
}Node,*pNode;

typedef struct stack  //链栈
{
	pNode top;
}Stack;

void InitStack(Stack& s);		//初始栈 
void push(Stack& s,char c);		//入栈 
char pop(Stack& s);				//出栈 
bool isEmpty(Stack& s);			//判断栈空 

int main(void)
{
	Stack s;
	InitStack(s);
	char ch;
	int i=0, temp=0, num=1; 
	long long power=1;
	/*
	power为与得到的每一位八进制数相乘后加到结果里，之和power乘10 
	i为计数器，每加三次为一个循环
	temp为当前得到的一位八进制数
	num保存二进制数每个位的权值（1，2，4），初值为1，计数器触发时归1 
	*/ 
	long long result = 0; 
	printf("Conversion for binary to octal\n\n");
	printf("please input:");
	while((ch=getchar())!='#')
	{
		push(s,ch); 		
//如果输入有字符不是二进制数，便将整个栈free了，同时fflush掉‘#’，使用户重新输入 
		if(ch!= '1' && ch!='0')         
		{
			fflush(stdin);
			printf("input error, input 1 or 0 only\n");
			printf("please input again:");
			while(s.top!=NULL)
				pop(s);
			InitStack(s);
		 } 	 	
	}	
	while(!isEmpty(s)) 			//每三位二进制数代表一位8进制数 
	{
		ch = pop(s);
		if(ch == '1') temp += num;
		num*=2;
		i++;
		if(i%3 == 0)
		{
			result += temp*power;
			temp = 0;
			power*=10;
			num = 1;
		}
	}
	if(temp)
	{
		result += temp*power;
	} 
	printf("%lld",result);
	return 0;
}

void InitStack(Stack& s)
{
	s.top = NULL;
}// InitStack

void push(Stack& s,char c)	
{
	pNode p;
	p = (pNode)malloc(sizeof(Node));
	if(!p)
	{
		printf("fail to alloc");
		return;
	}
	p->ch = c;
	p->next = s.top;
	s.top = p;
}// push

char pop(Stack& s)
{
	if(isEmpty(s))
	{
		printf("isEmpty");
		return NULL;
	}
	char c = s.top->ch;
	pNode p = s.top->next;
	free(s.top);
	s.top = p;
	return c;
}//pop 

bool isEmpty(Stack& s)
{
	return s.top == NULL;
}// isEmpty
