
default:
	mkdir -p bin
	gcc src/Main.c core/*.c -o bin/soare.exe -I include -Wall -Wextra -Werror -Wpedantic -Wno-unused-function -Wno-implicit-fallthrough -D __SOARE_DEBUG

run:
	./bin/soare.exe

clean:
	rm -rf bin
