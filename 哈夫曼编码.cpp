#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 256    //8λ��������������256���ַ� 
#define MAX 511	 //���������ĳ��ȣ� 2*N-1 

typedef unsigned char UCHAR;
typedef struct huffmanNode	//�������� 
{
	int weight;
	short lc, rc, parent;
}HF, HFNode[MAX];

typedef struct		//����� 
{
	char* ch;
}code, Code[N];

void compress(const char* filename, const char* destname);	//ѹ������ 
void decompress(const char* filename, const char* destname);	//��ѹ������ 
void select(int& s1, int& s2, HFNode HFN, int k);	//ѡ������Ȩ��С����Ϊ0����Ԫ���±� 
void encode(Code& route, HFNode HFN);	//���뺯�� 
UCHAR chars_to_bits(const char* ch);	//ʵ�ִ�byte��bit��ת��  

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
	int i, s1, s2, count = 0,total=0;	//count����¼�ж������ַ���total����¼�ַ����� 
	char str[1026] = {'\0'};
	for (i = 0; i<MAX; i++)		//��ʼ���������� 
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
			huff[str[i]].weight++;		//��¼ÿ���ַ���Ȩֵ��ֱ�������Ƕ�Ӧ��ASCII����Ϊ������±� 
			str[i] = '\0';
			total++;
		}		
	}
	
	for (i = 0; i < N; i++)	//��¼�ַ����� 
		if (huff[i].weight)
			count++;
			
	for (i = N; i < N + count - 1; i++)	//�������������� 
	{
		select(s1, s2, huff, i);
		huff[i].lc = s1;
		huff[i].rc = s2;
		huff[i].weight = huff[s1].weight + huff[s2].weight;
		huff[s1].parent = i;
		huff[s2].parent = i;
	}
	
	Code route;
	encode(route, huff);	//���� 
	rewind(source);			//��ָ���ٴ�ָ���ļ�ͷ�����㿪ʼѹ�� 
	FILE* dest = fopen(destname, "wb");
	if (dest == NULL)
	{
		printf("dest 	fail to create dest file");
		fclose(source);
		return;
	}

	fwrite(&total, sizeof(int), 1, dest);	//��д���ַ����� 
	fwrite(huff,sizeof(HF),MAX,dest);		//д��������� 
	
	UCHAR out=0;				//��ʼ��Ϊ 0000 0000�Ķ������� 
	int counter=0, k, j, left;		//counter����¼һ��out��8λ���ѱ����˶���λ 
	char temp[10];				//��ĳ���������counter�ĳ��ȳ���8ʱ����¼����8�Ĳ��� 
	
	while ((fread(str, 1, 1024, source)) != 0)
	{
		count = strlen(str);		
		for (i = 0; i<count; i++)
		{
//			puts(route[str[i]].ch);
			k = strlen(route[str[i]].ch);
			if (k + counter <= 8)
			{
				out <<= k;		//�����ƣ��ڳ�0������������ 
				out |= chars_to_bits(route[str[i]].ch);				
				counter += k;
			}
			else
			{
				left = 8 - counter;		//ʣ�����λ���� 
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
	
	if (counter)	//���������ַ���out���в�������ûд��ѹ���ļ� 
	{
		k = 8 - counter;		//�����ݶ������λ 
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
	fread(&total, sizeof(int), 1, source);	//��Ӧ�ģ��ȶ�ȡ�ַ������ͻ������� 
	HFNode huff;
	fread(huff, sizeof(HF), MAX, source);
	
	int root;	//�ҵ����������ĸ���� 
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
	
	printf("1��ʹ��windows�Դ����±��� 0������\n");
	int WINDOWS_NOTEPAD;	//�˹�������Ϊwindows�Դ����±��Ļ��в���'\n' 
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
		for (j = 7; j >= 0; j--)	//ʵ�ִ�bit��byte��ת�� 
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
				if (out == 10 && WINDOWS_NOTEPAD)	//windows�Դ����±����� 
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
	for (i = s1 + 1; i<k; i++)	//��Ȩ��Ϊ0����˫�׵�Ȩ��С����±� 
		if (HFN[i].parent == 0 && HFN[i].weight <= HFN[s1].weight && HFN[i].weight)
			s1 = i;
	for (i = 0; i<k; i++)
		if (i != s1 && HFN[i].parent == 0 && HFN[i].weight)	
		{
			s2 = i;
			break;
		}
	// ��Ȩ��Ϊ0����˫�׵�Ȩ��С�Ҳ�ͬ��s1���Ľ���±�	
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
			if (HFN[parent].lc == j)	//�������ӣ�����롮1��������롮0�� 
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
	for (i = 1; i<len; ++i)		//��8���ַ�ת����8��������λ  
	{
		bits <<= 1;
		if (ch[i] == '0')
			bits |= (UCHAR)0;
		else	bits |= (UCHAR)1;
	}
	return bits;
}//chars_to_bits
