#! /bin/sh


# rajouter des expand a toutes les commandes avec des cut pour evitr des probleme debile
# debugger ce bousin
# reste la troisieme question theoriquement

trap ''rm test1.txt taillememoire.txt taillememoiretrie.txt test2' SIGHUP SIGTERM SIGINT'

#Affiche les 5 plus gros consommateur memoire avec leurs nom. ou leurs PID si pas de nom
function CinqPlusGros () {
rm test1.txt taillememoire.txt taillememoiretrie.txt test2
VAL1=0
VAL2=0
VAL3=0
VAL4=0
VAL5=0
NOMVAL1=0
NOMVAL2=0
NOMVAL3=0
NOMVAL4=0
NOMVAL5=0


grep "Name\|VmSize" /proc/[0-9]*/status >> test1.txt

grep  VmSize test1.txt | tr -s " " | cut -f 2 -d " ">> taillememoire.txt

sort -g -r taillememoire.txt > taillememoiretrie.txt

	VAL1=`sed -n "1 p" taillememoiretrie.txt`
	VAL2=`sed -n "2 p" taillememoiretrie.txt`
	VAL3=`sed -n "3 p" taillememoiretrie.txt`
	VAL4=`sed -n "4 p" taillememoiretrie.txt`
	VAL5=`sed -n "5 p" taillememoiretrie.txt`




PID1=`grep $VAL1 test1.txt | tr -s " " | cut -f 3 -d "/"`
NOMVAL1=`grep $PID1 test1.txt | grep Name |tr -s ' '  | cut -f 3 -d :`
PID2=`grep $VAL2 test1.txt | tr -s " " | cut -f 3 -d "/"`
NOMVAL2=`grep $PID2 test1.txt | grep Name |tr -s ' '  | cut -f 3 -d :`
PID3=`grep $VAL3 test1.txt | tr -s " " | cut -f 3 -d "/"`
NOMVAL3=`grep $PID3 test1.txt | grep Name |tr -s ' '  | cut -f 3 -d :`
PID4=`grep $VAL4 test1.txt | tr -s " " | cut -f 3 -d "/"`
NOMVAL4=`grep $PID4 test1.txt | grep Name |tr -s ' '  | cut -f 3 -d :`
PID5=`grep $VAL5 test1.txt | tr -s " " | cut -f 3 -d "/"`
NOMVAL5=`grep $PID5 test1.txt | grep Name |tr -s ' '  | cut -f 3 -d :`

NOMVAL2=0
NOMVAL3=0
NOMVAL4=0
NOMVAL5=0

if [ "$NOMVAL1" =  "0" ]
	then 
		echo " $PID1 $VAL1 "
	else 
		echo " $NOMVAL1 $VAL1 "
fi		
if [ "$NOMVAL2" =  "0" ]
	then 
		echo " $PID2 $VAL2 "
	else 
		echo " $NOMVAL2 $VAL2 "
fi
if [ "$NOMVAL3" =  "0" ]
	then 
		echo " $PID3 $VAL3 "
	else 
		echo " $NOMVAL3 $VAL3 "
fi
if [ "$NOMVAL4" =  "0" ]
	then 
		echo " $PID4 $VAL4 "
	else 
		echo " $NOMVAL4 $VAL4 "
fi
if [ "$NOMVAL5" =  "0" ]
	then 
		echo " $PID5 $VAL5 "
	else 
		echo " $NOMVAL5 $VAL5 "
fi
}

#affiche et monitore un PID precis s'il existe

function SuivrePID () {
PID=0
NAME=0
TMP=0

read $PID

VALTMP="`grep VmSize /proc/$PID/status | tr -s ' '| cut -f 2 -d ' ' `"
NAME=`grep Name /proc/$PID/status | tr -s ' ' | cut -f 2 -d ' '`

	if [ "$NAME" = "0" ]
		then 
			NAME=$PID
			echo "PID:$NAME"
		else
			echo "NOM:$NAME"
	fi		


echo "instant      taille en octet"

if [ /proc/$PID -d ]
	then
		while [ -r test1.txt ]
			do
				slip 5
				echo "$TMP $VALTMP"
				VALTMP="$VALTMP `grep VmSize /proc/$PID/status`"
				TMP=`expr $TMP + 1`
			done 
	else echo "Usage: PID inexistant"		
fi
}


