run:
	g++ src/*.cpp -g -o raytracer
	./raytracer

run_show:
	g++ src/*.cpp -g -o raytracer
	./raytracer
	python3 vis_img.py