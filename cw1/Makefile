SAMPLES ?= 10
OUTFILE ?= pbrt.png

project:
	sed 's/"integer pixelsamples" 4/"integer pixelsamples" ${SAMPLES}/' /tmp/scene.pbrt |\
	sed 's/Film "image"/Film "image" "string filename" "${OUTFILE}"/' |\
	sed 's/"float fov" \[24.56717103880224\]/"float fov" [35]/' |\
	sed '/WorldBegin/r sceneplus.pbrt'\
	> /tmp/scene.out.pbrt

	if [ ${SAMPLES} -ge 512 ]; then \
		rsync /tmp/scene.out.pbrt /tmp/tex_* cirrus:; \
		time ssh cirrus pbrt-v3/build/pbrt scene.out.pbrt; \
		scp cirrus:${OUTFILE} .; \
	else \
		time pbrt /tmp/scene.out.pbrt; \
	fi

	# Open the resultant image
	open ${OUTFILE}
