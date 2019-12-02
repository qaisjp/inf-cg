#!/usr/bin/env bash

for nv in 10 100 1000 10000; do
    for nu in 10 100 1000 10000; do
        x='Material "anph"'
        nv_p='"float nv"'
        nu_p='"float nu"'

        # echo 'pre'
        # sed "s/$x/$x $nv_p [$nv] $nu_p [$nu]/" in.pbrt
        # echo 'hi'

        T="$(sed "s/$x/$x $nv_p [$nv] $nu_p [$nu]/" in.pbrt |\
            sed "s/sphere\.exr/sphere-Nv${nv}-Nu${nu}\.exr/")"
        # echo "$T"
        printf "%s" "$T" | ../pbrt-v3/cmake-build-release/pbrt &
    done
done

wait

echo "Done"
# for nv in 10 100 1000 10000; do
#     for nu in 10 100 1000 10000; do
#         rm