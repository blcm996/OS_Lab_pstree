#ifndef _GETPID_H_
#define _GETPID_H_
#include <dirent.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
//static pid_t* pids = NULL;
extern std::vector <long> pids;
/*
static int pids_capacity = 0, pids_number = 0;
const static int pids_initial_capacity = 8;
const static char *procfs_dir = "/proc/";
*/	

long dirent_to_pid(struct dirent* dirItem) ;

void insert_pid(long pid);

void get_pids(const char *dirName);
#endif