#! /bin/bash

echo 'Wat is je naam?'
read NAAM
echo 'Wat is je verjaardag? (DD/MM)'
read BIRTHDAY
if [ $BIRTHDAY = "14/10" ] 
then
	echo "Gelukkige verjaardag ${NAAM}"
else
	echo "Hoi ${NAAM}"
fi 
