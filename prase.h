#include <string.h>
#include <getopt.h>
#include <stdbool.h>

static int show_version = 0, numeric_sort = 0, show_pids = 0;
static const struct option long_option[]={
	{"show_pids", optional_argument, &show_pids, 1},
	{"numeric_sort", optional_argument, &numeric_sort, 1},
	{"show_version", optional_argument, &show_version, 1},
	{NULL, 0, NULL, 0}
};


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

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	


