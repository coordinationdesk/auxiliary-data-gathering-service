#!/bin/bash

oldpwd=$1

IFS='/' read -r -a array <<< "$oldpwd"
numchar=${#array[@]}
if [[ $numchar -eq 1 ]]
then
	echo $oldpwd
else
	newpwd=""
	for ((i=0;i<$numchar;i++)); 
	do 
		newpwd+=${array[$i]}
		if [[ $i -lt $numchar-1 ]]
		then
			newpwd+="\/"
		else
			if [ ${oldpwd:${#oldpwd}-1:1} == '/' ]
			then
				newpwd+="\/"
			fi
		fi
	done
	echo $newpwd
fi
exit 0 
