#!/bin/bash
#SBATCH --time=24:00:00
# running on owens
#SBATCH --nodes=1 --ntasks=28
#SBATCH --job-name=pc_data_test
# account for CSE 5441 Au'21
#SBATCH --account=PAS2065

echo job started at `date`
echo on compute node `cat $PBS_NODEFILE`

DIR=/fs/ess/PAS2065
EXEC=./lab1/main
mkdir TMP

cp ${DIR}/PCS_data_t00100A ${DIR}/PCS_data_t00100B ${DIR}/PCS_data_t00100C ${DIR}/PCS_data_t00100D ${DIR}/PCS_data_t00100E ${DIR}/PCS_data_t01000 ${DIR}/PCS_data_t05000 ${DIR}/PCS_data_t10000 ${DIR}/PCS_data_t20000 TMP/
# cp ./lab1/test TMP/
echo job started at `date` >>current.out
# time ${EXEC} <TMP/test  >>current.out 2>&1
time ${EXEC} <TMP/PCS_data_t00100A  >>current.out 2>&1
echo at `date` >>current.out 
time ${EXEC} <TMP/PCS_data_t00100B  >>current.out 2>&1
echo at `date` >>current.out 
time ${EXEC} <TMP/PCS_data_t00100C  >>current.out 2>&1
echo at `date` >>current.out 
time ${EXEC} <TMP/PCS_data_t00100D  >>current.out 2>&1
echo at `date` >>current.out 
time ${EXEC} <TMP/PCS_data_t00100E  >>current.out 2>&1
echo at `date` >>current.out 
time ${EXEC} <TMP/PCS_data_t01000  >>current.out 2>&1
echo at `date` >>current.out 
time ${EXEC} <TMP/PCS_data_t05000  >>current.out 2>&1
echo at `date` >>current.out 
time ${EXEC} <TMP/PCS_data_t10000  >>current.out 2>&1
echo at `date` >>current.out 
time ${EXEC} <TMP/PCS_data_t20000  >>current.out 2>&1
echo job ended at `date` >>current.out

export SAVEDIR='./tests/data_test.'${SLURM_JOBID}
mkdir -p ${SAVEDIR}
mv current.out ${SAVEDIR}
rm -rf TMP


