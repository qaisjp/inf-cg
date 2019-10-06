SAMPLES ?= 10
OUTFILE ?= pbrt.png

project:
	sed 's/"integer pixelsamples" 4/"integer pixelsamples" ${SAMPLES}/' /tmp/scene.pbrt |\
	sed 's/Film "image"/Film "image" "string filename" "${OUTFILE}"/' |\
	sed 's/"float fov" \[24.56717103880224\]/"float fov" [35]/' |\
	sed '/WorldBegin/r sceneplus.pbrt'\
	> /tmp/scene.out.pbrt

	if [ ${SAMPLES} -ge 512 ]; then \
		rsync /tmp/scene.out.pbrt /tmp/mesh_*.ply /tmp/tex_*.png cirrus:; \
		time ssh cirrus pbrt-v3/build/pbrt scene.out.pbrt; \
		scp cirrus:${OUTFILE} .; \
	else \
		time pbrt /tmp/scene.out.pbrt; \
	fi

	# time pbrt /tmp/scene.out.pbrt

	# Open the resultant image
	open ${OUTFILE}
