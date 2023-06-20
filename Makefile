all:
	g++ src/*.cpp -O3 -o raytracer

run:
	g++ src/*.cpp -O3 -o raytracer
	./raytracer

debug:
	g++ src/*.cpp -g -o raytracer

run_debug:
	g++ src/*.cpp -g -o raytracer
	./raytracer