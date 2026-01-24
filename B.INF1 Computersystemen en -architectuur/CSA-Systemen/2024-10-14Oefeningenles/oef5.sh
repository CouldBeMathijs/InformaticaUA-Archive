#! /bin/bash
for THING in *
do 
	if [ -f $THING ]
	then
		cat "${THING}"
	fi
done
