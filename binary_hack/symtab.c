#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <bfd.h>

void dump_symbols(const char *filename){
	bfd *abfd;
	long storage;
	asymbol **syms;
	int symnum,i;
	size_t size;
	int ret;

	abfd=bfd_openr(filename,NULL);
	assert(abfd);
	ret=bfd_check_format(abfd,bfd_object);
	assert(ret);
	if(!(bfd_get_file_flags(abfd)&& HAS_SYMS)){
		bfd_close(abfd);
		return;
	}

	storage = bfd_get_symtab_upper_bound(abfd);
	assert(storage>=0);
	if(storage) syms=(asymbol **)malloc(storage);
	symnum=bfd_canonicalize_symtab(abfd,syms);
	assert(symnum>=0);

	for(i=0;i<symnum;i++){
		asymbol *sym = syms[i];
		int value = bfd_asymbol_value(sym);
		const char *file,*name;
		int lineno;
		asection *dbgsec = bfd_get_section_by_name(abfd,".debug_info");
		ret = bfd_find_nearest_line(abfd,dbgsec,syms,value,&file,&name,&lineno);
		if(ret&&file&&name){
			printf("%08x %s (%s:%d)\n",value,name,file,lineno);
		}else{
			name = bfd_asymbol_name(sym);
			printf("%08x %s\n",value,name);
		}
	}
	free(syms);
	bfd_close(abfd);
}

int main(int argc,char *argv[]){
	dump_symbols(argv[0]);
	return 0;
}
