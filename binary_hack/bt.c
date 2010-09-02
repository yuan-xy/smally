#include <execinfo.h>

void foo() {
	void *trace[128];
	int m = sizeof(trace)/sizeof(trace[0]);
	printf("%d\n",m);
	sleep(10);
	int n = backtrace(trace,m);
	backtrace_symbols_fd(trace,n,1);
}
int main(){
	foo();
	return 0;
}
