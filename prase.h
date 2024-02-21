#ifndef _PRASE_H_
#define _PRASE_H_
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


void prase_arguements(int argc, char *argv[]);

#endif
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	


