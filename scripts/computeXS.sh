#!/bin/bash

rm -f xsec

for FILE in `cat files`
do

  echo $FILE
  ln -sf $FILE gen.root
  cmsExec.sh runTestGenInfoProduct_cfg.py >> xsec
  if [ $? != 0 ]; then echo "Problems in" $FILE ; fi 

done

grep Internal xsec | awk '{print $5,$7}' > dat.txt

root -l -q -b weighted_mean.C

