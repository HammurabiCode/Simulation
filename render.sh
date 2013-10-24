#!/bin/sh
#echo ${1}
curDir=${1}
echo ${curDir}
rm -rf ${curDir}/img/*
for povFile in ${curDir}/pov/* ; do
povray op.ini +I${povFile} +O${curDir}/img/
done
#ffmpeg -i ${curDir}/img/GroundGranular%03d.png -vcodec mpeg4 -r 60 -s 3840x2160 ${curDir}/img/g.mp4
