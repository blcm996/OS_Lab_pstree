#include <string.h>
#include <getopt.h>
#include <stdbool.h>
#include "prase.h"
#include <assert.h>

int show_version = 0, numeric_sort = 0, show_pids = 0;

void prase_arguements(int argc, char *argv[]) {
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