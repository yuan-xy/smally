            #include <bfd.h>
#define NULL 0
            int main (void)
	            {
				              bfd *abfd;
				              asymbol *ptrs[2];
				              asymbol *new;
				
				              abfd = bfd_openw ("foo",NULL);
				              bfd_set_format (abfd, bfd_object);
				              new = bfd_make_empty_symbol (abfd);
				              new->name = "dummy_symbol";
				              new->section = bfd_make_section_old_way (abfd, ".text");
				              new->flags = BSF_GLOBAL;
				              new->value = 0x12345;
				
				              ptrs[0] = new;
				              ptrs[1] = 0;
				
				              bfd_set_symtab (abfd, ptrs, 1);
				              bfd_close (abfd);
				              return 0;
				            }
