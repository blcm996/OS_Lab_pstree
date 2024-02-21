#include <stdio.h>
#include <assert.h>
#include "pidinfo.h"
#include "treePrint.h"
#include <string.h>

bool cmp(PTree *a, PTree *b){
	return a->cmd[0] < b->cmd[0];
}
//几种不同的打印树的逻辑，具体由命令行参数决定
void print_tree(PTree *root,char *pre, int idx){
        if(root == NULL) return;
        //打印行前缀
        if(idx != 0)
                printf("%s", pre);
        //打印行末的进程信息并回车
        if(root->spid.size() == 0)
        {printf("-%s\n", root->cmd);return;}
        //打印该节点信息并遍历打印子节点
        if(root->spid.size() > 0){
                printf("-%s--", root->cmd);
                //更新行前缀
                char *new_pre;
                int len = strlen(pre);
                int lens = len + strlen(root->cmd) + 3;
                new_pre = (char*)malloc(lens);
                strcpy(new_pre, pre);
                for(int i = len; i < lens-1; i++)
                        new_pre[i] = ' ';
                if(root->spid.size() == 1)
                        new_pre[lens-1] = ' ';
                else
                        new_pre[lens-1] = '|';
                //遍历打印子节点，下标i用来使第一个子节点向右打印
                int i = 0;
		std::sort(root->spid.begin(), root->spid.end(), cmp);
                for(auto g:root->spid)
                print_tree(g, new_pre, i), i++;
                return;
        }
        return;
}
//一下几种打印逻辑与第一个类似，不再赘述
void print_tree_n(PTree *root,char *pre, int idx){
	if(root == NULL) return;
	
	if(idx != 0)
		printf("%s", pre); 

	if(root->spid.size() == 0)
	{printf("-%s\n", root->cmd);return;}
	
	if(root->spid.size() > 0){
		printf("-%s--", root->cmd);
		char *new_pre;
		int len = strlen(pre);
		int lens = len + strlen(root->cmd) + 3;
		new_pre = (char*)malloc(lens);
		strcpy(new_pre, pre);
		for(int i = len; i < lens-1; i++)
			new_pre[i] = ' ';
		if(root->spid.size() == 1)
			new_pre[lens-1] = ' ';
		else
			new_pre[lens-1] = '|';
		int i = 0;
		for(auto g:root->spid)
		print_tree_n(g, new_pre, i), i++;
		return;
	}
	return;
}

void print_tree_pid(PTree *root,char *pre, int idx){
        if(root == NULL) return;

        if(idx != 0)
                printf("%s", pre);

        if(root->spid.size() == 0)
        {printf("-%s(%ld)\n", root->cmd, root->pid);return;}

        if(root->spid.size() > 0){
                printf("-%s(%ld)--", root->cmd, root->pid);
                char *new_pre;
                int len = strlen(pre);
                int lend = 0, pid_n = root->pid;
                do
                   {lend++;}
                while(pid_n /= 10);
                int lens = len + strlen(root->cmd) + lend + 5;
                new_pre = (char*)malloc(lens);
                strcpy(new_pre, pre);
                for(int i = len; i < lens-1; i++)
                        new_pre[i] = ' ';
                if(root->spid.size() == 1)
                        new_pre[lens-1] = ' ';
                else
                        new_pre[lens-1] = '|';
                int i = 0;
		std::sort(root->spid.begin(), root->spid.end(), cmp);
                for(auto g:root->spid)
                print_tree_pid(g, new_pre, i), i++;
		return;
        }
        return;
}

void print_tree_pid_n(PTree *root,char *pre, int idx){
        if(root == NULL) return;

        if(idx != 0)
                printf("%s", pre);

        if(root->spid.size() == 0)
        {printf("-%s(%ld)\n", root->cmd, root->pid);return;}

        if(root->spid.size() > 0){
                printf("-%s(%ld)--", root->cmd, root->pid);
                char *new_pre;
                int len = strlen(pre);
		int lend = 0, pid_n = root->pid;
		do 
		   {lend++;}
		while(pid_n /= 10);
                int lens = len + strlen(root->cmd) + lend + 5;
                new_pre = (char*)malloc(lens);
                strcpy(new_pre, pre);
                for(int i = len; i < lens-1; i++)
                        new_pre[i] = ' ';
                if(root->spid.size() == 1)
                        new_pre[lens-1] = ' ';
                else
                        new_pre[lens-1] = '|';
                int i = 0;
                for(auto g:root->spid)
                print_tree_pid_n(g, new_pre, i), i++;
                return;
        }
        return;
}
