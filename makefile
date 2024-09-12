all:
	echo "builds: linux | windows"
linux:
	g++ ./src/glad.c ./src/*.cpp ./src/classes/*.cpp -I./include -lglfw -lGL -lX11 -lpthread -lXrandr -ldl -o ./mineclone
windows:
	g++ -g -std=c++17 -I./include -L./lib ./src/glad.c ./src/*.cpp ./src/classes/*.cpp -lglfw3dll -o ./mineclone.exe
