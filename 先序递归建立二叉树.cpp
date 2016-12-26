#include <stdio.h>
#include <stdlib.h>

typedef struct btnode
{
	char data;
	struct btnode *lc, *rc;
}BiTNode,*BiTree;

BiTree preCreatBiTree();	//先序建立二叉树 
void preOrderTraverse(BiTree T);	//先序遍历二叉树 
void inOrderTraverse(BiTree T);		//中序遍历二叉树
void postOrderTraverse(BiTree T);	//后序遍历二叉树

int main(){
	printf("Create BiTree\n\n"); 
	printf("please input the sequence:");
	BiTree T = preCreatBiTree();
	if(T==NULL)
	{
		printf("T is NULL");
		return 0;
	}
	printf("先序输出：");
	preOrderTraverse(T);
	printf("\n中序输出：");
	inOrderTraverse(T);
	printf("\n后序输出：");
	postOrderTraverse(T);
    return 0;
}

BiTree preCreatBiTree()
{
	BiTree T;
	char ch;
	scanf("%c",&ch);
	if (ch == '*') T = NULL;
	else
	{	
		T = (BiTree)malloc(sizeof(BiTNode));
		if(T==NULL)
		{
			printf("fail to store %c",ch);
			return T;
		}
		T->data = ch;
		T->lc = preCreatBiTree();
		T->rc = preCreatBiTree();
	}
	return T;
}//preCreatBiTree 

void preOrderTraverse(BiTree T)
{
	if (T)
	{
		printf("%c",T->data);
		if(T->lc != NULL)
		preOrderTraverse(T->lc);
		if(T->rc != NULL)
		preOrderTraverse(T->rc);
	}
}//preOrderTraverse

void inOrderTraverse(BiTree T)
{
	if (T)
	{
		if(T->lc != NULL)
		inOrderTraverse(T->lc);
		printf("%c",T->data);	
		if(T->rc != NULL)
		inOrderTraverse(T->rc);
	}
}//inOrderTraverse

void postOrderTraverse(BiTree T)
{
	if (T)
	{
		if(T->lc != NULL)
		postOrderTraverse(T->lc);
		if(T->rc != NULL)
		postOrderTraverse(T->rc);
		printf("%c",T->data);
	}
}//postOrderTraverse
