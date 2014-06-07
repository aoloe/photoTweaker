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

function upload_to_imagebin() {
    filename=$1

    url=$(curl  -s --form "nickname=$(hostname)" --form \
     "image=@$filename;type=$(file --brief -L --mime-type $filename)"\
      --form "disclaimer_agree=Y" --form "mode=add" --form "Submit=Submit"\
      http://imagebin.org/index.php -w '%{redirect_url}')
    echo $url
}

function upload_to_imgur() {
    # snippet adapted from
    # https://github.com/jomo/imgur-screenshot/blob/master/imgur-screenshot.sh
    # (MIT license by jomo)
    # an alternative: https://github.com/Ceryn/img/blob/master/img.sh
    filename=$1
    imgur_anon_key="486690f872c678126a2c09a9e196ce1b"

    upload_connect_timeout="5"
    upload_timeout="120"
    upload_retries="1"

    response="$(curl --connect-timeout "$upload_connect_timeout" -m "$upload_timeout" --retry "$upload_retries" -s -F "image=@$filename" -F "key=$imgur_anon_key" https://imgur.com/api/upload.xml)"

    # imgur response contains stat="ok" when successful
    if [[ "$response" == *"stat=\"ok\""*  ]]; then
        # cutting the url from the xml response
        url="$(egrep -o "<original_image>.*</original_image>" <<<"$response" | cut -d ">" -f 2 | cut -d "<" -f 1)"
        # del_url="$(egrep -o "<delete_page>.*</delete_page>" <<<"$response" | cut -d ">" -f 2 | cut -d "<" -f 1)"
    else # upload failed
        url="$(egrep -o "<error_msg>.*</error_msg>" <<<"$response" | cut -d ">" -f 2 | cut -d "<" -f 1)"
    fi
    echo $url
}

#url=$(upload_to_imagebin $filename)
url=$(upload_to_imgur $filename)


type xclip >/dev/null 2>/dev/null && echo -n $url | xclip;

rm $filename
