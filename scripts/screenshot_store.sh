#!/bin/bash

filename=$(tempfile --suffix ".png");
echo "filename: " $filename

import -window root $filename

target_filename=`zenity --file-selection --save --confirm-overwrite`
# target_filename=`zenity --file-selection --save --confirm-overwrite --directory --filename=/tmp/` <-- this creates a directory with the png name <-- this creates a directory with the png name

case $? in
         0)
                echo "\"$target_filename\" selected."
                # TODO: make sure that the filename is .png
                mv $filename $target_filename
                ~/docs/src/photoTweaker/bin/photoTweaker $target_filename;;
         1)
                echo "No file selected.";;
        -1)
                echo "An unexpected error has occurred.";;
esac

rm $filename
