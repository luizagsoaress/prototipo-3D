CC = gcc
GUI_FLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

install:
	$(CC) prototipo3D.c -o prototipo3D $(GUI_FLAGS)
	mkdir -p ~/.local/bin
	mkdir -p ~/.local/share/prototipo3D/assets
	cp prototipo3D ~/.local/bin/prototipo3D
	cp -r assets/models/* ~/.local/share/prototipo3D/assets
	chmod +x ~/.local/bin/prototipo3D
run:
	~/.local/bin/prototipo3D