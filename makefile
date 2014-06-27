cardinality:
	gcc -std=c99 -Wall -O3 *.c zlib/*.c tests/*.c libs/*.c -o cardinality -lm

test:
	./cardinality --test

debug:
	gcc -std=c99 -Wall -ggdb *.c zlib/*.c tests/*.c libs/*.c -o cardinality -lm

.PHONY: test
.PHONY: cardinality
.PHONY: debug
