OBJS = lexer.o parser.o emitter.o symbol.o\
	init.o error.o main.o
trans: $(OBJS)
	cc $(OBJS) -o trans
$(OBJS): global.h
clean: 
	rm *.exe *.o *.bak