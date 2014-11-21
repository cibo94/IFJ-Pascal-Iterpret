#!/bin/bash
# Exportovane globalne premenne
export dir
export name
export file
export src
export input
export output
export retCode
export CC=gcc

# Globalne premenne
CCErr=0
OUTErr=0
RETErr=0
VALErr=0

# Posunutie do testovej zlozky
cd tests

# Krasne vypisy
function log {
    msg=$1
    if [ -t 1 ]; then
    ncolors=$(tput colors)
    if test -n "$ncolors" && test $ncolors -ge 8; then
    cols=$(tput cols)
        color=$3
        bcol="\e[39m"
    else
        cols=50
    fi
    else 
    cols=50
    fi
    msgLen=${#msg}
    shft=$(($cols-$msgLen-10))
    printf "  $msg"
    for ((i = 0; i < $shft; i++)); do
        printf " "
    done
    printf "[${color}%s${bcol}]\n" $2
}

# WRAPP functions:
function __wrapp_exe {
    ${CC} -DDEBUG -g -std=c99 -Wall -Wextra -pedantic -o ${dir}/${name}.exe -x c - ${file} ../src/init.c 2>${dir}/${CC}.err 1>${dir}/${CC}.out <<EOF
${src}
EOF
}
function __wrapp_test {
    printf "%s" "${output}" > ${dir}/${name}.tstout
}
function __wrapp_input {
    printf "%s" "${input}" > ${dir}/${name}.tstin
}

# 3 big functions ...
function __INIT__ {
    mkdir ${dir} 2>/dev/null
    __wrapp_test
    __wrapp_input
    __wrapp_exe
}

# zmazanie prebytocnych adresarov
function __FREE__ {
    rm -rf ${dir}
}

# Testy
function __TEST__ {
    echo ""
    ./${dir}/${name}.exe ${dir}/${name}.tstin >${dir}/${name}.out 2>${dir}/${name}.err
    ret=$?
    diff ${dir}/${name}.out ${dir}/${name}.tstout --suppress-common-lines > ${dir}/${name}.diff
    valgrind -q ${dir}/${name}.exe ${dir}/${name}.tstin 2>${dir}/${name}.val >/dev/null
    dff=$(cat ${dir}/${name}.diff)
    if [[ "$dff" == "" ]]; then
        log "Test: $name stdout" "OK" "\e[32m"
    else
        OUTErr=1
        log "Test: $name stdout" "FAIL" "\e[31m"
    fi
    if [[ "$(cat ${dir}/${CC}.err)" == "" ]]; then
        log "   ${CC} error" "NONE" "\e[32m"
    else
        CCErr=1
        log "   ${CC} error" "ERR" "\e[31m"
    fi
    if [[ "$(cat ${dir}/${name}.val)" == "" ]]; then
        log "   Valgrind" "OK" "\e[32m"
    else
        VALErr=1
        log "   Valgrind" "ERR" "\e[31m"
    fi
    if [[ "$(cat ${dir}/${name}.err)" == "" ]]; then
        log "   Error output" "NONE" "\e[32m"
    else
        log "   Error output" "ERR" "\e[31m"
    fi
    if [[ "$ret" == "$retCode" ]]; then
        log "   Return Code: $ret" "OK" "\e[32m"
    else
        RETErr=1
        log "   Return Code: $ret Should be  $retCode" "BAD" "\e[31m"
    fi
    if [[ "$arg" != "-m" ]]; then
        echo "
        $dff"
    fi
    if [[ "$arg" == "-o" ]]; then
        echo "
        ${CC} ERR:"
        cat ${dir}/${CC}.err
        echo "
    ${name} stdout:"
        cat ${dir}/${name}.out
        echo "
    ${name} stderr:"
        cat ${dir}/${name}.err
    fi
}

# main function
function TEST {
    __INIT__
    __TEST__
    if [[ "$s" != "-s" ]]; then
        __FREE__
    fi
}
function __HELP__ {
    if [[ "$arg" == "-h" ]]; then
        echo "
./test [-s] [-m|-o] [-h]
    -h  -> show this help
    -m  -> minimal output
    -o  -> normal output + output of programs
    -s  -> save test outputs
    "
        exit
    fi
}
function __SUM__ {
    echo "
                SUMMARY
    "
    if [[ CCErr -ne 0 ]]; then
        log "   ${CC} error" "ERR" "\e[31m"
    else
        log "   ${CC} error" "OK" "\e[32m"
    fi
    if [[ OUTErr -ne 0 ]]; then
        log "   Output tests" "FAIL" "\e[31m"
    else
        log "   Output tests" "PASS" "\e[32m"
    fi
    if [[ RETErr -ne 0 ]]; then
        log "   Return code tests" "FAIL" "\e[31m"
    else
        log "   Return code tests" "PASS" "\e[32m"
    fi
    if [[ VALErr -ne 0 ]]; then
        log "   Valgrind tests" "FAIL" "\e[31m"
    else
        log "   Valgrind tests" "PASS" "\e[32m"
    fi
}
function main {
    __HELP__
    for tstDir in TEST_*; do
        echo "
                $tstDir
        "
        for tst in ${tstDir}/test_*; do
            # Vymazanie premmenych
            export dir=""
            export name=""
            export file=""
            export src=""
            export input=""
            export output=""
            export retCode=0
            . ./${tst}
            export dir="${tstDir}/${dir}"
            TEST
        done
        echo ""
    done
    __SUM__
}

arg="$1"
if [[ "$2" == "" ]]; then
    s="$1"
else
    s="$2"
fi

main
cd ..