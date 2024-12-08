
default:
	mkdir -p bin
	gcc src/Main.c core/*.c -o bin/soare.exe -I include -Wall -Wextra -Werror -Wpedantic -Wno-unused-function

run:
	./bin/soare.exe

clean:
	rm -rf bin
