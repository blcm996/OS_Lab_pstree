#include <stdio.h>
#include <assert.h>
#include "prase.h"
#include "pidinfo.h"
#include "getpid.h"
#include "treePrint.h"

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
