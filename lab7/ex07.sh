#!/bin/sh

# 取端口去重
awk '!a[$5]++{print "### 2021-9-"$2" "$3" " $5 }' auth.log | awk -F '[' '{print $1" ["$2}' > port.log

# 取后面字段处理
awk -F '[' '{print "["$2}' auth.log > tmp.log

count=$(awk 'END{print NR}' port.log)
for i in $(seq 1 ${count})
do
  awk 'NR=="'$i'"{$4="";print}' port.log >> /usr/local/log/error.md
  awk 'NR=="'$i'"{print "#### "$4}' port.log >> /usr/local/log/error.md
  Port=$(awk 'NR=="'$i'" {print $5}' port.log)
  # tmp.log 按端口分类
  awk '$1=="'$Port'"{print}' tmp.log > data.log
  # 以: 为分隔符,判断分割数目
  awk -F ": " '{
    if (NF-1 == 3){
        printf("**%s:** %s: %s\n",$2,$3,$4)
    }
	else if(NF-1 == 2){
        printf("**%s:** %s\n",$2,$3)
    }

   }' data.log >> /usr/local/log/error.md
   echo "\n" >> /usr/local/log/error.md
  awk -F ": " '{
    if (NF-1==1){
        printf("%s\n",$2)
    }
    }' data.log >> /usr/local/log/error.md
  echo "\n" >> /usr/local/log/error.md


done
rm data.log tmp.log port.log