#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

typedef struct node     
{
	char data;
	struct node* next;
}Node, *pNode;

typedef struct cstack    //链栈
{
	pNode top;
}CStack;

void InitCStack(CStack& c);		// 初始栈 
void push(CStack& c, char x);	//	进栈 
char pop(CStack& c);			//	出栈 
bool isEmpty(CStack& c);		//	判空 

typedef struct cqueue  //链队列 
{
	pNode front;
	pNode rear;
}CQueue;

void InitQueue(CQueue& q);
void EnQueue(CQueue& q, char x);
char DeQueue(CQueue& q);
bool isEmpty(CQueue& q);

int main(void)
{
	CQueue cq;      //创建链栈和链队列并初始化 
	CStack cs;
	InitCStack(cs);
	InitQueue(cq);
	printf("Plalindrome examination\n\n"); 
	char ch;
	printf("please input:"); 
	while((ch=getchar())!='#')	//存进栈和队列 
	{
		push(cs,ch);
		EnQueue(cq,ch);
	}
	while(!isEmpty(cs) && !isEmpty(cq))
	{
		if(pop(cs) != DeQueue(cq))	//利用栈先进后出和队列先进先出的性质判断回文 
		{
			printf("false");
			return 0;
		}
	}
	printf("true");
	return 0;
}

void InitCStack(CStack& c)
{
	c.top = NULL;
}

void push(CStack& c, char x)
{
	pNode p;					//创建新节点存储数据 
	p = (pNode)malloc(sizeof(Node));
	if (!p)
	{
		printf("fail to malloc");
		return;
	}
	p->data = x;           
	p->next = c.top;        //将新阶段放进栈里 
	c.top = p;
}

char pop(CStack& c)
{
	if (isEmpty(c))   //判断不是空栈才能输出 
	{
		printf("isEmpty");
		return NULL;
	}
	char data = c.top->data;
	pNode temp = c.top->next;    //处理掉栈顶元素，并让c.top指向新栈顶 
	free(c.top);
	c.top = temp;
	return data;
}

bool isEmpty(CStack& c)
{
	return c.top == NULL;
}

void InitQueue(CQueue& q)
{
	q.front = (pNode)malloc(sizeof(Node));
	if (q.front == NULL) printf("error to malloc");
	q.rear = q.front;  
}

void EnQueue(CQueue& q, char x)
{
	pNode p;
	p = (pNode)malloc(sizeof(Node));
	if (!p)
	{
		printf("fail to malloc");
		return;
	}
	p->data = x;            //用新元素p存储数据， 
	p->next = NULL;
	q.rear->next = p;       //队尾的下一个指向p
	q.rear = p;            // 队尾指向p 
}

char DeQueue(CQueue& q)
{
	if (isEmpty(q))    //先判断不是空队列 
	{
		printf("isEmpty");
		return NULL;
	}
	char data = q.front->next->data;
	pNode p = q.front->next;        //处理掉旧队头，并让q.next指向新队头 
	q.front->next = p->next;
	free(p);
	return data;
}

bool isEmpty(CQueue& q)
{
	return q.front->next == NULL;
}

