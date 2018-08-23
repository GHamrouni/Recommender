TARGETS := data_set.o find_minimum_tsearch.o k_fold_rmse.o k_fold_test.o ../../src/libRecommender.a 
LIBS := -lm
CC := gcc
CFLAGS :=-I../../src/ -std=c99 -c -Wall -Wextra -pedantic -Werror -g -fPIC
all : $(TARGETS)
	$(CC) $(TARGETS) $(LIBS) -o kfold 
.c.o:
	$(CC) $(CFLAGS) $< -o $@
../../src/libRecommender.a :
	make -C ../../src/
clean :
	rm -f *.o kfold
