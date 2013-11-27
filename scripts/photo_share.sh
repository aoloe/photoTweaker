#!/bin/bash

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

url=$(curl  -s --form "nickname=$(hostname)" --form \
 "image=@$filename;type=$(file --brief -L --mime-type $filename)"\
  --form "disclaimer_agree=Y" --form "mode=add" --form "Submit=Submit"\
  http://imagebin.org/index.php -w '%{redirect_url}')
echo "url: " $url

type xclip >/dev/null 2>/dev/null && echo -n $url | xclip;

rm $filename
