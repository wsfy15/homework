#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_VERTEX_NUM 100

typedef struct Edgenode			//�� 
{
	int adjvex;
	int weight;
	struct 	Edgenode* next;
}EdgeNode;

typedef struct Vertexnode		//ͷ��� 
{
	char data;
	EdgeNode* firstedge;
}VertexNode, AdjList[MAX_VERTEX_NUM];

typedef struct       		//�ڽӱ� 
{
	AdjList adjlist;
	int numVertex, numEdge;
}GraphList;

int visited[MAX_VERTEX_NUM];	//�ж��Ƿ���� 
int loop;						//�ж��Ƿ��л� 

int locate(const GraphList g, char ch);		//�ҵ�ch��Ӧ�����g�е�λ�� 
void createGraph(GraphList &g);			//�����ڽӱ� 
bool cycle(const GraphList g);			//�ж��ǲ����޻�ͼ 
void dfs(const GraphList g, int v);		//������ȱ��� 
void FarestVertices(const GraphList g);		//�ҳ�ͼ�о�����Զ��������� 

int main(void)
{
	GraphList g;
	createGraph(g);
	if(!cycle(g))		//�����ڻ�������²�����Զ����Ľ�� 
		FarestVertices(g);	
	else 	
		printf("loop exist\n");	
	return 0;
} 

int locate(const GraphList g, char ch)
{
	for (int i = 0; i < g.numVertex; i++)
		if (g.adjlist[i].data == ch)
			return i;
	printf("%c not found\n", ch);
	return -1;
}//locate

void createGraph(GraphList &g)
{
	int i, w, temp = 0;
	char p, q;
	char num[10]={'\0'};
	EdgeNode *e, *f;
	printf("input the number of vertexes:");	
	do{						//��ֹ����Ĳ������� 
		if(temp)
			printf("input again(number only!!!): ");
		temp = 0;
		memset(num,'\0',sizeof(num));
		scanf("%s",num);
		for(i = 0; num[i] != '\0'; i++) 		//�ж��Ƿ�Ϊ���� 
			if(num[i] > '9' || num[i] < '0')
				temp = 1;
	}while(temp);
	g.numVertex = atoi(num);
	fflush(stdin);
	
	printf("input the number of edges:");
	do{
		if(temp)
			printf("input again(number only!!!): ");
		temp = 0;
		memset(num,'\0',sizeof(num));
		scanf("%s",num);
		for(i = 0; num[i] != '\0'; i++)
			if(num[i] > '9' || num[i] < '0')
				temp = 1;	
	}while(temp);
	g.numEdge = atoi(num);
	fflush(stdin);
	
	for (i = 0; i < g.numVertex; i++)		//����ÿ���������� 
	{		
		g.adjlist[i].firstedge = NULL;
		printf("input No.%d vertex:", i + 1);
		while ((g.adjlist[i].data = getchar()) == '\n')
		{
			fflush(stdin);
			printf("\ninput again:");
		}
		fflush(stdin);
	}
	
	for (i = 0; i < g.numEdge; i++)		//����ÿ���ߵ���Ϣ 
	{
		printf("input i and j of (Vi,Vj) and weight\n");
		
		printf("now input i:");		//������� 
		while ((p = getchar()) == '\n')
		{
			printf("\ninput i again:");
			fflush(stdin);
		}
		fflush(stdin);
		printf("now input j:");
		
		while ((q = getchar()) == '\n')  //�����յ� 
		{
			printf("\ninput j again:");
			fflush(stdin);
		}
		fflush(stdin);
		
		printf("now input weight:");	//����Ȩֵ 
		scanf("%d",&w);
		fflush(stdin);
		
		int m, n;
		m = locate(g, p);
		n = locate(g, q);
		if (-1 == m || -1 == n)		//�Ҳ���ĳ�������Ҳ��� 
		{
			printf("Vi or Vj not found\n");
			i--;			//����ʧ�ܣ���С�������� 
		}
		else
		{			
			e = (EdgeNode*)malloc(sizeof(EdgeNode));
			if (NULL == e)
				printf("fail to malloc edgenode");
			else
			{
				e->adjvex = n;
				e->weight = w;
				e->next = g.adjlist[m].firstedge;
				g.adjlist[m].firstedge = e;
			}
		}
	}
}//createGraph

bool cycle(const GraphList g)
{
	loop = 0;
	int i, j;
	memset(visited,0,sizeof(visited));
	for (i = 0; i < g.numVertex; i++)
		if(1 == loop)
		{
			printf("loop exist\n");
			return true;
		}
		else if (!visited[i])		//δ���ʵĽ��
				dfs(g,i);	
	if(1 == loop)		
		return true;			
	return false;
}//cycle

void dfs(const GraphList g, int v)		
{
	int i, j;
	visited[v] = 1;
	EdgeNode* p = NULL;
	for (i = 0; i < g.numVertex;i++)
	{
		p = g.adjlist[v].firstedge;
		while(p != NULL)
		{
			j = p->adjvex;
			if (visited[j] == 1)	//���ڻ� 
			{
				loop = 1;
				return;
			}				
			else if (!visited[j])   dfs(g, j);
			if(1 == loop)	return;
			p = p->next;
		}
	}			
	visited[v] = 2;
}//dfs

void FarestVertices(const GraphList g)
{
	int i, j, k, temp, max = 0;
	int result[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
	EdgeNode* p = NULL;
	
	//��ʼ��result���� 
	for(i = 0;i < g.numVertex; i++)
		for(j = 0;j < g.numVertex; j++)
	{
		if(i == j)	result[i][j] = 0;
		else result[i][j] = -1;
	}
	for (i = 0; i < g.numVertex; i++)
	{
		p = g.adjlist[i].firstedge;
		while(p != NULL)
		{
			k = p->adjvex;
			result[i][k] = p->weight;
			p = p->next;
		}
	}
	//��ʼ������ 
	//����floyd�㷨˼������Զ���� 
	for (k = 0; k < g.numVertex; k++)
		for (i = 0; i < g.numVertex; i++)
			for (j = 0; j < g.numVertex; j++)
				{
					if(result[i][k] >= 0 && result[k][j] >= 0)	//���Խ�㶼��ʵ�ֵ���ʱ 
						temp = result[i][k] + result[k][j];
					else 	temp = -1; 		
					if (temp > result[i][j] && i != j) 		//�����Լ����Լ������и�Զ����ʱ���� 
					{
						result[i][j] = temp;
						if(temp > max)	max=temp;	//����maxֵ 
					}					
				}	
	//������			
	printf("The longest distance is:%d\n",max);					
	for (i = 0; i < g.numVertex; i++)		//�ҳ����о������max�Ľ�� 
		for (j = 0; j < g.numVertex; j++)
		{
			if(result[i][j] == max)
				printf("%c-->%c\n",g.adjlist[i].data,g.adjlist[j].data);
		}
}//FarestVertices
