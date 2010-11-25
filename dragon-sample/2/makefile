OBJS = lex.yy.o parser.o emitter.o symbol.o\
	init.o error.o main.o
trans: $(OBJS)
	cc $(OBJS) -lfl -o trans
$(OBJS): global.h
lex.yy.o: global.h
lex.yy.c: 
	flex test.flex
clean: 
	rm lex.yy.c *.exe *.o *.bak