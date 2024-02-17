#include <stdio.h>
#include <assert.h>
#include "prase.h"
#include "pidinfo.h"

static bool cmp(PTree *a, PTree *b){
	return a->cmd[0] < b->cmd[0];
}
//几种不同的打印树的逻辑，具体由命令行参数决定
static void print_tree(PTree *root,char *pre, int idx){
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
static void print_tree_n(PTree *root,char *pre, int idx){
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

static void print_tree_pid(PTree *root,char *pre, int idx){
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

static void print_tree_pid_n(PTree *root,char *pre, int idx){
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

int main(int argc, char *argv[]) {
        //解析命令行指令参数
        prase_arguements(argc, argv);
        assert(!argv[argc]);
        //获取当前所有进程id
        get_pids("/proc");
        //建立进程树并得到根节点指针（0号进程）
        PTree *Root = build_pstree();
        //得到1号进程地址（默认以1号进程为根节点打印树）
        PTree *root_1 = Root->spid[0];
        char *pre;
        pre = " ";
        //版本信息
        char *version = "pstree (c++) 1.3\nCopyright (C) 2002-2023 BLCM\nThis is my class lab, and you are welcome to redistribute it underthe terms of the GNU General Public License.\nFor more information about these matters, please send e-mail to b886866b@163.com.";
        //根据命令行参数选择打印进程树的方式
        if(show_version)
                printf("%s\n", version);
        else if(show_pids && !numeric_sort)
                print_tree_pid(root_1, pre, 1);
        else if(!show_pids && numeric_sort)
                print_tree_n(root_1, pre, 1);
        else if(show_pids && numeric_sort)
                print_tree_pid_n(root_1, pre, 1);
        else
                print_tree(root_1, pre, 1);
        return 0;
}
