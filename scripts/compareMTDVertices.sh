#!/bin/sh

for histo in `root -l -q -b 'navigateROOTFile.C("new.root","DQMData/Run 1/MTD/Run summary/Vertices")' | grep ^[0-9] | cut -f 2 -d " "`; do root -l -b -q 'compareHisto.C(2,"old.root","standard","new.root","new","three.root","Summer12 best","four.root","Summer12 PU -300 ns","'${histo}'.pdf","DQMData/Run 1/MTD/Run summary/Vertices","'${histo}'",true,"E1",false,false,false,0.,0.,0.,0.,false,false,1.,1.,1.,1.,false,false,"BR")' ; done

for histo in `root -l -q -b 'navigateROOTFile.C("new.root","DQMData/Run 1/MTD/Run summary/Vertices")' | grep ^[0-9] | cut -f 2 -d " "`; do echo root -l -b -q 'compareHisto.C(2,"old.root","standard","new.root","new","three.root","Summer12 best","four.root","Summer12 PU -300 ns","'${histo}'.pdf","DQMData/Run 1/MTD/Run summary/Vertices","'${histo}'",true,"E1",false,false,false,0.,0.,0.,0.,false,false,1.,1.,1.,1.,false,false,"BR")' >> esegui ; done

sed s/compa/\'compa/ esegui | sed s/")/"\)\'/ > pippo ; mv pippo esegui

cat esegui  | while read -r i; do printf "%q\n" "$i"; done | xargs -P 10 -L 1 -I % sh -c %
