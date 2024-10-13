
default:
	mkdir -p bin
	gcc app/Main.c src/*.c -o bin/soare -I include

run:
	./bin/soare

clean:
	rm -drf bin
