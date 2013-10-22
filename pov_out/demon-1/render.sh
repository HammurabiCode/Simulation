#!/bin/sh
#dir = ${PWD}/pov
#echo ${PWD}
#outDir = '${PWD}/img/'
rm -rf ${PWD}/img/*
for povFile in ${PWD}/pov/* ; do
povray op.ini +I${povFile} +O${PWD}/img/
done
