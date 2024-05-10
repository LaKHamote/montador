all:
	+$(MAKE) -C bin

clean:
	rm -f ./bin/*.o

test:
	cd ./bin && make test