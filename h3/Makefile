all:
	gcc -o h3_ex3 -Wall -std=c11 src/deque.c src/dictionary.c src/exception.c src/main.c src/utils.c
	cp h3_ex3 testcase/he_ex3 
	
test:
	gcc -o h3_ex3_test -Wall -std=c11 src/deque.c src/dictionary.c src/exception.c src/test.c src/utils.c 
	
clean:
	rm -f h3_ex3 h3_ex3_test
