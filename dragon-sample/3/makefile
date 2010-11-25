SRCS = lex.yy.c parser.tab.c symbol.c\
	init.c error.c main.c

OBJS=$(SRCS:.c=.o)
MAKEDEPEND=gcc -MM


trans: ${OBJS}
	cc $(OBJS) -lfl -ly -o $@

$(OBJS) : %.o : %.c
	gcc -c -o $@ $<

parser.tab.c : parser.y
	bison parser.y


lex.yy.c : test.flex
	flex test.flex

.PHONY: clean

clean: 
	-rm .depend lex.yy.c parser.tab.c *.o *.exe *.bak

#自动生成依赖方案1
.depend: $(SRCS)
	$(MAKEDEPEND) $(SRCS) > $@

depend:
	-rm .depend
	$(MAKE) .depend

include .depend


#自动生成依赖的方案2
#include $(subst .c,.d,$(SRCS))
#
#%.d : %.c
#	gcc -M $< > $@.$$$$;	\
#	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
#	rm -f $@.$$$$

ifndef verbose
	v:=@
endif

test :
	$v echo test.
	@echo $(origin verbose)
	$(warning 下面的代码为什么会出错)
	ifdef verbose
		@echo verbose.
	endif