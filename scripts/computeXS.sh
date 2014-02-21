#!/bin/bash

rm -f xsec

for FILE in `cat files`
do

  echo $FILE
  ln -sf $FILE gen.root
  cmsRun runTestGenInfoProduct_cfg.py >> xsec

done

grep Internal xsec | awk '{print $5,$7}' > dat.txt

root -l -q -b weighted_mean.C

