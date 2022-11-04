#!/bin/bash
function getpwd(){
    local string="qwertyuiopasdfghjklzxcvbnm!@#$%^&*QWERTYUIOPASDFGHJKLZXCVBNM1234567890"
    for i in {1..9}
    do
    num=$[RANDOM%70]
    x=${string:num:1}
    password=${password}$x
    done
    result=$password
}

useradd -d /home/test test
mkdir /home/test
getpwd

echo test:$result | chpasswd