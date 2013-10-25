#!/bin/sh
#echo ${1}
curDir=${1}
echo ${curDir}
mkdir ${curDir}/img
rm -rf ${curDir}/img/*
for povFile in ${curDir}/pov/* ; do
povray op.ini[Low] +I${povFile} +O${curDir}/img/
done
avconv -i ${curDir}/img/%03d.png -r 60 -codec copy ${curDir}/a.avi
#ffmpeg -i ${curDir}/img/GroundGranular%03d.png -vcodec mpeg4 -r 60 -s 3840x2160 ${curDir}/img/g.mp4
