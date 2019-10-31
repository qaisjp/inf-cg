.PHONY: app test remake
app:
	cd build && make raytracer/fast && cd ..

remake:
	rm -rf build
	mkdir build
	cd build && cmake3 -DCMAKE_BUILD_TYPE=Debug ..
	${MAKE} app

F?=examples/example.json

test: app
	./build/raytracer "$F" "$F.ppm"
	feh "$F.ppm" --scale-down --auto-zoom
