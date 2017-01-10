#!/bin/bash
#msg=`echo 'www-data' | sudo -ES /var/www/html/cgi-bin/on.cgi >/dev/null 2>&1`
OFF=0
PIN=0
ERROR_MSG_1='Done.'
ERROR_MSG_2='Already in progress.'
TWO_MINS=$((60*2))

fileName='pid'
filePath='/tmp/switch_1/'
file="$filePath$fileName"

printOutput(){
    output="{\"success\":$3, \"exitCode\":\"$1\", \"msg\":\"$2\"}"

    echo $output
}

off(){
    gpio mode $PIN down
    gpio write $PIN $OFF
    echo $?
}

secElapsed(){
    echo expr $(date +%s) - $(date +%s -r $file)
}

turnOff(){
    mkdir -p $filePath
    echo $$ > $file
    ecode=$(off)
    printOutput $ecode $ERROR_MSG_1 true
    rm $file
}

if [ -e $file ]
then
    if [ secElapsed -gt $TWO_MINS ]
    then
        rm $file
        turnOf
    else
        printOutput 1 "$ERROR_MSG_2 $(secElapsed)" false
        exit
    fi
else
    turnOff
fi

exit
