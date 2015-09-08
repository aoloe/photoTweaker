#!/bin/bash

# make sure that the camera/card is mount with lowercase file names
# UUID=6664-3139 /mnt/camera    vfat    defaults,user,rw,noauto,shortname=lower  0       0

filename=$(tempfile --suffix ".jpg");
echo "filename: " $filename

mount /mnt/nikon 2> /dev/null
#echo $q

q=$(ls -tr1 /mnt/nikon/dcim 2> /dev/null | tail -n 1)

if [ "$q" != "" ]; then
    r=$(ls -tr1 /mnt/nikon/dcim/$q 2> /dev/null | tail -n 1)
    #echo $r
    if [ "$q" != "" ]; then
        # copy or move each token to /tmp
        for item in $r; do
            echo $item
            cp /mnt/nikon/dcim/$q/$item $filename
        done
    else
        echo "could not find any image"
    fi
else
    echo "could not mount the sd card"
fi

umount /mnt/nikon 2> /dev/null


~/docs/src/photoTweaker/bin/photoTweaker $filename

claws-mail --compose --attach $filename

# rm $filename
