#include <bfd.h>
#define NULL 0
int main (void)
{
	bfd *abfd;
	asection *text;
	asymbol *ptrs[2];
	asymbol *new;

	abfd = bfd_openw ("foo",NULL);
	bfd_set_format(abfd, bfd_object);
	bfd_set_default_target(bfd_elf64_x86_64_vec);
	//bfd_default_set_arch_mach();
	//bfd_set_arch_info(abfd,);
	text = bfd_make_section_old_way(abfd, ".text");
	new = bfd_make_empty_symbol (abfd);
	new->name = "dummy_symbol";
	new->section = text;
	new->flags = BSF_GLOBAL;
	new->value = 0x12345;

	ptrs[0] = new;
	ptrs[1] = 0;

	bfd_set_symtab (abfd, ptrs, 1);
	bfd_close (abfd);
	return 0;
}
