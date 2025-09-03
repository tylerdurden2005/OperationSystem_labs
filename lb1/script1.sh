#!/bin/bash
> $1
list=(- d b c p s)
for i in ${list[@]};
do
str=$(ls -lR / 2>/dev/null | grep "^$i" -m 1)
name=$(echo "$str" | awk '{print $NF}')
link=$(find / 2>/dev/null -name $name -printf '%p\n' -quit)
echo "$str" | awk -v val="$link" '{ $NF = val; print }' >> "$1"
done
#отдельно обрабатываем символьную ссылку
str=$(ls -lR / 2>/dev/null | grep "^l" -m 1)
name=$(echo "$str" | awk '{print $9}')
link=$(find / 2>/dev/null -type l -name $name -printf '%p\n' -quit)
echo "$str" | awk -v val="$link" '{ $NF = val; print }' >> "$1"
