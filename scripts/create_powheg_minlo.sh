#!/bin/bash

#set -o verbose

set -x

EXPECTED_ARGS=6

if [ $# -ne $EXPECTED_ARGS ]
then
  echo "Usage: `basename $0` repository name process card Nevents RandomSeed "
  echo "process names are: Dijet Zj WW hvq WZ  W_ew-BW Wbb Wj VBF_Hgg_H W Z  Wp_Wp_J_J VBF_Wp_Wp ZZ"  
  echo "Example: ./create_lhe_powheg.sh slc5_ia32_gcc434/powheg/V1.0/src powhegboxv1.0_Jan2012 Z slc5_ia32_gcc434/powheg/V1.0/8TeV_Summer12/DYToEE_M-20_8TeV-powheg/v1/DYToEE_M-20_8TeV-powheg.input 1000 1212" 
  exit 1
fi

echo "   ______________________________________     "
echo "         Running Powheg                       "
echo "   ______________________________________     "

repo=${1}
echo "%MSG-POWHEG repository = $repo"

name=${2} 
echo "%%MSG-POWHEG name = $name"

process=${3}
echo "%MSG-POWHEG process = $process"

cardinput=${4}
echo "%MSG-POWHEG location of the card = $cardinput"

nevt=${5}
echo "%MSG-POWHEG number of events requested = $nevt"

rnum=${6}
echo "%MSG-POWHEG random seed used for the run = $rnum"


seed=$rnum
file="events"
# Release to be used to define the environment and the compiler needed
export PRODHOME=`pwd`
export SCRAM_ARCH=${SCRAM_ARCH}
export RELEASE=${CMSSW_VERSION}
export WORKDIR=`pwd`

# Get the input card
#wget --no-check-certificate http://cms-project-generators.web.cern.ch/cms-project-generators/${cardinput} -O powheg.input
cp ${repo}/${cardinput} powheg.input
card="$WORKDIR/powheg.input"

# initialize the CMS environment 
scram project -n ${name} CMSSW ${RELEASE} ; cd ${name} ; mkdir -p work ; cd work
eval `scram runtime -sh`

# force the f77 compiler to be the CMS defined one
#ln -s `which gfortran` f77
#ln -s `which gfortran` g77
export PATH=`pwd`:${PATH}

# FastJet and LHAPDF
#fastjet-config comes with the paths used at build time.
#we need this to replace with the correct paths obtained from scram tool info fastjet

newinstallationdir=`scram tool info fastjet | grep FASTJET_BASE |cut -d "=" -f2`
cp ${newinstallationdir}/bin/fastjet-config ./fastjet-config.orig

oldinstallationdir=`cat fastjet-config.orig | grep installationdir | head -n 1 | cut -d"=" -f2`
sed -e "s#${oldinstallationdir}#${newinstallationdir}#g" fastjet-config.orig > fastjet-config 
chmod +x fastjet-config

#same for lhapdf
newinstallationdirlha=`scram tool info lhapdf | grep LHAPDF_BASE |cut -d "=" -f2`
cp ${newinstallationdirlha}/bin/lhapdf-config ./lhapdf-config.orig
oldinstallationdirlha=`cat lhapdf-config.orig | grep prefix | head -n 1 | cut -d"=" -f2`
sed -e "s#prefix=${oldinstallationdirlha}#prefix=${newinstallationdirlha}#g" lhapdf-config.orig > lhapdf-config
chmod +x lhapdf-config

#svn checkout --username anonymous --password anonymous svn://powhegbox.mib.infn.it/trunk/POWHEG-BOX
# # retrieve the wanted POWHEG-BOX from the official repository 

wget --no-check-certificate http://cms-project-generators.web.cern.ch/cms-project-generators/${repo}/${name}.tar.gz  -O ${name}.tar.gz
#cp ${repo}/${name}.tar.gz ${name}.tar.gz
tar xzf ${name}.tar.gz

cat > powheg-box.patch <<EOF
diff -Naur POWHEG-BOX.orig/Version-pre2-1/cernroutines.f POWHEG-BOX/Version-pre2-1/cernroutines.f
--- POWHEG-BOX.orig/Version-pre2-1/cernroutines.f	2013-10-03 16:01:59.000000000 +0200
+++ POWHEG-BOX/Version-pre2-1/cernroutines.f	2013-10-03 16:30:29.000000000 +0200
@@ -793,26 +793,26 @@
 
 
 
-c# 10 "lenocc.F" 2
-      FUNCTION LENOCC (CHV)
-C
-C CERN PROGLIB# M507    LENOCC          .VERSION KERNFOR  4.21  890323
-C ORIG. March 85, A.Petrilli, re-write 21/02/89, JZ
-C
-C-    Find last non-blank character in CHV
-
-      CHARACTER    CHV*(*)
-
-      N = LEN(CHV)
-
-      DO 17  JJ= N,1,-1
-      IF (CHV(JJ:JJ).NE.' ') GO TO 99
-   17 CONTINUE
-      JJ = 0
-
-   99 LENOCC = JJ
-      RETURN
-      END
+C c# 10 "lenocc.F" 2
+C       FUNCTION LENOCC (CHV)
+C C
+C C CERN PROGLIB# M507    LENOCC          .VERSION KERNFOR  4.21  890323
+C C ORIG. March 85, A.Petrilli, re-write 21/02/89, JZ
+C C
+C C-    Find last non-blank character in CHV
+
+C       CHARACTER    CHV*(*)
+
+C       N = LEN(CHV)
+
+C       DO 17  JJ= N,1,-1
+C       IF (CHV(JJ:JJ).NE.' ') GO TO 99
+C    17 CONTINUE
+C       JJ = 0
+
+C    99 LENOCC = JJ
+C       RETURN
+C       END
 c# 1 "mtlset.F"
 c# 1 "<built-in>"
 c# 1 "<command line>"
diff -Naur POWHEG-BOX.orig/Version-pre2-1/include/pwhg_par.h POWHEG-BOX/Version-pre2-1/include/pwhg_par.h
--- POWHEG-BOX.orig/Version-pre2-1/include/pwhg_par.h	2013-10-03 16:01:58.000000000 +0200
+++ POWHEG-BOX/Version-pre2-1/include/pwhg_par.h	2013-10-03 16:31:12.000000000 +0200
@@ -2,7 +2,7 @@
       real * 8 par_csicut
       parameter (par_csicut=1)
       integer par_maxseeds,par_maxxgriditerations
-      parameter (par_maxseeds=200,par_maxxgriditerations=10)
+      parameter (par_maxseeds=100000,par_maxxgriditerations=10)
       
       real * 8 par_diexp,par_dijexp,par_2gsupp,
      1         par_fsrtinycsi,par_fsrtinyy,
EOF

patch -p0 < powheg-box.patch

cd POWHEG-BOX/${process}

mv Makefile Makefile.orig
cat Makefile.orig | sed -e "s#STATIC[ \t]*=[ \t]*-static#STATIC=-dynamic#g" | sed -e "s#PDF[ \t]*=[ \t]*native#PDF=lhapdf#g"> Makefile
echo "LIBS+=-lz -lstdc++" >> Makefile


LHA_BASE="`readlink -f "$LHAPATH/../../../"`"

#slc5_amd64_gcc462/external/lhapdf/5.8.5 has a bug. if this version is used, replace it by 5.8.5-cms:
if [ `basename $LHA_BASE` == "5.8.5" ]
then  
LHA_BASE="`echo "$LHA_BASE" | sed 's@slc5_amd64_gcc462/external/lhapdf/5.8.5@slc5_amd64_gcc462/external/lhapdf/5.8.5-cms@'`"
fi

LHA_BASE_OLD="`$LHA_BASE/bin/lhapdf-config --prefix`"
cat > lhapdf-config-wrap <<EOF
#!/bin/bash
"$LHA_BASE/bin/lhapdf-config" "\$@" | sed "s|$LHA_BASE_OLD|$LHA_BASE|g"
EOF
chmod a+x lhapdf-config-wrap

make LHAPDF_CONFIG="`pwd`/lhapdf-config-wrap" pwhg_main
mkdir workdir
cd workdir
cat ${card} | sed -e "s#SEED#${seed}#g" | sed -e "s#NEVENTS#${nevt}#g" > base_powheg.input
#cat powheg.input
#../pwhg_main &> log_${process}_${seed}.txt

#touch log_${process}_${seed}.txt
seq 1 ${seed} > pwgseeds.dat

for i in {1..4}
do

   date
   sed 's#parallelstage 1#parallelstage '${i}'#g' base_powheg.input  > powheg.input
   cat powheg.input
   
   echo ${rnum} | ../pwhg_main 

   rm -f powheg.input

done

mv pwgevents*lhe pwgevents.lhe

#remove the spurious random seed output that is non LHE standard 
cat pwgevents.lhe | grep -v "Random number generator exit values" > ${file}_final.lhe
ls -l ${file}_final.lhe
pwd
cp ${file}_final.lhe ${WORKDIR}/.
#cp ${file}_final.lhe ${WORKDIR}/${file}_final.lhe
#cp ${file}_final.lhe ${WORKDIR}/output.lhe

echo "Output ready with log_${process}_${seed}.txt and ${file}_final.lhe at `pwd` and $WORKDIR"
echo "End of job on " `date`
exit 0;
