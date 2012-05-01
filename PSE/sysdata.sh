#! /bin/sh


# rajouter des expand a toutes les commandes avec des cut pour evitr des probleme debile
# debugger ce bousin
# reste la troisieme question theoriquement

#trap "rm test1.txt taillememoire.txt taillememoiretrie.txt test2; exit" SIGHUP SIGTERM SIGINT

#Affiche les 5 plus gros consommateur memoire avec leurs nom. ou leurs PID si pas de nom

INTERVALLE=5

CinqPlusGros() 
{
rm test1.txt taillememoire.txt taillememoiretrie.txt 
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

grep  VmSize test1.txt |expand |  tr -s " " | cut -f 2 -d " ">> taillememoire.txt

sort -g -r taillememoire.txt > taillememoiretrie.txt

	VAL1=`sed -n "1 p" taillememoiretrie.txt`
	VAL2=`sed -n "2 p" taillememoiretrie.txt`
	VAL3=`sed -n "3 p" taillememoiretrie.txt`
	VAL4=`sed -n "4 p" taillememoiretrie.txt`
	VAL5=`sed -n "5 p" taillememoiretrie.txt`




PID1=`grep $VAL1 test1.txt |expand | tr -s " " | cut -f 3 -d "/"`
NOMVAL1=`grep $PID1 test1.txt | expand | grep Name |tr -s ' '  | cut -f 2 -d' '`
PID2=`grep $VAL2 test1.txt |expand |  tr -s " " | cut -f 3 -d "/"`
NOMVAL2=`grep $PID2 test1.txt | grep Name |expand | tr -s ' '  | cut -f 2 -d' ' `
PID3=`grep $VAL3 test1.txt |expand |  tr -s " " | cut -f 3 -d "/"`
NOMVAL3=`grep $PID3 test1.txt | grep Name |expand | tr -s ' '  | cut -f 2 -d' '`
PID4=`grep $VAL4 test1.txt |expand |  tr -s " " | cut -f 3 -d "/"`
NOMVAL4=`grep $PID4 test1.txt | grep Name |expand | tr -s ' '  | cut -f 2 -d' '`
PID5=`grep $VAL5 test1.txt |expand |  tr -s " " | cut -f 3 -d "/"`
NOMVAL5=`grep $PID5 test1.txt | grep Name |expand | tr -s ' '  | cut -f 2 -d' '`



if [ "$NOMVAL1" =  "0" ]
	then 
		echo " [$PID1]: $VAL1 "
	else 
		echo " $NOMVAL1: $VAL1 "
fi		
if [ "$NOMVAL2" =  "0" ]
	then 
		echo " [$PID2]: $VAL2 "
	else 
		echo " $NOMVAL2: $VAL2 "
fi
if [ "$NOMVAL3" =  "0" ]
	then 
		echo " [$PID3]: $VAL3 "
	else 
		echo " $NOMVAL3 $VAL3 "
fi
if [ "$NOMVAL4" =  "0" ]
	then 
		echo " [$PID4]: $VAL4 "
	else 
		echo " $NOMVAL4: $VAL4 "
fi
if [ "$NOMVAL5" =  "0" ]
	then 
		echo " [$PID5]: $VAL5 "
	else 
		echo " $NOMVAL5: $VAL5 "
fi
}

#affiche et monitore un PID precis s'il existe
SuivrePID() 
{
PID="$1"
NAME=0
TMP=0



VALTMP="`grep VmSize /proc/$PID/status | expand | tr -s ' '| cut -f 2 -d ' ' `"
NAME=`grep Name /proc/$PID/status | expand | tr -s ' ' | cut -f 2 -d ' '`

	if [ "$NAME" = "0" ]
		then 
			NAME=$PID
			echo "PID:$NAME"
		else
			echo "NOM:$NAME"
	fi		


echo "\033[32m instant taille en kiloctet \033[0m "

if [ -d /proc/$PID  ]
	then
		while [ 1 -eq 1 ]
			do
				echo "T$TMP 		$VALTMP"
				VALTMP="$VALTMP `grep VmSize /proc/$PID/status|expand|tr -s ' '|cut -f 2 -d' '`" 
				echo $VALTMP > PID.dat
				TMP=`expr $TMP + 1`
				sleep $2
			done 
	else echo "Usage: PID inexistant"		
fi
}


AfficheCPU()
{
echo "Veuillez patientez $1 secondes ..."
rm util.cpu cpu.1 cpu.2
grep cpu /proc/stat	> cpu.1
sleep $1
grep cpu /proc/stat	> cpu.2
nbrp=`wc -l cpu.1|cut -f 1 -d' '`

for i in `seq 1 $nbrp`
	do

VAL1=`sed -n "$i p" cpu.1 |expand |tr -s " "|cut -f 2 -d' '`
VAL2=`sed -n "$i p" cpu.2 |expand |tr -s " "|cut -f 2 -d' '`
VAL3=`sed -n "$i p" cpu.1 |expand |tr -s " "|cut -f 3 -d' '`
VAL4=`sed -n "$i p" cpu.2 |expand |tr -s " "|cut -f 3 -d' '`
VAL5=`sed -n "$i p" cpu.1 |expand |tr -s " "|cut -f 4 -d' '`
VAL6=`sed -n "$i p" cpu.2 |expand |tr -s " "|cut -f 4 -d' '`
VAL7=`sed -n "$i p" cpu.1 |expand |tr -s " "|cut -f 5 -d' '`
VAL8=`sed -n "$i p" cpu.2 |expand |tr -s " "|cut -f 5 -d' '`


EXP1=`expr $VAL2 - $VAL1  `  
EXP2=`expr $VAL4 - $VAL3  `  
EXP3=`expr $VAL6 - $VAL5  `  
EXP4=`expr $VAL8 - $VAL7  `  

EXP5=`expr $EXP1 + $EXP2`
EXP6=`expr $EXP3 + $EXP4`

EXP7=`expr $EXP5 + $EXP6`

EXP8=`expr $EXP5 + $EXP3`

echo "$EXP7 $EXP8" >>util.cpu

	done

  TotalCPU=`sed -n "1  p" util.cpu |cut -f 1 -d' '`
	TotalUtil=`sed -n "1  p" util.cpu |cut -f 2 -d' '`
	pourcentageCPU=`expr \( $TotalUtil \* 200 \) / $TotalCPU `
	
	
	echo "cpus $pourcentageCPU" 
	
	for i in `seq 2 $nbrp`
		do
		  TotalCPU=`sed -n "$i  p" util.cpu |cut -f 1 -d' '`
			TotalUtil=`sed -n "$i  p" util.cpu |cut -f 2 -d' '`
			pourcentageCPU=`expr  \( $TotalUtil \* 200 \) / $TotalCPU` 
			CPUid=`sed -n "$i  p" /proc/stat |cut -f 1 -d' '`
			
			echo "$CPUid `expr $pourcentageCPU  `"
			
		done 
		
	}

AfficheHelp(){

echo "NAME \n"
echo "\t sysdata - Recuperation de certaine information memoire et processeur\n"		
echo "SYNOPSIS\n"
echo "\t sysdata [OPTION] ...\n"
echo "DESCRIPTION\n"
echo "\t Permet de suivre la consommation memoire a intervalle regulier defini \n \t ou non par l'utilisateur (defaut 5 secondes) d'un processus particulier en donnant son PID.\n \t Donne la liste des 5 plus gros consommateurs en memoire au lancement du script. \n \t Donne l'utilisation processeur global et par unites (cores) en se basant dans un intervalle defini (defaut 5 secondes)\n"
echo "\t -i"
echo "\t\t definir un intervalle de temps en seconde pour les mesures, a utiliser en premier.\n"
echo "\t -c"
echo "\t\t Information utilisation processeur global et detaille par cores.\n"
echo "\t -m"
echo "\t\t Affiche les 5 plus gros processus consommateurs de memoire en kb.\n"
echo "\t -h"
echo "\t\t Aide.\n"
echo "\t -p "
echo "\t\t Suivre la consommation memoire d'un PID donne\n"
}
#while getops i: s
	#do 
		#case "$s" in
			#i) INTERVALLE=$OPTARG ;;
			#[?]) break ;; 
  #esac
#done 

while getopts i:cmhp: o
	do	
		case "$o" in
			i)	INTERVALLE=$OPTARG ;;
			c)	AfficheCPU $INTERVALLE ;;
			m)	CinqPlusGros ;;
			h)  AfficheHelp ;;
			p)  SuivrePID $OPTARG $INTERVALLE ;;
			[?])	echo "Usage: $0 [OPTION] ..."
		exit 1;;
	esac
done
