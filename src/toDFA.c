#include<stdio.h>
#include<stdlib.h>
#include "toDFA.h"

int DFA_state_num=100;

int min_DFA_state_num=1000;

//字母表
char symbol_list[30];
int listno=0;

//状态集合
minDFAnode * minset[30];
int min_count=0;

//标记状态集合是否被修改
int dirty=0;

DFAnode* DFA_start;

DFAnode* toDFA(NFAnode *nfa_start){
    
    DFAnode *dfa_start=creatDFAnode();
    DFAnode *node_stack[30];
    
    colureOfNFAnode(nfa_start,dfa_start);
    
    node_stack[0]=dfa_start;
    
    int index=0;
    int sum=1;
    
    DFAnode* curr;
    while(index<sum){
        curr=node_stack[index];
        //保存可以转换状态的输入
        char list[20];
        int list_num=0;
        //遍历DFAnode中NFAnode
        for(int i=0;i<curr->NFAcount;i++){
            NFAnode* curr_nfa=curr->NFAset[i];
            //遍历NFAnode中的edge
            for (int j = 0; j <curr_nfa->count; j++)
            {
                if(curr_nfa->edge[j]==0){
                    continue;
                }
                int flag=1;
                //list中含有重复元素
                for(int k=0;k<list_num;k++){
                    if(list[k]==curr_nfa->edge[j]){
                        flag=0;
                        break;
                    }
                }
                if(flag){
                    list[list_num++]=curr_nfa->edge[j];
                }
            }
        }
        //让NFAnode遍历每个符号
        for(int i=0;i<list_num;i++){
            DFAnode* next=creatDFAnode();
            //遍历每个nfa
            for(int j=0;j<curr->NFAcount;j++){
                NFAnode* curr_nfa=curr->NFAset[j];
                //遍历nfa中的每个后继态
                for(int k=0;k<curr_nfa->count;k++){
                    if(curr_nfa->edge[k]==list[i]){
                        colureOfNFAnode(curr_nfa->next[k],next);
                    }
                }
            }
            //判断栈中是否已经存在该状态
            int flag=1;
            for(int m=0;m<sum;m++){
                if(compareDFA(next,node_stack[m])){
                    flag=0;
                    next=node_stack[m];
                    DFA_state_num--;
                    break;
                }
            }
            if(flag){
                node_stack[sum++]=next;
            }
            addNextDFA(curr,next,list[i]);
        }
        index++;
    }
    DFA_start=dfa_start;
    
    printDFA(dfa_start);
}

DFAnode* creatDFAnode(){
    DFAnode *node=malloc(sizeof(DFAnode));
    
    //初始化DFAnode成员变量
    node->id=DFA_state_num++;
    node->next_count=0;
    node->NFAcount=0;
    node->isFinal=0;
    
    return node;

}

void addNFAnode2DFAnode(NFAnode* nfa,DFAnode* dfa){
    
    //检查DFAnode中是否已存在NFAnode
    for(int j=0;j<dfa->NFAcount;j++){
        if(nfa==dfa->NFAset[j]){
            return;
        }
    }

    dfa->NFAset[dfa->NFAcount++]=nfa;
    
    //如果NFAndoe为终态，则DFAnode也为终态
    if(nfa->isFinal){
        dfa->isFinal=1;
    }

}

void colureOfNFAnode(NFAnode* nfa,DFAnode* dfa){
    addNFAnode2DFAnode(nfa,dfa);
    
    //建立一个nfanode栈
    NFAnode* stack[50];
    stack[0]=nfa;
    int index=0;
    int sum=1;
    
    NFAnode* curr;
    
    while(index<sum){
        curr=stack[index];

        //遍历nfa的后继状态
        for(int i=0;i<curr->count;i++){
            if(curr->edge[i]==0){
                //将这个空边跳转的状态加入到DFA节点中
                addNFAnode2DFAnode(curr->next[i],dfa);
                //避免重复nfa节点入栈
                int flag=1;
                for(int j=0;j<sum;j++){
                    if(stack[j]==curr->next[i]){
                        flag=0;
                        break;
                    }
                }
                if(flag){
                    stack[sum++]=curr->next[i];
                }
            }
        }
        index++;
    }
}

void addNextDFA(DFAnode* pre_node,DFAnode* next_node,char symbol){
    pre_node->next[pre_node->next_count]=next_node;
    pre_node->edge[pre_node->next_count]=symbol;
    pre_node->next_count++;
}

//判断两个DFA节点是否相同
int compareDFA(DFAnode* pre_node,DFAnode* next_node){

    if(pre_node->NFAcount!=next_node->NFAcount){
        return 0;
    }
    else{
        for(int i=0;i<pre_node->NFAcount;i++){
            int found=0;
            for(int j=0;j<next_node->NFAcount;j++){
                if(pre_node->NFAset[i]==next_node->NFAset[j]){
                    found=1;break;
                }
            }
            if(!found){
                return 0;
            }
        }
        return 1;
    }

}

DFAnode* DFA_list[100];

int dfa_list_count=0;
//通过BFS遍历打印DFA
void printDFA(DFAnode* start){

    DFA_list[0]=start;
    int index=0;
    dfa_list_count++;
    while(index<dfa_list_count){
        if(DFA_list[index]->isFinal==1){
            printf("final ");
        }
        printf("state:%d\nset:",DFA_list[index]->id);

        for(int i=0;i<DFA_list[index]->NFAcount;i++){
            printf("%d\t",DFA_list[index]->NFAset[i]->id);
        }

        printf("\n");
        
        for(int i=0;i<DFA_list[index]->next_count;i++){
            printf("next state:%d\tedge:%d\t ",DFA_list[index]->next[i]->id,DFA_list[index]->edge[i]);
            int flag=1;
            for(int j=0;j<dfa_list_count;j++){
                if(DFA_list[j]==DFA_list[index]->next[i]){
                    flag=0;
                    break;
                }
            }
            if(flag){
                DFA_list[dfa_list_count++]=DFA_list[index]->next[i];
            }
        }

        printf("\n");
        index++;
    }
}


int copareMinDFA(minDFAnode *pre,minDFAnode *next){
    if(pre->dfa_count!=next->dfa_count){
        return 0;
    }
    else{
        for(int i=0;i<pre->dfa_count;i++){
            int found=0;
            for(int j=0;j<next->dfa_count;j++){
                if(pre->dfa_set[i]==next->dfa_set[j]){
                    found=1;break;
                }
            }
            if(!found){
                return 0;
            }
        }
        return 1;
    }
};

void addminDFAnode(minDFAnode* node){
    
    //如果状态集合中已存在，则直接返回
    for(int i=0;i<min_count;i++){
        if(copareMinDFA(node,minset[i])){
            return;
        }
    }
    
    dirty=1;
    minset[min_count++]=node;
}

void deleteminDFAnode(minDFAnode* node){
    
    for(int i=0;i<min_count;i++){
        if(minset[i]==node){
            minset[i]=minset[i+1];
            minset[i+1]=node;            
        }
    }
    
    min_count--;
    dirty=1;
}


minDFAnode* createMinDFAnode(){
    minDFAnode* node=malloc(sizeof(minDFAnode));
    node->isStart=0;
    node->id=min_DFA_state_num++;
    node->dfa_count=0;
    node->next_count=0;
    node->isFinal=0;

    return node;
}

void minDFA(){
    
    minDFAnode *min_start=createMinDFAnode();
    minDFAnode *min_end=createMinDFAnode();
    int hasStart=0;
    //遍历DFAnode
    for(int i=0;i<dfa_list_count;i++){
        if(DFA_list[i]->isFinal){
            addDFA2MIN(DFA_list[i],min_end);
        }
        else{
            hasStart=1;
            addDFA2MIN(DFA_list[i],min_start);
        }
    }

    if(hasStart){
        addminDFAnode(min_start);
    }
    addminDFAnode(min_end);
    //不断更新minset
    while(dirty){
        dirty=0;
        //遍历状态集合中的每个minDFAnode
        for(int i=0;i<min_count;i++){
            minDFAnode* curr=minset[i];
            //遍历字母表
            for(int j=0;j<listno;j++){
                //记录一个集合会被划分得到多少个
                int sub_count=0;
                minDFAnode* next_min[10];
                minDFAnode* new_min[10];
                //遍历每元素
                for(int m=0;m<curr->dfa_count;m++){
                    DFAnode* curr_dfa=curr->dfa_set[m];
                    //记录是否存在该符号边
                    int hasNext=0;

                    //遍历DFAnode的下一状态
                    for(int v=0;v<curr_dfa->next_count;v++){
                        if(curr_dfa->edge[v]==symbol_list[j]){
                            hasNext=1;
                            //找到下一状态所属的minDFAnode
                            minDFAnode* next_minDFAnode=search(curr_dfa->next[v]);
                            int exist=0;
                            int index=0;
                            //查询next_min中是否存在
                            for(int x=0;x<sub_count;x++){
                                if(next_minDFAnode==next_min[x]){
                                    index=x;
                                    exist=1;
                                }
                            }
                             //若next_min不存在，则添加一个
                            if(!exist){
                                next_min[sub_count]=next_minDFAnode;
                                new_min[sub_count]=createMinDFAnode();
                                addDFA2MIN(curr_dfa,new_min[sub_count]);
                                sub_count++;
                            }
                            else{
                                addDFA2MIN(curr_dfa,new_min[index]);
                            }
                            break;
                        }
                    }
                    //若无该状态转换边，将下一状态置为NULL
                    if(!hasNext){
                        int index=0;
                        int found=0;
                        for(int y=0;y<sub_count;y++){
                            if(next_min[y]==NULL){
                                found=1;
                                index=y;
                                break;
                            }
                        }
                        if(!found){
                            next_min[sub_count]=NULL;
                            new_min[sub_count]=createMinDFAnode();
                            addDFA2MIN(curr_dfa,new_min[sub_count]);
                            sub_count++;
                        }
                        else{
                            addDFA2MIN(curr_dfa,new_min[index]);
                        }
                    }
                }
                if(sub_count>1){
                    deleteminDFAnode(curr);
                    for(int z=0;z<sub_count;z++){
                        addminDFAnode(new_min[z]);
                    }
                    break;
                }
            }
        }
            
    }
}

void addDFA2MIN(DFAnode *dfa,minDFAnode* min_dfa){

    //避免重复
    for(int i=0;i<min_dfa->dfa_count;i++){
        if(dfa==min_dfa->dfa_set[i]){
            return;
        }
    }
    
    min_dfa->dfa_set[min_dfa->dfa_count++]=dfa;

    if(dfa->isFinal){
        min_dfa->isFinal=1;
    }
    if(dfa==DFA_start){
        min_dfa->isStart=1;
    }
}

minDFAnode *search(DFAnode *node){
    //遍历状态集合
    for(int i=0;i<min_count;i++){
        //遍历状态集合中的元素的每个DFAnode成员
        minDFAnode* curr=minset[i];
        for(int j=0;j<curr->dfa_count;j++){
            if(node==curr->dfa_set[j]){
                return curr;
            }
        }
    }
    printf("error:DFAnode is lost!!!");
    return NULL;
}

void addNextminDFA(minDFAnode* pre_node,minDFAnode* next_node,char symbol){
    pre_node->next[pre_node->next_count]=next_node;
    pre_node->edge[pre_node->next_count]=symbol;
    pre_node->next_count++;
};



void printMinDFA(){

    printf("MIN DFA:\n");

    for(int i=0;i<min_count;i++){
        minDFAnode* curr=minset[i];

        DFAnode* oneOfcurr=curr->dfa_set[0];

        for(int j=0;j<oneOfcurr->next_count;j++){
            minDFAnode* next_min_dfa=search(oneOfcurr->next[j]);
            addNextminDFA(curr,next_min_dfa,oneOfcurr->edge[j]);
        }

        if(curr->isFinal){
            if(curr->isStart){
                printf("strat\t");
            }
            printf("final state:%d",curr->id);
        }
        else if(curr->isStart){
            printf("start state:%d",curr->id);
        }
        else{
            printf("state:%d",curr->id);   
        }
        printf("\nset:");
        for(int k=0;k<curr->dfa_count;k++){
            printf("%d\t",curr->dfa_set[k]->id);
        }
        printf("\n");
        for(int j=0;j<curr->next_count;j++){
            printf("next_state:%d\t edge:%d\t",curr->next[j]->id,curr->edge[j]);
        }
        printf("\n");
    }
}