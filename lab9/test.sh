#!/bin/bash
cp ./static/index ./static/index.html
top -b -n 1| sed '1,7d' > temp  # 去除开头没用的内容

n=0
top -o %CPU -b -n 1 | sed -n  '8,10p' |
while read line
do
    let n+=1
    process=`echo $line | awk -F ' ' '{print $12}'`
    num=`echo $line | awk -F ' ' '{print $9}'`
    sed -i "${n}s/$/ ${process}/" temp
    sed -i "${n}s/$/ ${num}/" temp
done

n=0
top -o %MEM -b -n 1 | sed -n  '8,10p' |
while read line
do
    let n+=1
    process=`echo $line | awk -F ' ' '{print $12}'`
    num=`echo $line | awk -F ' ' '{print $10}'`
    sed -i "${n}s/$/ ${process}/" temp
    sed -i "${n}s/$/ ${num}/" temp
done

# 拼接html
awk '{
    if(NR<=3){
        if($2!="root")
        {
            printf("<li><div>%s %s <button class=\"btn\">关闭</button> </div><div>%s %s%</div><div>%s %s%</div></li>",$1,$12,$13,$14,$15,$16);
        }
        else{
            printf("<li><div>%s %s </div><div>%s %s%</div><div>%s %s%</div></li>",$1,$12,$13,$14,$15,$16);
        }
    }
    if(NR>4){
        if($2!="root")
        {
            printf("<li><div>%s %s <button class=\"btn\">关闭</button></div></li>",$1,$12);
        }
        else{
            printf("<li><div>%s %s </div></li>",$1,$12);
        }
    }
}' temp >> ./static/index.html

echo "</div></body>" >> ./static/index.html