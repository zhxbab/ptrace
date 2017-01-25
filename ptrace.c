#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/user.h>
#include <asm/ptrace-abi.h>
#include <asm/ldt.h>
#include "auxiliary.h"
#include <signal.h>

#define TLS_MEM_SIZE 0x100

int main(int argc, char * argv[])
{   pid_t child;
	int status,ret;
	struct user_regs_struct regs;
    unsigned int index = 0x0;
    unsigned int tsl_data = 0x0;
    struct user_desc tls_desc;
	struct args_s pargs = {
		main_eip:0x0,
		file_name:"None"
	};
	struct file_name_set_s file_name_set;
	FILE * tls_file;
	parse_args(argc,argv,&pargs);
    pretty_file_name(pargs.file_name,&file_name_set);
	if((tls_file=fopen(file_name_set.dump_file_name,"w"))==NULL) DEBUG_ERR("open file %s error",file_name_set.dump_file_name);
    child = fork();
    if(child == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl(file_name_set.execl_para_1, file_name_set.execl_para_2, NULL);
    }
    else {
        //ptrace(PTRACE_ATTACH, child, NULL, NULL);
		while(1) {
			wait(&status);
			if(WIFEXITED(status))
				break;
			ptrace(PTRACE_GETREGS,child, NULL, &regs);
#ifdef __x86_64__
			if(regs.rip != pargs.main_eip) {
				ptrace(PTRACE_SINGLESTEP, child,NULL, NULL);
			}
			else{
				fprintf(tls_file,"MAIN ADDR: 0x%08lx\n",regs.rip);
				index = regs.fs>>3;
				ptrace(PTRACE_GET_THREAD_AREA, child, index, &tls_desc);
				fprintf(tls_file,"FS[base]: 0x%08x\n",tls_desc.base_addr);
				fprintf(tls_file,"FS[entry_number]: 0x%08x\n",tls_desc.entry_number);
				fprintf(tls_file,"FS[limit]: 0x%08x\n",tls_desc.limit);
				fprintf(tls_file,"TLS_MEM_DUMP\n");
				for(int i =0; i<TLS_MEM_SIZE ; i=i+4){
					tsl_data = ptrace(PTRACE_PEEKDATA, child, tls_desc.base_addr-i, NULL);
					fprintf(tls_file,"0x%08x:0x%08x\n",tls_desc.base_addr-i,tsl_data);
				}
				//ptrace(PTRACE_CONT, child, NULL, NULL);
				if ((ret = kill(child,SIGKILL))!=-1){
					DEBUG_INFO("kill thread successfully");
					break;
				}
			}
#elif  __i386__
			if(regs.eip != pargs.main_eip) {
				ptrace(PTRACE_SINGLESTEP, child,NULL, NULL);
			}
			else{
				fprintf(tls_file,"MAIN ADDR: 0x%08lx\n",regs.eip);
				index = regs.xgs>>3;
				ptrace(PTRACE_GET_THREAD_AREA, child, index, &tls_desc);
				fprintf(tls_file,"GS[base]: 0x%08x\n",tls_desc.base_addr);
				fprintf(tls_file,"GS[entry_number]: 0x%08x\n",tls_desc.entry_number);
				fprintf(tls_file,"GS[limit]: 0x%08x\n",tls_desc.limit);
				fprintf(tls_file,"TLS_MEM_DUMP\n");
				for(int i =0; i<TLS_MEM_SIZE ; i=i+4){
					tsl_data = ptrace(PTRACE_PEEKDATA, child, tls_desc.base_addr-i, NULL);
					fprintf(tls_file,"0x%08x:0x%08x\n",tls_desc.base_addr-i,tsl_data);
				}
				//ptrace(PTRACE_CONT, child, NULL, NULL);
				if ((ret = kill(child,SIGKILL))!=-1){
					DEBUG_INFO("kill thread successfully");
					break;
				}
			}
#endif
        //ptrace(PTRACE_DETACH, child, NULL, NULL);
		}
	    if ((ret=fclose(tls_file))==-1) DEBUG_ERR("close file error");
    }
    return 0;
}
