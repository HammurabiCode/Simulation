#!/bin/sh
#echo ${1}
curDir=${1}
echo ${curDir}
mkdir ${curDir}/img
rm ${curDir}/img/*.png
for povFile in ${curDir}/pov/* ; do
povray op.ini[High] +I${povFile} +O${curDir}/img/
done
avconv -i ${curDir}/img/%03d.png -r 25 -codec copy ${curDir}/a.avi
#ffmpeg -i ${curDir}/img/GroundGranular%03d.png -vcodec mpeg4 -r 60 -s 3840x2160 ${curDir}/img/g.mp4
