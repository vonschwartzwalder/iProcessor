#!/bin/bash

# clean up
rm *.ppm

# run iProcessor
time ./iProcessor.exe -i pipeline.ip -o pipeline.out -image images/input.ppm

# make a montage of all the PNG files
files=`ls *.ppm | sort`
montage -mode Concatenate -pointsize 48 -label %f -tile x1 -frame 1 *.ppm montage.png
imdisplay montage.png
