project:
	#obj2pbrt geometry/scene.obj geometry/scene.pbrt
	#cp /tmp/exp2.pbrt geometry/scene.pbrt
	#pbrt project.pbrt
	#open project.exr
	sed 's/"integer pixelsamples" 4/"integer pixelsamples" 512/' /tmp/scene.pbrt |\
	sed '/WorldBegin/r sceneplus.pbrt'\
	> /tmp/scene.out.pbrt
	pbrt /tmp/scene.out.pbrt
	open pbrt.exr
