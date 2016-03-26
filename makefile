TESTS_SOURCES=$(wildcard tests/test_*.c)
TESTS=$(TESTS_SOURCES:.c=)

CPPFLAGS=-g -ggdb -O0 -std=c11 -Wall -Werror -I.
CFLAGS=-fPIC -ggdb -I. 
LDLIBS=-lm

all: libautomate.a

check: test
	for i in $(TESTS); do \
		echo -n "$$i ... "; \
		eval "$$i" > log 2>&1; \
		if test $$? -eq 0; then \
			echo "\033[32mPASS\033[0m"; \
		else \
			echo "\033[31mFAIL\033[0m" && echo "" && cat log && echo ""; \
		fi \
	done

checkmemory: test
	for i in $(TESTS); do \
		echo -n "$$i ... "; \
		eval "valgrind --error-exitcode=1 --leak-check=full --errors-for-leak-kinds=definite,indirect,possible $$i" > log 2>&1; \
		if test $$? -eq 0; then \
			echo "\033[32mMEMORY PASS\033[0m"; \
		else \
	        echo "\033[31mMEMORY FAIL\033[0m" && echo "" && cat log && echo ""; \
	    fi \
	done

test: all
	echo "$(TESTS)" |sed -e "s#\([^ ]*\) *#\1: \1.o libautomate.a\n#g" > tests.mk
	make test_2

test_2: $(TESTS)

-include tests.mk

libautomate.a: libautomate.a(automate.o table.o ensemble.o avl.o fifo.o outils.o)

doc:
	doxygen

clean:
	-rm -rf html latex
	-rm -rf *.o
	-rm -rf *.a
	-rm -rf *.mk
	-rm -rf tests/*.o
	-rm -rf $(TESTS)

.PHONY: all clean check checkmemory doc test
