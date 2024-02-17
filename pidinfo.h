#include <vector>
#include <cstdlib>
#include <map>
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
//通过map动态维护每个pid对应的进程树节点地址，方便完成插入操作
std::map<int, PTree*> ind;
//借助FILE库函数打开并读取进程状态文件
static void get_info(PTree *pNode,long pid){
	char cmd[30] = {0};
	char pstat[24] = {0};
	long ppid = 0;
	//生成文件路径
	sprintf(pstat, "/proc/%ld/stat", pid);
	//打开文件流并读入到缓冲区
	FILE *fstat = fopen(pstat, "r");
	if(!fstat) { return;}
	//从缓冲区读入进程信息
	fscanf(fstat, "%s (%s %c %ld", pstat, cmd, pstat, &ppid);
	pNode->pid = pid;
	pNode->ppid = ppid;
	//标准化处理
	int len = strlen(cmd);
	pNode->cmd = (char*)malloc(len);
	strcpy(pNode->cmd, cmd);
	pNode->cmd[len - 1] = 0;
	//关闭释放文件流
	fclose(fstat);
}
//进程树初始化
static PTree* init_ptree(){
	std::sort(pids.begin(), pids.end());
	//const int size = pids[pids.size()-1];
	//static PTree index[size];
	PTree *pNode = new PTree();
	//初始化0号进程
	pNode->pid = 0;
	ind[0] = pNode;
	return pNode;
}
//通过vector实现动态插入子节点
static void insert_ptree(long pid) {
	PTree *pNode = new PTree();
	//从文件读取进程信息
	get_info(pNode, pid);
	//新建pid与节点地址的映射关系
	ind[pid] = pNode;
	//assert(ind[pNode.ppid] != NULL);
	//找到父节点地址并插入
	ind[pNode->ppid] -> spid.push_back(pNode);
}
//建树主程序		
static PTree* build_pstree(){
	PTree *pRoot = init_ptree();
	//遍历并插入节点
	for(auto g:pids){
		assert(g);
		insert_ptree(g);
	}
	return pRoot;
}
