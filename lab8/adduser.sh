#!/bin/bash

# 获取随机密码
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

# 按行读取文本
cat userlist.dat | while read line
do
    # 分割字符串 
    name=`echo $line | awk -F ' ' '{print $1}'`
    tt=`echo $line | awk -F ' ' '{print $2}'`
    useradd -d /home/$name $name
    # 创建主目录
    mkdir /home/$name
    getpwd
    echo $name:$result | chpasswd
    chown $name  /home/$name
    if [ $tt = "manage" ] 
    then
        chmod 644 /home/$name
    elif [ $tt = "user" ]
    then
        mkdir /home/$name/data
        chmod 444 -R /home/$name/data
        mkdir /home/$name/src
        chmod 555 /home/$name/src
    elif [ $tt = "visit" ]
    then
        chmod 444 -R /home/$name
    fi
done