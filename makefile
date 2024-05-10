all:
	+$(MAKE) -C bin

clean:
	rm -f ./bin/*.o
	rm -f ./bin/*.exe

test:
	cd ./bin && make test