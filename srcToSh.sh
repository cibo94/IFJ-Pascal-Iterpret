#!/bin/bash
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



src=$1
shift
cntr=$(echo "${@}" | wc --words)
c=0
for i in "$@"; do
    fname=$(echo "${i}" | sed "s/\.[^\.]*\$//g" | sed "s/.*\///g")
    o="$(cat ./output/${fname}.out 2>/dev/null)"
    in="$(cat ${i} 2>/dev/null)"
    ret=0
    cat > ./tests/TEST_PROJECT/test_${fname}.sh <<EOF
#!/usr/bin/env bash
export dir="${fname}"
export name="${fname}"
export file="../src/LEX_lexem.c"
export src='$(cat ${src} | sed "s/#include\s*\"/#include\ \"..\/src\//g")'
export input="
${in}
"
export output="${o}"
export retCode=${ret}
EOF
    c=$(($c+1))
    log "Processing: '${i}' to './tests/TEST_PROJECT/test_${fname}.sh'" "$(($c*100/$cntr))%" "\e[32m" 
    shift
done
