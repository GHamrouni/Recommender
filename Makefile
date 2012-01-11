CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
LIBS=-lm
SOURCES=training_set.c utils.c learned_factors.c matrix_factorization.c matrix_factorization_bias.c Recommender.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=test
OUT = libRecommender.a
.PHONY: clean purge

all: $(SOURCES) $(OUT)

$(OUT): $(OBJECTS) 
	ar rcs $(OUT) $(OBJECTS)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

purge: clean
	rm -f *.o
