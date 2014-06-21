
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ZuPuTree.h"

#define MAXOFCHILD 10 //族谱树孩子的最大值


enum boolean {False,True};//声明枚举类型

typedef struct Date {
	
	int year;//年

	int month;//月

	int date;//日

} Date;


/*
 *族谱的结点的信息
 **/
typedef struct member {

	int id;//主键：用于唯一标示某个成员

	char name[20];//姓名
	
	char address[50];//住址

	enum boolean isExist;//是否健在，如果是True表示还在，否则是不在了

	Date birthday;//出生日期

	Date dieDay;//死亡日期

	enum boolean isManage;//婚否，如果是True表示已经结婚了，Falise没有

	char zuPuName;//所属的族谱;

	int parent;//父母的编号值
}Member;

Member member[50];
int length = 0;

/*
 *族谱树的存储结构
 *使用了孩子表示法
 */
typedef struct ZuPuTree{
	
	Member data;//当前结点的数据
	
	struct ZuPuTree *children;

	int numberOfChild;

}ZuPuTree;



typedef struct ZuPu {
	
	int id;//族谱的id

	char name[20];//族谱的名字

	ZuPuTree tree;//所有成员构成的树

	

}ZuPu;


/*
 *创建一个族谱文件，
 */
void createZuPu(ZuPu *zuPu) 
{
	int id;
	char name[20];
	FILE *fp;
	Member m;
	printf("输入族谱的编号:");
	scanf("%d",&id);
	printf("\n");

	printf("输入族谱的名称");
	scanf("%s",name);
	printf("\n");
	
	zuPu->id = id ;
	strcpy(zuPu->name,name);
	
	strcat(name,".txt");
	
	printf("族谱的名称是%s\n",zuPu->name);

	fp = fopen(name,"wb");//创建一个dat文件
	
	
	m.id = 0;
	strcpy(m.name,"root");

	zuPu->tree.data = m;//创建一个root结点
	
	fclose(fp);
}

/*输入族谱成员的信息,将信息存入临时数组member*/
void scanfZuPuMember(int *flag) {
	scanf("%d%d%s",&member[length].id,&member[length].parent,&member[length].name);
	*flag = member[length].id;
	if(*flag == -1)
	{
		return;
	}
	length++;
}

/*
 *打印临时数组里面的下标为i的member的信息
 */
void printfZuPuMember(int i) {
	printf("%-4d%-4d%10s",member[i].id,member[i].parent,member[i].name);
}


/*
 *@param f 目标文件的指针
 */
void writeInfoToFile(FILE *f) {
	
	int j;
	for( j = 0;j < length;j++)
	{
		if(fwrite(&member[j],sizeof(Member),1,f) != 1) {
			printf("file write error");
			exit(0);
		}
	}
}



/*
 *创建族谱关系树
 *@param tree 
 *@param parentId 
 */
void createTree(ZuPuTree *tree,int parentId) {
	int i;
	int j;
	tree->children = (ZuPuTree *)malloc(sizeof(ZuPuTree)*MAXOFCHILD); //创建存储孩子的空间，只不过这里有对空间有点浪费
	/*遍历所有，然后找到*/
	tree->numberOfChild = 0;
	for(i = 0 ,j = 0;i < length; i++) {
		if(member[i].parent == parentId)
		{	
			tree->children[j].data = member[i]; 
			tree->numberOfChild++;
			createTree(&(tree->children[j]),tree->children[j].data.id);
			j++;
		}
	}
}

/*
 *族谱树的遍历
 */
void travalZuPuTree(ZuPuTree *z) {
	/*输出当前结点的信息*/
	int i = 0;
	printf("%-4d%10s%4d\n",z->data.id,z->data.name,z->data.parent);
	for(;i < z->numberOfChild; i++) {
		travalZuPuTree(&(z->children[i]));
	}
}



/*
 *族谱的展示
 */
void displayZuPu(ZuPu *zuPu) { 

	ZuPuTree z = zuPu->tree;
	printf("族谱名：%s\n",zuPu->name);
	printf("族谱编号：%d\n",zuPu->id);
	travalZuPuTree(&z);
	printf("\n");
}



/*
 *创建家族关系
 *先把数据存入文件里面，然后再把通过数据在内存建立族谱树
 */
void createZuPuTree(ZuPu *zuPu) {
	
	int i =0;
	FILE *f;
	char fileName[20];

	/*输入成员的信息
	 *这里有个要求：最后的一条数据的第一个字段是-1(作为结束标志)
	 */
	int flag=1;
	while(flag != -1) 
	{	
		
		scanfZuPuMember(&flag);
		printf("the flag is %d\n",flag);
	}

	/*将成员的信息写入文件里面*/
	strcpy(fileName,zuPu->name);
	strcat(fileName,".txt");
	f = fopen(fileName,"wb");
	writeInfoToFile(f);
	
	createTree(&zuPu->tree,0);//在内存里面构造关系树
}


int main()
{
	ZuPu zuPu; 

	while(1) 
	{
		/*创建一个空的族谱，其实是创建了一个文件*/
		createZuPu(&zuPu);
		createZuPuTree(&zuPu);
		displayZuPu(&zuPu);
	}
}