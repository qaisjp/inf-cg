project:
	#obj2pbrt geometry/scene.obj geometry/scene.pbrt
	#cp /tmp/exp2.pbrt geometry/scene.pbrt
	#pbrt project.pbrt
	#open project.exr
	sed 's/"integer pixelsamples" 4/"integer pixelsamples" 200/' /tmp/scene.pbrt |\
	sed '/WorldBegin/r sceneplus.pbrt'\
	> scene.out.pbrt
	pbrt scene.out.pbrt | less
	open pbrt.exr
