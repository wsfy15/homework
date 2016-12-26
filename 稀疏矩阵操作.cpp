#include<stdio.h>
#include<stdlib.h>

#define max 100

struct tuple
{
	int i,j,v;   //i为行号，j为列号，v为值 
};

typedef struct sparmattp
{
	int m, n, t;  //m为行数，n为列数，t为非零元素个数
	struct tuple data[max];
}Spar;

int getInput(int n);				 //得到规范的数字输入，n为1执行判断是否大于0，n为0直接返回数字 
void InitSpar(Spar& s,int m,int n);  //初始化矩阵 
Spar Multiply(Spar a,Spar b);        //矩阵相乘 
Spar Add(Spar a,Spar b);			 //矩阵相加 
void Print(Spar s);					 //打印矩阵 			
int Find(Spar s,int m,int n);		//寻找矩阵第m行第n列的数据 

int main(void)
{
	int m,n,M,N;
	int add_able=0, multi_able_fs=0, multi_able_sf=0;
/*  
	m n分别为第一个矩阵的行列数
	M N分别为第二个矩阵的行列数
	add_able： 两个矩阵相加，multi_able_fs：第一个矩阵乘第二个，multi_able_sf：第二个矩阵乘第一个
	三个able参数：值为1表示可以，值为0表示不行 
*/	
	printf("Matrix add and multiply\n\n"); 
	printf("please input lines of the first matrix:");  //读入第一个矩阵行列数 
	m = getInput(1);
	printf("please input rows of the first matrix:");
	n = getInput(1);	

	printf("please input lines of the second matrix:");	//读入第二个矩阵行列数 
	M = getInput(1);	 
	printf("please input rows of the second matrix:");
 	N = getInput(1);		
	if(m == M && n == N)       //判断两个矩阵能做的操作，先判断，后输入，能提高效率 
	{
		add_able = 1;
		printf("the two matrix can add\n");
	}
	if(n == M )
	{
		multi_able_fs = 1;
		printf("the first matrix can multiply the second one\n");
	}
	if(N == m )
	{
		multi_able_sf = 1;
		printf("the second matrix can multiply the first one\n");
	}
	if(!add_able && !multi_able_sf &&!multi_able_fs)
	{
		if(!add_able)
		printf("the two matrix can not add.because their lines or rows are unequal\n");
		if(!multi_able_sf)
		printf("the two matrix can not multiply. because the number of lines of the first matrix does not equal to the number of rows of the second matrix\n");
		if(!multi_able_fs)
		printf("the two matrix can not multiply. because the number of lines of the second matrix does not equal to the number of rows of the first matrix\n");
		return 0;
	};
	
	Spar first,second;
	printf("now input elements of the first matrix\n");
	InitSpar(first,m,n);
	printf("now input elements of the second matrix\n");
	InitSpar(second,M,N);
		
	if(add_able)		//执行可以做的操作 
	{
		printf("ADD:\n");
		Spar add;
		add = Add(first,second);
		Print(add);
	}
	else 
		printf("the two matrix can not add.\nbecause their lines or rows are unequal\n");	
	if(multi_able_fs)
	{
		printf("first multiply second:\n");
		Spar multiply1;
		multiply1 = Multiply(first,second);
		Print(multiply1);
	}
	else
		printf("the two matrix can not multiply. because the number of lines of the second matrix does not equal to the number of rows of the first matrix\n");		
	if(multi_able_sf)
	{
		printf("second multiply first:\n");
		Spar multiply2;
		multiply2 = Multiply(second,first);
		Print(multiply2);
	}
	else 
		printf("the two matrix can not multiply. because the number of lines of the first matrix does not equal to the number of rows of the second matrix\n");

	return 0;
} 

int getInput(int n)
{
	int flag, i, m;
	char ch[10];
	do{
		flag = 1;
		i = 0;
		scanf("%s",ch);
		while(ch[i] != '\0')   //判断是否为数字字符 
		{
			if(ch[i] < '0' || ch[i] > '9')
				flag = 0;
			i++;
		}
		if(flag)	//都是数字 
		{
			if(!n) 		return	atoi(ch);
			m = atoi(ch);
			fflush(stdin);
			if(m>0) 	return m;
			else flag = 0;
		}
		if(n)
		printf("please input again(only numbers and > 0):");
		else	printf("please input again(only numbers):");
	}while(!flag);	
}//getInput

void InitSpar(Spar& s,int m,int n)
{
	int i,j,v;    //i j为计数器 v为读取的矩阵元素值 
	s.m = m;
	s.n = n;
	s.t = 0;
	for(i=0;i<m;i++)
	{
		fflush(stdin);
		printf("please input %d elements of No.%d row:", n, i+1);
		for(j=0;j<n;j++)
		{ 
			v = getInput(0);
			if(v)		
			{
				if(s.t < max)
				{
					s.data[s.t].i = i+1;
					s.data[s.t].j = j+1;
					s.data[s.t].v = v;
					s.t++;
				}	
				else	printf("The maximum number of stores has been exceeded\n");
			}				
		}
	}
}//InitSpar

Spar Multiply(Spar a,Spar b)
{
	Spar c;
	c.m = a.m;
	c.n = b.n;
	int tc=0;
	if(!(a.t*b.t))   //相乘结果无非0元素 
	{
		c.t = 0;
		return c;
	}
	int k, num, A, B;
	for(int i=1;i<=c.m;i++)
	{
		for(int j=1;j<=c.n;j++)
		{
			num = 0;
			k = 1;
			while(k <= a.n)
			{
				A = Find(a,i,k);				
				B = Find(b,k,j);
				if(A && B)
					num += A*B; 
				k++;	
			}
			if(num)
			{
				c.data[tc].i = i;
				c.data[tc].j = j;
				c.data[tc++].v = num;
			}
		}
	}
	return c;
}//Multiply

Spar Add(Spar a,Spar b)
{
	Spar c;
	c.m = a.m;
	c.n = a.n;
	c.t = 0;
	int ta=0,tb=0;
	for(int i=1;i<=c.m;i++)
		for(int j=1;j<=c.n;j++)
		{
			c.data[c.t].v = 0;
			if(a.data[ta].i == i && a.data[ta].j == j)
			{
				c.data[c.t].v += a.data[ta].v;
				ta++;
			}
				 
			if(b.data[tb].i == i && b.data[tb].j == j)
			{
				c.data[c.t].v += b.data[tb].v; 
				tb++;
			}	
			if(c.data[c.t].v)	//值不为0才添加到三元组表里 
			{
				c.data[c.t].i = i;
				c.data[c.t].j = j;
				c.t++;
			}
		}
	return c;
}//Add

void Print(Spar s)
{
	int i, j, k=0, v;
	for(i=0;i<s.m;i++)
	{
		for(j=0;j<s.n;j++)   //有非0值的打印，没有的直接打印0 
		{
			if(s.data[k].i == i+1 && s.data[k].j == j+1)  
			{
				v = s.data[k].v;
				k++;
			}
			else v = 0;
			printf("%d	",v);
		}			
		printf("\n");
	}	
}//Print

int Find(Spar s,int m,int n) 
{
	int i;
	for(i=0;i<s.t;i++)
	{
		if(s.data[i].i == m && s.data[i].j == n)
			return s.data[i].v;
	}
	return 0;
}// Find
