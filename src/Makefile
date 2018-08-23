CC=gcc
CFLAGS=-c -Wall -Wextra -pedantic -Werror -g -fPIC
LDFLAGS=
LIBS=-lm
SOURCES=training_set.c utils.c learned_factors.c matrix_factorization.c matrix_factorization_bias.c \
projection.c projection_family.c standard_normal.c sparse_matrix_hash.c \
sparse_matrix.c recommender.c float_tester.c recommended_items.c red_black_tree.c \
binary_heap.c rlog.c neighborMF.c items_rated_by_user.c box_muller.c social_reg.c
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
