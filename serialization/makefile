CC := gcc
OBJECTS :=  serialize_factors.o Serialize_sparse_matrix.o serialize_training_set.o
all : serialization.a
serialization.a : $(OBJECTS)
	ar -q serialization.a $(OBJECTS)
serialize_factors.o : serialize_factors.c  
	$(CC) -c serialize_factors.c -o serialize_factors.o
Serialize_sparse_matrix.o: serialize_sparse_matrix.c
	$(CC) -c serialize_sparse_matrix.c -o Serialize_sparse_matrix.o
serialize_training_set.o: serialize_training_set.c
	$(CC) -c serialize_training_set.c -o serialize_training_set.o
clean :
	rm *.a *.o


