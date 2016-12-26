#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_VERTEX_NUM 100

typedef struct Edgenode			//边 
{
	int adjvex;
	int weight;
	struct 	Edgenode* next;
}EdgeNode;

typedef struct Vertexnode		//头结点 
{
	char data;
	EdgeNode* firstedge;
}VertexNode, AdjList[MAX_VERTEX_NUM];

typedef struct       		//邻接表 
{
	AdjList adjlist;
	int numVertex, numEdge;
}GraphList;

int visited[MAX_VERTEX_NUM];	//判断是否访问 
int loop;						//判断是否有环 

int locate(const GraphList g, char ch);		//找到ch对应结点在g中的位置 
void createGraph(GraphList &g);			//创建邻接表 
bool cycle(const GraphList g);			//判断是不是无环图 
void dfs(const GraphList g, int v);		//深度优先遍历 
void FarestVertices(const GraphList g);		//找出图中距离最远的两个结点 

int main(void)
{
	GraphList g;
	createGraph(g);
	if(!cycle(g))		//不存在环的情况下才找最远距离的结点 
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
	do{						//防止输入的不是数字 
		if(temp)
			printf("input again(number only!!!): ");
		temp = 0;
		memset(num,'\0',sizeof(num));
		scanf("%s",num);
		for(i = 0; num[i] != '\0'; i++) 		//判断是否为数字 
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
	
	for (i = 0; i < g.numVertex; i++)		//输入每个结点的名称 
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
	
	for (i = 0; i < g.numEdge; i++)		//输入每条边的信息 
	{
		printf("input i and j of (Vi,Vj) and weight\n");
		
		printf("now input i:");		//输入起点 
		while ((p = getchar()) == '\n')
		{
			printf("\ninput i again:");
			fflush(stdin);
		}
		fflush(stdin);
		printf("now input j:");
		
		while ((q = getchar()) == '\n')  //输入终点 
		{
			printf("\ninput j again:");
			fflush(stdin);
		}
		fflush(stdin);
		
		printf("now input weight:");	//输入权值 
		scanf("%d",&w);
		fflush(stdin);
		
		int m, n;
		m = locate(g, p);
		n = locate(g, q);
		if (-1 == m || -1 == n)		//找不到某个结点或都找不到 
		{
			printf("Vi or Vj not found\n");
			i--;			//输入失败，减小计数次数 
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
		else if (!visited[i])		//未访问的结点
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
			if (visited[j] == 1)	//存在环 
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
	
	//初始化result数组 
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
	//初始化结束 
	//利用floyd算法思想求最远距离 
	for (k = 0; k < g.numVertex; k++)
		for (i = 0; i < g.numVertex; i++)
			for (j = 0; j < g.numVertex; j++)
				{
					if(result[i][k] >= 0 && result[k][j] >= 0)	//两对结点都能实现到达时 
						temp = result[i][k] + result[k][j];
					else 	temp = -1; 		
					if (temp > result[i][j] && i != j) 		//不是自己到自己的且有更远距离时更新 
					{
						result[i][j] = temp;
						if(temp > max)	max=temp;	//更新max值 
					}					
				}	
	//输出结果			
	printf("The longest distance is:%d\n",max);					
	for (i = 0; i < g.numVertex; i++)		//找出所有距离等于max的结点 
		for (j = 0; j < g.numVertex; j++)
		{
			if(result[i][j] == max)
				printf("%c-->%c\n",g.adjlist[i].data,g.adjlist[j].data);
		}
}//FarestVertices
