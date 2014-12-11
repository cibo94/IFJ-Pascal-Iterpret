#!/bin/bash 

FILES=./emb_tests/*.pas

function SEPP {
    cols=$(tput cols)
    echo ""
    for ((i=0; i<${cols}; i++)); do 
        printf '='
    done
}

function main {
    make --always-make TARGET=proj >/dev/null 2>/dev/null
    SEPP
    for FILE in ${FILES}; do
        OUT=$(./proj ${FILE} 2>/dev/null)
        ERR=$(./proj ${FILE} 2>&1 >/dev/null)
        if [ "${ERR}" != "" ]; then
            echo "${ERR}"
        else
            if [ "${OUT}" != "" ]; then
                echo "${OUT}"
            else
                echo "No output and no error"
            fi
        fi
        SEPP
    done
}

main;
