#!/bin/bash

filename=$(tempfile --suffix ".png");
echo "filename: " $filename

import -window root $filename

~/docs/src/photoTweaker/bin/photoTweaker $filename

url=$(curl  -s --form "nickname=$(hostname)" --form \
 "image=@$filename;type=$(file --brief -L --mime-type $filename)"\
  --form "disclaimer_agree=Y" --form "mode=add" --form "Submit=Submit"\
  http://imagebin.org/index.php -w '%{redirect_url}')
echo "url: " $url

type xclip >/dev/null 2>/dev/null && echo -n $url | xclip;

rm $filename
