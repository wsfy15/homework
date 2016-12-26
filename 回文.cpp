#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

typedef struct node     
{
	char data;
	struct node* next;
}Node, *pNode;

typedef struct cstack    //��ջ
{
	pNode top;
}CStack;

void InitCStack(CStack& c);		// ��ʼջ 
void push(CStack& c, char x);	//	��ջ 
char pop(CStack& c);			//	��ջ 
bool isEmpty(CStack& c);		//	�п� 

typedef struct cqueue  //������ 
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
	CQueue cq;      //������ջ�������в���ʼ�� 
	CStack cs;
	InitCStack(cs);
	InitQueue(cq);
	printf("Plalindrome examination\n\n"); 
	char ch;
	printf("please input:"); 
	while((ch=getchar())!='#')	//���ջ�Ͷ��� 
	{
		push(cs,ch);
		EnQueue(cq,ch);
	}
	while(!isEmpty(cs) && !isEmpty(cq))
	{
		if(pop(cs) != DeQueue(cq))	//����ջ�Ƚ�����Ͷ����Ƚ��ȳ��������жϻ��� 
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
	pNode p;					//�����½ڵ�洢���� 
	p = (pNode)malloc(sizeof(Node));
	if (!p)
	{
		printf("fail to malloc");
		return;
	}
	p->data = x;           
	p->next = c.top;        //���½׶ηŽ�ջ�� 
	c.top = p;
}

char pop(CStack& c)
{
	if (isEmpty(c))   //�жϲ��ǿ�ջ������� 
	{
		printf("isEmpty");
		return NULL;
	}
	char data = c.top->data;
	pNode temp = c.top->next;    //�����ջ��Ԫ�أ�����c.topָ����ջ�� 
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
	p->data = x;            //����Ԫ��p�洢���ݣ� 
	p->next = NULL;
	q.rear->next = p;       //��β����һ��ָ��p
	q.rear = p;            // ��βָ��p 
}

char DeQueue(CQueue& q)
{
	if (isEmpty(q))    //���жϲ��ǿն��� 
	{
		printf("isEmpty");
		return NULL;
	}
	char data = q.front->next->data;
	pNode p = q.front->next;        //������ɶ�ͷ������q.nextָ���¶�ͷ 
	q.front->next = p->next;
	free(p);
	return data;
}

bool isEmpty(CQueue& q)
{
	return q.front->next == NULL;
}

