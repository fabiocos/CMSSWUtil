#!/bin/bash

#set -o verbose

set -x

EXPECTED_ARGS=5

if [ $# -ne $EXPECTED_ARGS ]
then
  echo "Usage: `basename $0` repository process card Nevents RandomSeed "
  echo "process names are: Dijet Zj WW hvq WZ  W_ew-BW Wbb Wj VBF_Hgg_H W Z  Wp_Wp_J_J VBF_Wp_Wp ZZ"  
  echo "Example: ./create_lhe_powheg.sh $CMSSW_BASE/work Z slc5_ia32_gcc434/powheg/V1.0/8TeV_Summer12/DYToEE_M-20_8TeV-powheg/v1/DYToEE_M-20_8TeV-powheg.input 1000 1212" 
  exit 1
fi

echo "Beginnig of job on" `date`

echo "   ______________________________________     "
echo "         Running Powheg                       "
echo "   ______________________________________     "

repo=${1}
echo "%MSG-POWHEG repository = $repo"

process=${2}
echo "%MSG-POWHEG process = $process"

cardinput=${3}
echo "%MSG-POWHEG location of the card = $cardinput"

nevt=${4}
echo "%MSG-POWHEG number of events requested = $nevt"

rnum=${5}
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

dat1=`echo ${cardinput} | sed 's/-powheg.input/-pwggrid.dat/' `
dat2=`echo ${cardinput} | sed 's/-powheg.input/-pwgxgrid.dat/' ` 
dat3=`echo ${cardinput} | sed 's/-powheg.input/-pwgubound.dat/' ` 

cp ${repo}/${process}.exe .

mkdir workdir
cd workdir

if [ -e ${repo}/${dat1} ]; then cp ${repo}/${dat1} pwggrid.dat ; fi
if [ -e ${repo}/${dat2} ]; then cp ${repo}/${dat2} pwgxgrid.dat ; fi
if [ -e ${repo}/${dat3} ]; then cp ${repo}/${dat3} pwgubound.dat ; fi

cat ${card} | sed -e "s#SEED#${seed}#g" | sed -e "s#NEVENTS#${nevt}#g" > base_powheg.input
mv base_powheg.input powheg.input
cat powheg.input
time ../${process}.exe &> log_${process}_${seed}.txt

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


