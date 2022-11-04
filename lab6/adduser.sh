#!/bin/bash
all_str=(0 1 2 3 4 5 6 7 8 9 q w e r t y u i o p l k j h g f d s a z x c v b n m Q W E R T Y U I O P L K J H G F D S A Z X C V B N M \! \@ \# \$ \% \^ \& \* )
sum=''
upper=(Q W E R T Y U I O P L K J H G F D S A Z X C V B N M)
super=(\! \@ \# \$ \% \^ \& \* )

#读文件
while read line
do
  USER_NAME=$line
  # 密码两位特殊字符
  for i in {1..2}
  do
    each_num=$[RANDOM%8]
    passw=${super[each_num]}
    sum=${sum}${passw}
  done
  # 密码一位大写
  each_num=$[RANDOM%26]
  passw=${upper[each_num]}
  sum=${sum}${passw}
  # 密码随机五位
  for i in {1..5}
  do
    each_num=$[RANDOM%70]
    passw=${all_str[each_num]}
    sum=${sum}${passw}
  done
  PASSWORD=$sum
  sum=''
  # 创建用户
  useradd -m ${USER_NAME}
  echo ${USER_NAME}:${PASSWORD}|chpasswd

  # 首次登录修改密码
  passwd -e ${USER_NAME}

  done < userlist.data


