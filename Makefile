#--------------------------------------
# Makefile for all Recommender packages
#--------------------------------------

default:
	- ( cd src && $(MAKE) all )
	- ( cd tools/kfold && $(MAKE) all )

all:
	- ( cd src && $(MAKE) all )
	- ( cd tools/kfold && $(MAKE) all )

purge:
	- ( cd src && $(MAKE) purge )

clean:
	- ( cd src && $(MAKE) clean )
	- ( cd tools/kfold && $(MAKE) clean )
