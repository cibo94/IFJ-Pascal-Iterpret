#!/bin/bash
EIP_SIZE=32
STACK_SIZE=32
EIP_STEP=32
STACK_STEP=64
TICKS=0
COUNTER=0

MIN_TICKS=99999999999999
BEST_EIP=0
BEST_STACK=0

STEPS=20
CYCLES=10

PERCENT=0
FIRST=0
POR=0

function logBest {
    if [[ ${FIRST} -eq 1 ]]; then
       for ((c=0; c<62; c++)); do
           printf '\b'
       done 
    fi
    printf " \e[1;32m#%03d\e[m: \e[1;34mTICKS\e[m: %5d \e[1;34mSTACK_SIZE\e[m: %5d \e[1;34mEIP_SIZE\e[m: %5d" ${POR} ${TICKS} ${STACK_SIZE} ${EIP_SIZE}
    echo ""
}

function log {
    if [[ ${FIRST} -eq 1 ]]; then
       for ((b=0; b<62; b++)); do
           printf '\b'
       done 
    fi
    FIRST=1
    printf " \e[1;31m%3d%%\e[m: \e[1;34mTICKS\e[m: %5d \e[1;34mSTACK_SIZE\e[m: %5d \e[1;34mEIP_SIZE\e[m: %5d" ${PERCENT} ${TICKS} ${STACK_SIZE} ${EIP_SIZE}
}

for ((i=0; i<${STEPS}; i++)); do
    for ((j=0; j<${STEPS}; j++)); do
        make --always-make DEFINES=-DEIP_SIZE=${EIP_SIZE}\ -DBASE_STACK_SIZE=${STACK_SIZE} >/dev/null
        TICKS=0
        COUNTER=0
        for ((k=0; k<${CYCLES}; k++)); do
            COUNTER=$((${COUNTER}+1))
            TICKS=$(($(./proj program.pas)+${TICKS}))
        done
        TICKS=$((${TICKS}/${COUNTER}))
        if [[ ${TICKS} -lt ${MIN_TICKS} ]]; then
            POR=$((${POR}+1))
            MIN_TICKS=${TICKS}
            BEST_EIP=${EIP_SIZE}
            BEST_STACK=${STACK_SIZE}
            logBest
        else
            PERCENT=$(((100*((${i}*${STEPS})+${j}))/(${STEPS}*${STEPS})))
            log
        fi
        EIP_SIZE=$((${EIP_SIZE}+${EIP_STEP}))
    done
    EIP_SIZE=32
    STACK_SIZE=$((${STACK_SIZE}+${STACK_STEP}))
done
echo ""
echo "BEST: TICKS: ${MIN_TICKS} EIP_SIZE: ${BEST_EIP} STACK_SIZE: ${BEST_STACK}"
