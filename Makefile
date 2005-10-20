all:
	cd src/ && make

clean:
	cd src/ && make clean
	cd src/ && make -f Makefile.mgw clean

test:
	cd src/ && make test
