APP = calc
CC = clang
CFLAGS = -Wall -g
LDFLAGS = -lfl
OBJECTS = lex.yy.o parser.tab.o ast_cons.o pretty_print.o\
          list.o symtab.o semant.o cgen.o utils.o main.o 

$(APP): $(OBJECTS)
lex.yy.c: lexer.l parser.tab.h parser.tab.c
lex.yy.o: lex.yy.c
parser.tab.h: parser.y
parser.tab.c: parser.y ast.h
parser.tab.o: parser.tab.c parser.tab.h
ast_cons.o: ast_cons.c ast_cons.h
pretty_print.o: pretty_print.c pretty_print.h
list.o: list.c list.h
symtab.o: symtab.c symtab.h type.h list.h utils.h
semant.o: semant.c semant.h
cgen.o: cgen.c cgen.h
utils.o: utils.c utils.h
main.o: main.c ast.h

$(APP):
	$(CC) -o $@ $^ $(LDFLAGS)

parser.tab.h, parser.tab.c:
	bison -d $<

lex.yy.c:
	flex $<

clean:
	rm -rf parser.tab.h parser.tab.c lex.yy.c $(OBJECTS) $(APP)

