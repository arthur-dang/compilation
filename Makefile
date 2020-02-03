ALPAGA=alpaga/parser_generator
TP=1

all:
	make -C alpaga
	make expr/expr_parser.c
	make -C expr

expr/expr_parser.c: $(ALPAGA) expr_grammar_action.g
	$(ALPAGA) -g expr_grammar_action.g -pc expr/expr_parser.c -ph expr/expr_parser.h -t expr/table.html

test: all
	./test.sh expr $(TP)

clean:
	make clean -C alpaga
	make clean -C expr
