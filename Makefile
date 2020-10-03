build:
	g++ -w -std=c++14 -Wfatal-errors \
	./src/*.cpp \
	./src/game/*.cpp \
	-o ./build/game \
	-I"./lib/lua" \
	-L"./lib/lua" \
	-llua5.3 \
	-lSDL2 \
	-lSDL2_image \
	-lSDL2_ttf \
	-lSDL2_mixer;

clean:
	[ -f ./build/game ] && rm ./build/game; # remove game file only if it exists

run:
	./build/game;
