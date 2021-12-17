gcc -g -o ./build/MandelSaver ./src/mandelSaver.c -lopengl32 -lglu32 -lglew32 -lglfw3 -lm
cp -r ./src/*.glsl ./build/  