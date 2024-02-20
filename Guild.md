# 打印进程树

### 适合对象：

Linux&C/C++初学者

### 实验内容：

参考Linux-bash命令行中pstree的命令参数与效果，复刻一个简版的pstree

（实验来自NJU2022操作系统课程：[M1: 打印进程树 (pstree) (jyywiki.cn)](https://jyywiki.cn/OS/2022/labs/M1.html)）

### 分析与实现:

#### 目标：

程序能够读取命令行输入的指令，并给出相应的输出

**例如：** 

```bash
>pstree 

>pstree -p

>pstree -p -n

```

显而易见，如果你知道“如何将大象放进冰箱”，那么就一定能看出来我们即将设计的程序可以分为“读取并解析指令输入”、“处理数据”和“打印数据”三个阶段。接下来我就会依次讲解这三个阶段的完成过程。

#### 阶段一：解析指令输入

##### 读取指令

如果你熟悉LinuxC编程，一定了解如何读取命令行输入的文本。最常用的莫过于"scanf()"函数。而如果我们想要程序在启动时就能够接收指令参数，就必须启用main函数的参数列表了。
如下所示
```
int main(int argc, char *argv[]) {
        //解析命令行指令参数
        prase_arguements(argc, argv);
```
这样我们可以如下的方式启动程序
```
>./pstree.cpp pstree -p
```

这里我将参数处理函数放在了头文件（prase.h）中。
那么如何处理参数呢？
##### 解析指令
当然，我们可以对读入的一行参数进行逐个的字符串分析，但那样可能有些过于繁琐。这里我查了一下库函数，找到了一个更方便的解决方案，如下所示：

```
static void prase_arguements(int argc, char *argv[]) {
	//解析cmd参数
	//校验argv合法性
	for(int i = 1; i < argc; i++) {
		assert(argv[i]);
	}	
	int opt = 0;
	while((opt=getopt_long(argc, argv, "pnV", long_option, NULL))!=-1){
		switch(opt){
			case 0: 
				break;
			case 'p': 
				show_pids = 1;break;
			case 'n': 
				numeric_sort = 1;break;
			case 'V': 
				show_version = 1;break;
			default:
				break;
		}
	}
}
```
这里的getopt_long()函数就是c++函数库<getopt.h>中专门用来处理此类命令行命令参数的库函数。
接着再初始化一些静态变量作为每个可供选择的参数的flag
```
static int show_version = 0, numeric_sort = 0, show_pids = 0;
static const struct option long_option[]={
	{"show_pids", optional_argument, &show_pids, 1},
	{"numeric_sort", optional_argument, &numeric_sort, 1},
	{"show_version", optional_argument, &show_version, 1},
	{NULL, 0, NULL, 0}
};
```
最后我们只要设计一定逻辑，使得程序读取到指令参数后能够正确的激活对应的flag变量
关于如何使用相关库函数，建议自行搜索或者通过AI学习，这里不再赘述。

#### 阶段二：处理进程信息
接下来的部分大概就是整个实验最难也是最关键的部分了
##### 获取进程信息
不知大家是否听过一句话，叫做“Linux中，一切皆文件”。大意就是指在Linux系统中，所有的一切都可以通过文件的方式访问、管理。这包括硬件设备、进程、套接字等，即使它们本身不是文件，也被抽象成文件。https://zhuanlan.zhihu.com/p/349354666
因此，我们可以直接从系统根目录下的“/proc”目录获得包含当前系统所有进程信息的文件https://www.cnblogs.com/DswCnblog/p/5780389.html。这里建议大家阅读博客的同时自己在linux终端找到该目录并打印其中的文件，观察其内容结构是否符合预期。
直接说结论，所有以数字作为文件名的目录就是我们要找到目标文件。每个数字就是系统进程的id，文件里存放的就是该进程的所有信息。我们只需找到代表其父进程和子进程的字符串（数字），就可以在逻辑上构建出一棵进程树。当然，别忘了一并读取必要的进程信息。
思路有了，接下来就是实践了
相信一定有人发现，“/proc”目录下不仅有以数字作为文件名的文件，还有很多其他无关文件。所以我们不得不先从寻找所有进程相关文件开始。
```
static void get_pids(const char *dirName){
        //打开指定目录
        DIR *dir = opendir(dirName);
        assert(dir);
        //依次读取目录并转为long类型来保存
        struct dirent *dirItem = NULL;
        while((dirItem = readdir(dir)) != NULL){
                long pid = dirent_to_pid(dirItem);
                if (pid > 0) insert_pid(pid);
        }
}
static long dirent_to_pid(struct dirent* dirItem) {
	assert(dirItem);

	long pid = 0;
	if(dirItem->d_type == DT_DIR){
		char *name = dirItem->d_name;
		for(int i = 0; name[i]; i++){
			if(name[i] > '9' || name[i] < '0')
				return -1;
			pid = pid * 10 + name[i] - '0';
		}
		return pid;
	}
	return -1;
}
```
这里我的做法是遍历/proc目录下所有文件与文件夹，将所有纯数字组成的目录由字符串转化为长整形存储到一个vector中。这样我们就获得了一个包含当前所有进程号的vector，以便后续使用
##### 组织信息结构
刚刚提到，具体包含进程信息的文件是每个进程目录下的“stat”文件，其绝对路径的构成也就是“/proc/<ID>/stat”。因此，我们可以使用vector中保存的进程id信息来拼凑出每个进程的绝对路径，并依次读取其中的信息，具体实现如下：
```
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
```
上面的代码还涉及一些文件批处理操作、字符串操作等，不过都是些库函数的常规用法，还是建议大家自行了解。
###### 进程树
树结构的设计很简单，程序打印时需要几种数据，就相应的创建几个变量来存储就好。需要注意的是树的维护，我这里借助了stl中的map来动态维护PTree
```
struct PTree{
	long pid;
	long ppid;
	std::vector<PTree*> spid;
	char *cmd;
};
//通过map动态维护每个pid对应的进程树节点地址，方便完成插入操作
std::map<int, PTree*> ind;
```
相信大家也注意到了，一个进程可能拥有多个子进程，所以我声明了一个vector类型变量用来存储指向子进程节点的指针。进程树的初始化与子节点的维护代码如下所示：
```
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
```