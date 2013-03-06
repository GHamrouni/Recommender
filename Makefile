#--------------------------------------
# Makefile for all Recommender packages
#--------------------------------------

default:
	- ( cd src && $(MAKE) all )

all:
	- ( cd src && $(MAKE) all )

purge:
	- ( cd src && $(MAKE) purge )

clean:
	- ( cd src && $(MAKE) clean )
