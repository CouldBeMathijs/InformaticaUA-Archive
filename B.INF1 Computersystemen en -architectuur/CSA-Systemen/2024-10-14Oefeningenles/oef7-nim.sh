#! /bin/bash

echo "Met hoeveel lucifers wil je beginnen?"
GAMERUNNING=1
BEURT=1
KEERGESPEELD=1
read AANTALLUCIFERS
echo ""
echo "Kies wie begint, en wissel vanaf dan altijd af."
until [ $AANTALLUCIFERS -eq "0" ]; do
	echo "Wil je 1, 2 of 3 lucifers afnemen?"
	read AFTETREKKEN
	case $AFTETREKKEN
	in
		1|2|3)
			AANTALLUCIFERS=$((AANTALLUCIFERS-AFTETREKKEN))
			echo "Er zijn nog er ${AANTALLUCIFERS} over."
			((KEERGESPEELD++))
		;;
		*)
			echo "\"${AFTETREKKEN}\" is een foutieve waarde."
	esac
	echo ""
done
echo "Je hebt gewonnen!"
echo "Er zijn ${KEERGESPEELD} keer lucifers weggenomen."
	
	
