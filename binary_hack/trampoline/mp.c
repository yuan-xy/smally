#include <stdio.h>
 #include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

double func(void){
	return 3.14;
}

void allow_exec(const void *addr){
	long pagesize=(long)sysconf(_SC_PAGE_SIZE);
	char *p = (char *)((long)addr-pagesize);
	size_t len = pagesize*100L;
	int flag = PROT_READ|PROT_WRITE|PROT_EXEC;
	int i=mprotect((void *)addr,len,flag);
	if(i!=0){
		printf("mprotect failure on %x,error no=%d\n",p,errno);
		exit(i);
	}
}

int main(int argc, const char *argv[])
{
	void *p =(void *) malloc(1000);
	memcpy(p,func,1000);
	allow_exec(p);
	printf("PI is %g\n",((double (*) (void))p)());
	return 0;
}
