TARGETS := data_set.o find_minimum_tsearch.o k_fold_rmse.o k_fold_test.o ../serialization/serialization.a ../libRecommender.a ../hiredis/libhiredis.a
LIBS := -lm
CC := gcc
CFLAGS :=-std=c99 -c -Wall -Wextra -pedantic -Werror
all : $(TARGETS)
	$(CC) $(TARGETS) $(LIBS) -o kfold 
.c.o:
	$(CC) $(CFLAGS) $< -o $@
../serialization/serialization.a :
	make -C ../serialization/	
../libRecommender.a :
	make -C ../
../hiredis/libhiredis.a :
	make -C ../hiredis
clean :
	rm -f *.o kfold
