#ifndef _PID_H_
#define _PID_H_
#include <vector>
#include <map>
#include <cstdlib>
#include <dirent.h>
#include <algorithm>
#include "getpid.h"

//进程树结构体
struct PTree{
	long pid;
	long ppid;
	std::vector<PTree*> spid;
	char *cmd;
};

//借助FILE库函数打开并读取进程状态文件
void get_info(PTree *pNode,long pid);
//进程树初始化
PTree* init_ptree();
//通过vector实现动态插入子节点
void insert_ptree(long pid) ;
//建树主程序		
PTree* build_pstree();
#endif