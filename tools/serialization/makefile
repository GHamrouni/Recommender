CC := gcc
OBJECTS :=  serialize_factors.o Serialize_sparse_matrix.o serialize_training_set.o hiredis/libhiredis.a
CFLAGS= -c -Wall -Wextra -pedantic -Werror -std=c99 -fPIC
all : serialization.a
serialization.a : $(OBJECTS)
	ar -q serialization.a $(OBJECTS)
serialize_factors.o : serialize_factors.c  
	$(CC) $(CFLAGS) -c serialize_factors.c -o serialize_factors.o
Serialize_sparse_matrix.o: serialize_sparse_matrix.c
	$(CC) $(CFLAGS) -c serialize_sparse_matrix.c -o Serialize_sparse_matrix.o
serialize_training_set.o: serialize_training_set.c
	$(CC) $(CFLAGS) -c serialize_training_set.c -o serialize_training_set.o
hiredis/libhiredis.a:
	make -C hiredis
clean :
	rm *.a *.o


