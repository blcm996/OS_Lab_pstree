
#ifndef _TREEPRINT_H_
#define _TREEPRINT_H_
#include <stdio.h>
#include <assert.h>
#include "pidinfo.h"

void print_tree(PTree *root,char *pre, int idx);
void print_tree_n(PTree *root,char *pre, int idx);
void print_tree_pid(PTree *root,char *pre, int idx);
void print_tree_pid_n(PTree *root,char *pre, int idx);
#endif
