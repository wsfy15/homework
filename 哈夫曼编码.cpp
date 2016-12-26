#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 256    //8位二进制数最多代表256中字符 
#define MAX 511	 //霍夫曼树的长度： 2*N-1 

typedef unsigned char UCHAR;
typedef struct huffmanNode	//霍夫曼树 
{
	int weight;
	short lc, rc, parent;
}HF, HFNode[MAX];

typedef struct		//编码表 
{
	char* ch;
}code, Code[N];

void compress(const char* filename, const char* destname);	//压缩函数 
void decompress(const char* filename, const char* destname);	//解压缩函数 
void select(int& s1, int& s2, HFNode HFN, int k);	//选择两个权最小（不为0）的元素下标 
void encode(Code& route, HFNode HFN);	//编码函数 
UCHAR chars_to_bits(const char* ch);	//实现从byte到bit的转换  

int main(void) {
	printf("please input your choice: 1: compress 2: decompress 0: exit\n");
	int choice, temp = 1;
	char filename[20], destname[20];
	do {
		scanf("%d", &choice);
		fflush(stdin);
		if (choice != 1 && choice != 2 && choice != 0)
			printf("input error.please input again:");
		else temp = 0;
	} while (temp);
	if (1 == choice)
	{
		printf("please input the filename you want to compress:");
		scanf("%s", filename);
		fflush(stdin);
		printf("please input the destname for compression:");
		scanf("%s", destname);
		compress(filename, destname);
	}
	if (2 == choice)
	{
		printf("please input the filename you want to decompress:");
		scanf("%s", filename);
		printf("please input the destname for decompression:");
		scanf("%s", destname);
		decompress(filename, destname);
	}
	return 0;
}

void compress(const char* filename, const char* destname)
{
	FILE* source = fopen(filename, "r");
	if (source == NULL)
	{
		printf("source 	fail to open source file");
		return;
	}
	HFNode huff;
	int i, s1, s2, count = 0,total=0;	//count：记录有多少种字符，total：记录字符总数 
	char str[1026] = {'\0'};
	for (i = 0; i<MAX; i++)		//初始化霍夫曼树 
	{
		huff[i].weight = 0;
		huff[i].lc = 0;
		huff[i].rc = 0;
		huff[i].parent = 0;
	}
	
	while ((fread(str, sizeof(char), 1024, source)) != 0)
	{
		for (i = 0; str[i] != '\0'; i++)
		{
			huff[str[i]].weight++;		//记录每个字符的权值，直接用他们对应的ASCII码作为数组的下标 
			str[i] = '\0';
			total++;
		}		
	}
	
	for (i = 0; i < N; i++)	//记录字符种类 
		if (huff[i].weight)
			count++;
			
	for (i = N; i < N + count - 1; i++)	//霍夫曼树的完善 
	{
		select(s1, s2, huff, i);
		huff[i].lc = s1;
		huff[i].rc = s2;
		huff[i].weight = huff[s1].weight + huff[s2].weight;
		huff[s1].parent = i;
		huff[s2].parent = i;
	}
	
	Code route;
	encode(route, huff);	//编码 
	rewind(source);			//将指针再次指向文件头，方便开始压缩 
	FILE* dest = fopen(destname, "wb");
	if (dest == NULL)
	{
		printf("dest 	fail to create dest file");
		fclose(source);
		return;
	}

	fwrite(&total, sizeof(int), 1, dest);	//先写入字符总数 
	fwrite(huff,sizeof(HF),MAX,dest);		//写入霍夫曼树 
	
	UCHAR out=0;				//初始化为 0000 0000的二进制数 
	int counter=0, k, j, left;		//counter：记录一个out（8位）已被用了多少位 
	char temp[10];				//当某个编码加上counter的长度超过8时，记录超过8的部分 
	
	while ((fread(str, 1, 1024, source)) != 0)
	{
		count = strlen(str);		
		for (i = 0; i<count; i++)
		{
//			puts(route[str[i]].ch);
			k = strlen(route[str[i]].ch);
			if (k + counter <= 8)
			{
				out <<= k;		//先左移，腾出0，再做与运算 
				out |= chars_to_bits(route[str[i]].ch);				
				counter += k;
			}
			else
			{
				left = 8 - counter;		//剩余多少位可用 
				out <<= left;
				for ( j = 0; j < left; j++)
					temp[j] = route[str[i]].ch[j];
				temp[j] = '\0';
				out |= chars_to_bits(temp);
				fwrite(&out, sizeof(UCHAR), 1, dest);	
				counter = k - left;
				out = 0;
				out |= chars_to_bits(&route[str[i]].ch[left]);
			}
			if (counter == 8)
			{
				fwrite(&out, sizeof(UCHAR), 1, dest);;
				out = 0;
				counter = 0;
			}			
		}
	}
	
	if (counter)	//读完所有字符，out里有残余数据没写入压缩文件 
	{
		k = 8 - counter;		//将数据顶到最高位 
		out <<= k;
	}		
	fwrite(&out, sizeof(UCHAR), 1, dest);
	printf("compress completed\n");
	fclose(source);
	fclose(dest);
}//compress

void decompress(const char* filename, const char* destname)
{
	FILE* source = fopen(filename, "rb");
	if (source == NULL)
	{
		printf("fail to open source file");
		return;
	}
	
	int total;
	fread(&total, sizeof(int), 1, source);	//相应的，先读取字符总数和霍夫曼树 
	HFNode huff;
	fread(huff, sizeof(HF), MAX, source);
	
	int root;	//找到霍夫曼树的根结点 
	for (root = MAX-1; root >= 0; root--)
	{
		if (huff[root].weight != 0)
			break;
	}
	FILE* dest = fopen(destname, "wb");
	if (dest == NULL)
	{
		printf("fail to create dest file");
		fclose(source);
		return;
	}
	
	printf("1：使用windows自带记事本打开 0：其他\n");
	int WINDOWS_NOTEPAD;	//此功能是因为windows自带记事本的换行不是'\n' 
	do {
		scanf("%d", &WINDOWS_NOTEPAD);
		if (WINDOWS_NOTEPAD != 1 && WINDOWS_NOTEPAD != 0)
			printf("input again(1 or 0):");
	} while (WINDOWS_NOTEPAD != 1 && WINDOWS_NOTEPAD != 0);
	
	int i,j,temp=root;
	UCHAR in;
	char chars[9];
	char out;
	while ((fread(&in, sizeof(UCHAR), 1, source)) != 0)
	{
		for (j = 7; j >= 0; j--)	//实现从bit到byte的转换 
		{
			if (in%2)
				chars[j] = '1';
			else chars[j] = '0';
			in >>= 1;
		}
		chars[8] = '\0';
//		puts(chars);
		for (i = 0; chars[i] != '\0'; i++)	
		{
			if (chars[i] == '1')
				temp = huff[temp].lc;
			else temp = huff[temp].rc;
			if (temp < N)
			{
				total--;
				out = temp;
				if (out == 10 && WINDOWS_NOTEPAD)	//windows自带记事本换行 
					fwrite("\r\n", sizeof(char), 2, dest);
				else		fwrite(&out, sizeof(char), 1, dest);
				temp = root;
			}
			if (!total)
			{
				printf("decompress completed\n");
				fclose(source);
				fclose(dest);
				return;
			}
		}
	}	
}//decompress

void select(int& s1, int& s2, HFNode HFN, int k)
{
	int i;
	for (i = 0; i<k; i++)
		if (!HFN[i].parent && HFN[i].weight)
		{
			s1 = i;
			break;
		}
	for (i = s1 + 1; i<k; i++)	//找权不为0且无双亲的权最小结点下标 
		if (HFN[i].parent == 0 && HFN[i].weight <= HFN[s1].weight && HFN[i].weight)
			s1 = i;
	for (i = 0; i<k; i++)
		if (i != s1 && HFN[i].parent == 0 && HFN[i].weight)	
		{
			s2 = i;
			break;
		}
	// 找权不为0且无双亲的权最小且不同于s1结点的结点下标	
	for (i = s2 + 1; i<k; i++)
		if (HFN[i].parent == 0 && HFN[i].weight <= HFN[s2].weight && i != s1 && HFN[i].weight)
			s2 = i;
}//select

void encode(Code& route, HFNode HFN)
{
	int parent, j, start,count;
	char ch[10];
	ch[9] = '\0';
	for (int i = 0; i<256; i++)
	{
		start = 8;
		j = i;
		parent = HFN[i].parent;
		count = 1;
		while (parent)
		{
			count++;
			if (HFN[parent].lc == j)	//若是左孩子，则编码‘1’否则编码‘0’ 
				ch[start--] = '1';
			else	ch[start--] = '0';
			j = parent;
			parent = HFN[parent].parent;
		}
		route[i].ch = (char*)malloc(sizeof(char)*count);
		if (route[i].ch == NULL)
			printf("malloc error for route");
		else	strcpy(route[i].ch, &ch[++start]);
	}
}//encode

UCHAR chars_to_bits(const char* ch)
{
	int i;
	UCHAR bits = (UCHAR)0;
	if(ch[0] == '0')
		bits |= (UCHAR)0;
	else	bits |= (UCHAR)1;
	int len = strlen(ch);
	for (i = 1; i<len; ++i)		//将8个字符转换成8个二进制位  
	{
		bits <<= 1;
		if (ch[i] == '0')
			bits |= (UCHAR)0;
		else	bits |= (UCHAR)1;
	}
	return bits;
}//chars_to_bits
