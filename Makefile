all: main

main: main.c
	$(CC) -o main main.c `pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0`

clean:
	rm main
