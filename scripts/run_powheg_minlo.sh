#!/bin/bash

#set -o verbose

set -x

EXPECTED_ARGS=4

if [ $# -ne $EXPECTED_ARGS ]
then
  echo "Usage: `basename $0` process card Nevents RandomSeed "
  echo "process names are: Dijet Zj WW hvq WZ  W_ew-BW Wbb Wj VBF_Hgg_H W Z  Wp_Wp_J_J VBF_Wp_Wp ZZ"  
  echo "Example: ./create_lhe_powheg.sh Z slc5_ia32_gcc434/powheg/V1.0/8TeV_Summer12/DYToEE_M-20_8TeV-powheg/v1/DYToEE_M-20_8TeV-powheg.input 1000 1212" 
  exit 1
fi

echo "   ______________________________________     "
echo "         Running Powheg                       "
echo "   ______________________________________     "

process=${1}
echo "%MSG-POWHEG process = $process"

cardinput=${2}
echo "%MSG-POWHEG location of the card = $cardinput"

nevt=${3}
echo "%MSG-POWHEG number of events requested = $nevt"

rnum=${4}
echo "%MSG-POWHEG random seed used for the run = $rnum"


seed=$rnum
file="events"
# Release to be used to define the environment and the compiler needed
export PRODHOME=`pwd`
export SCRAM_ARCH=slc5_amd64_gcc462
export RELEASE=${CMSSW_VERSION}
export WORKDIR=`pwd`

# Get the input card
#wget --no-check-certificate http://cms-project-generators.web.cern.ch/cms-project-generators/${cardinput} -O powheg.input
cp ${PRODHOME}/${cardinput} powheg.input
card="$WORKDIR/powheg.input"

mkdir workdir
cd workdir
cat ${card} | sed -e "s#SEED#${seed}#g" | sed -e "s#NEVENTS#${nevt}#g" > base_powheg.input
#cat powheg.input
#../pwhg_main &> log_${process}_${seed}.txt

#touch log_${process}_${seed}.txt
echo ${seed} > pwgseeds.dat

for i in {1..4}
do

   date
   sed 's#parallelstage 1#parallelstage '${i}'#g' base_powheg.input  > powheg.input
   cat powheg.input
   
   echo 1 | ../${process}.exe 

   rm -f powheg.input

done

ls -lrt

mv pwgevents-*.lhe pwgevents.lhe

#reweighting info calculation
sed 's#parallelstage 1#parallelstage 4#g' base_powheg.input | sed 's#\#compute_rwgt#compute_rwgt#g' > prod_powheg.input

# renormalization and factorization scale

for i in 2 0.5
do

   date
   sed 's#facscfact 1#facscfact '${i}'#g' prod_powheg.input | sed 's#renscfact 1#renscfact '${i}'#g' > powheg.input
   cat powheg.input
   
   echo 1 | ../${process}.exe 

   rm -f powheg.input
   mv pwgevents-*.lhe pwgevents.lhe

done 

# PDF variation

for i in 21100 192800
do

   date
   sed 's#lhans1 10800#lhans1 '${i}'#g' prod_powheg.input | sed 's#lhans2 10800#lhans2 '${i}'#g' > powheg.input
   cat powheg.input
   
   echo 1 | ../${process}.exe 

   rm -f powheg.input
   mv pwgevents-*.lhe pwgevents.lhe

done 

#remove the spurious random seed output that is non LHE standard 
cat pwgevents.lhe | grep -v "Random number generator exit values" > ${file}_final.lhe
ls -l ${file}_final.lhe
pwd
cp ${file}_final.lhe ${WORKDIR}/.
#cp ${file}_final.lhe ${WORKDIR}/${file}_final.lhe
#cp ${file}_final.lhe ${WORKDIR}/output.lhe

#echo "Output ready with log_${process}_${seed}.txt and ${file}_final.lhe at `pwd` and $WORKDIR"
echo "End of job on " `date`
exit 0;
