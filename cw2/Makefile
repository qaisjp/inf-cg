.PHONY: app test
app:
	cd build && make raytracer/fast && cd ..

F?=examples/example.json

test: app
	./build/raytracer "$F" "$F.ppm"
	feh "$F.ppm" --scale-down --auto-zoom
