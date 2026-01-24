#! /bin/bash

if [ -f $1 ]
then
	cat $1
elif [ -d $1 ]
then
	ls $1
else
	echo "Onbehandelbaar file path"
fi
