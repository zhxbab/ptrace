#include "auxiliary.h"
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include "ex_string.h"

void parse_args(int argc, char* argv[], struct args_s *pargs){
    int re;
    if(argc == 1){
    	DEBUG_ERR("ptrace only has one parameter");
		help();
		return;
    }
    while((re = getopt(argc, argv, "f:a:h")) != -1){
		//DEBUG_INFO("option char: %c", re);
		switch(re){
		case 'a':
			sscanf(optarg,"%lx",&(pargs->main_eip));
			DEBUG_INFO("main address is 0x%08lx",pargs->main_eip);
			break;
		case 'f':
			strcpy(pargs->file_name,optarg);
			DEBUG_INFO("the file name is %s", pargs->file_name);
			break;
		case '?':
	    	DEBUG_ERR("ptrace don't support this option");
			help();
			break;
		case ':':
			DEBUG_ERR("miss option char in optstring");
			break;
		case 'h':
			help();
			break;
		}
    }
    if((!strcmp("None",pargs->file_name))|(pargs->main_eip == 0x0)){
		DEBUG_ERR("ptrace miss option");
		help();
    }
}

void help(){

	DEBUG_INFO("Usage: ptrace [-f file] [-a addr]");
	return;
}

void pretty_file_name(char* file_name,struct file_name_set_s *file_name_set){
	char file_name_ori[200];
	char *temp_file, sep = '/';
	strcpy(file_name_ori,file_name);
	replaceFirst(file_name_ori,".elf",".tls");
	strcpy(file_name_set->dump_file_name,file_name_ori);
	DEBUG_INFO("TLS file name is %s",file_name_set->dump_file_name);
	strcpy(file_name_ori,file_name);
	if((temp_file = strrchr(file_name_ori,sep)) == NULL){
		strcpy(file_name_set->execl_para_2,file_name_ori);
		sprintf(file_name_set->execl_para_1,"%s%s","./",file_name_ori);
	}
	else{
		//DEBUG_INFO("temp_file is %s",temp_file);
		strcpy(file_name_set->execl_para_1,file_name_ori);
		sprintf(file_name_set->execl_para_2,"%s",temp_file+1);
	}
	DEBUG_INFO("execl para 1 is %s",file_name_set->execl_para_1);
	DEBUG_INFO("execl para 2 is %s",file_name_set->execl_para_2);
	return;
}
