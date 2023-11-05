#include<stdio.h>
#include<stdlib.h>

//字母表
extern char symbol_list[30];
extern int listno;

//NFA状态节点
typedef struct node
{
    int id;//状态id
    
    struct node* next[10];//下一状态
    
    //边和下一状态利用index实现一一对应
    char edge[10];
    
    int isFinal;//是否为终态
    
    int count;//下个状态的总数
    
    //对于一个正则表达式转换成的NFA节点只会有一个终结态，因此设置一个end终态节点。
    struct node* end;

}NFAnode;

//DFA状态节点
typedef struct DFA
{
    int id;//状态id
    
    NFAnode* NFAset[10];//状态集中包含的NFA状态。
    
    int isFinal;//标记终态

    struct DFA* next[10];//下一状态
    
    //边和下一状态利用index实现一一对应
    char edge[10];

    int next_count;//下个状态的总数

    int NFAcount;//包含的NFA状态的总数

}DFAnode;

typedef struct DFAset
{
    int id;//状态id

    int isFinal;//标记终态

    int isStart;//标记初态

    //包含的DFAnode
    DFAnode * dfa_set[10];

    int dfa_count;

    struct DFAset* next[10];//下一状态
    
    //边和下一状态利用index实现一一对应
    char edge[10];

    int next_count;//下个状态的总数

}minDFAnode;



//将一个minDFAnode从状态集合中删除
void deleteminDFAnode(minDFAnode* node);

//将一个minDFAnode添加到状态集合中
void addminDFAnode(minDFAnode* node);


//利用子集构造法，将NFA转换成DFA
DFAnode* toDFA(NFAnode* start);

//转换为最小化DFA
void minDFA();

//创建一个DFA节点
DFAnode* creatDFAnode();

//将一个NFAnode加入到一个DFA节点中
void addNFAnode2DFAnode(NFAnode* nfa,DFAnode* dfa);

//将一个NFAnode的闭包转换为一个DFA节点
void colureOfNFAnode(NFAnode* nfa,DFAnode* dfa);

//连接两个DFAnode
void addNextDFA(DFAnode* pre_node,DFAnode* next_node,char symbol);

//比较两个DFA节点是否相同
int compareDFA(DFAnode* pre_node,DFAnode* next_node);

//比较两个minDFAnode
int copareMinDFA(minDFAnode *pre,minDFAnode *next);

//创建一个minDFAnode
minDFAnode* createMinDFAnode();

//打印DFA
void printDFA(DFAnode* start);

//找到DFAnode所属的minDFAnode
minDFAnode *search(DFAnode *node);

//连接两个minDFAnode
void addNextminDFA(minDFAnode* pre_node,minDFAnode* next_node,char symbol);

//将DFAnode加入到minDFAnode中
void addDFA2MIN(DFAnode *dfa,minDFAnode* min_dfa);

//打印MinDFA
void printMinDFA();