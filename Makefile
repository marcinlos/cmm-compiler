bin=bin
src=src

CFLAGS := -g

PROG := compile
COMPILER := gcc ${CFLAGS}

LEX := flex
YACC := bison -d

FLEX_FILES := lexer.lex
FLEX_SOURCES := ${patsubst %.lex,%.c,${FLEX_FILES}}

YACC_FILES := parser.yy
YACC_SOURCES := ${patsubst %.yy,%.tab.c,${YACC_FILES}}
YACC_HEADERS := ${patsubst %.yy,%.tab.h,${YACC_FILES}}


SRC_FILES := ${YACC_SOURCES} \
			 ${FLEX_SOURCES} \
			 util.c			 \
			 ast.c 			 \
			 main.c 		 \
			 types.c		 \
			 symbol_table.c	 \
			 static_analysis.c \
			 ir.c 			 \
		 	 generator.c 	 \
		 	 line_list.c	 \
		 	 ir_list.c		 \
		 	 ir_generator.c	 \
		 	 mips.c			 \
		 	 mips_list.c	 \
		 	 ir_to_mips.c	 
		 	 
OBJ_FILES := ${patsubst %.c,%.o,${SRC_FILES}}

all: ${PROG}

${PROG}: ${OBJ_FILES}
	${COMPILER} -o $@ $^

${FLEX_SOURCES}: %.c: %.lex
	${LEX} -o $@ $^

${YACC_SOURCES}: %.tab.c: %.yy
	${YACC} -o $@ $^

.PHONY: clean
clean:
	rm -rf ${OBJ_FILES} ${FLEX_SOURCES} ${YACC_SOURCES} ${YACC_HEADERS} ${PROG} 
