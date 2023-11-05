.PHONY: all

all: DFA

DFA: y.tab.c toDFA.c
	gcc -o $@ $^

y.tab.c: toNFA.y
	yacc $<

clean:
	rm -f y.tab.c DFA