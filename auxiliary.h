#ifndef _AUXILIARY_H
#define _AUXILIARY_H	1

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#ifdef __x86_64__
	typedef unsigned long treg; //reg is 64bit
#elif  __i386__
	typedef long int treg; //reg is 32bit
#endif

struct args_s{
	treg main_eip;
	char file_name[200];
};

struct file_name_set_s{
	char execl_para_1[200];
	char execl_para_2[200];
	char dump_file_name[200];
};
#define USE_DEBUG
#ifdef USE_DEBUG
	#define DEBUG_LINE() printf("[%s:%s] line=%d\r\n",__FILE__, __func__, __LINE__)
	//#define DEBUG_ERR(fmt, args...) printf("\033[47;31m[%s:%d]\033[0m "#fmt" errno=%d, %m\r\n", __func__, __LINE__, ##args, errno, errno)
	#define DEBUG_ERR(fmt, args...) printf("\033[47;31m[%s:%d]\033[0m "#fmt"\n", __func__, __LINE__, ##args)
	#define DEBUG_INFO(fmt, args...) printf("\033[32m[%s:%d]\033[0m "#fmt"\n", __func__, __LINE__, ##args)
#else
	#define DEBUG_LINE()
	#define DEBUG_ERR(fmt,...)
	#define DEBUG_INFO(fmt,...)
#endif


void parse_args(int argc, char* argv[], struct args_s *args);
void help();
void pretty_file_name(char* file_name,struct file_name_set_s *file_name_set);

#endif /* auxiliary.h */
