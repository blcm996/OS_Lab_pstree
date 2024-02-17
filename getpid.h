#include <dirent.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
//static pid_t* pids = NULL;
static std::vector <long> pids;
/*
static int pids_capacity = 0, pids_number = 0;
const static int pids_initial_capacity = 8;
const static char *procfs_dir = "/proc/";
*/	

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

static void insert_pid(long pid){
	assert(pid > 0);
	/*
	//pids内存初始化
	if(pids == NULL){
		pids_capacity = pids_initial_capacity;
		pids_number = 0;
		pids = (pid_t*)malloc(sizeof(pid_t) * pids_capacity);
	}
	//pids内存动态分配
	if(pids_number == pids_capacity){
		pids_capacity *= 2;
		pids = (pid_t*)realloc(pids, sizeof(pid_t) * pids_capacity);
	}
	*/
	//由vector自动完成内存动态分配
	pids.push_back(pid);
	//assert(pids != 0);
	//pids[pids_number++] = (pid_t)pid;
}

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
