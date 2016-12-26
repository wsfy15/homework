#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

typedef struct node
{
	char ch;
	struct node* next;
}Node,*pNode;

typedef struct stack  //��ջ
{
	pNode top;
}Stack;

void InitStack(Stack& s);		//��ʼջ 
void push(Stack& s,char c);		//��ջ 
char pop(Stack& s);				//��ջ 
bool isEmpty(Stack& s);			//�ж�ջ�� 

int main(void)
{
	Stack s;
	InitStack(s);
	char ch;
	int i=0, temp=0, num=1; 
	long long power=1;
	/*
	powerΪ��õ���ÿһλ�˽�������˺�ӵ�����֮��power��10 
	iΪ��������ÿ������Ϊһ��ѭ��
	tempΪ��ǰ�õ���һλ�˽�����
	num�����������ÿ��λ��Ȩֵ��1��2��4������ֵΪ1������������ʱ��1 
	*/ 
	long long result = 0; 
	printf("Conversion for binary to octal\n\n");
	printf("please input:");
	while((ch=getchar())!='#')
	{
		push(s,ch); 		
//����������ַ����Ƕ����������㽫����ջfree�ˣ�ͬʱfflush����#����ʹ�û��������� 
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
	while(!isEmpty(s)) 			//ÿ��λ������������һλ8������ 
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
