#! /bin/sh

sleep 0.1
NOMSCRIPT="gnuplot.spl"
trap 'rm /tmp/PID.dat /tmp/PID.dat2 /tmp/info.0  /tmp/tmp.000 /tmp/label.txt 2>/dev/null;mv gnuplot.spl $NOMSCRIPT 2>/dev/null;exit ' HUP INT TERM QUIT
Valminmax(){
sed s/' '/'\n'/g /tmp/PID.dat > /tmp/PID.dat2 
echo e >> /tmp/PID.dat2
nbrVAL=`wc -l /tmp/PID.dat2 | cut -f 1 -d " "`
min=`sed -n "1 p" /tmp/PID.dat2|  tr  " " "\n"`
max=`sed -n "1 p" /tmp/PID.dat2|  tr  " " "\n"`
val=0

for i in `seq 1 $nbrVAL`
	do
		val=`sed -n "$i p" /tmp/PID.dat2`
		if [ $val = "e" ]
			then break
		else
			if [ $val -le $min ]
				then min=$val
			fi
		
			if [ $val -ge $max ]
				then max=$val
			fi
		fi
	done

if [ $1 = "min" ]
	then 
		echo "$min"
fi
	if [ $1 = "max" ]
		then echo "$max"
 fi
}



Creergnuscript(){
temps=`sed -n "3 p" /tmp/info.0`
Min=`Valminmax min`
Max=`Valminmax max`
Min=`expr $Min - 50`
Max=`expr $Max + 50`

if [ "$3" = "0" ]
	then 
		xlab="Temps par $temps secondes"
	else 
		xlab="$3"
fi

echo "
reset
set ylabel '$2'
set xlabel '$xlab'
set terminal $4
set yrange [$Min:$Max] 
set xrange [0:`wc -l /tmp/PID.dat2 |  cut -f 1 -d " "`] 


plot \"/tmp/PID.dat2\"  using 1 title '$1' with linespoint 
 
" > gnuplot.spl
}


GnuSuivrePID(){

while [ 1 -eq 1 ]
	do
	sleep $1
  Creergnuscript "$2" "$3" "$4" "$5"
	
	cat gnuplot.spl gnuplot.spl
done | gnuplot -persist

}
Max(){
nbrVAL=`wc -l /tmp/info.0 | cut -f 1 -d " "`

max=`sed -n "2 p" /tmp/info.0 | tr -s " " | cut -f 2 -d' '`
val=0

for i in `seq 2  $nbrVAL`
	do
		if [ $val = "e" ]
			then 
				break
		else
			val=`sed -n "$i p" /tmp/info.0 | cut -f 2 -d' '`
		
			if [ $val -ge $max ]
				then max=$val
			fi
		fi
	done
	echo "$max"
}
GnuCpu(){

sl=`sed -n "2 p" /tmp/info.0 `
sl=`expr $sl + 3`
sleep $sl
nbrC=`wc -l /tmp/info.0 | cut -f 1 -d' '`
nbrC=` expr $nbrC - 2 `
sed '1d' /tmp/info.0 > /tmp/tmp.000
cat /tmp/tmp.000 > /tmp/info.0
sed '1d' /tmp/info.0 > /tmp/tmp.000
cat /tmp/tmp.000 > /tmp/info.0

if [ -e /tmp/label.txt ]
	then rm /tmp/label.txt
fi

for i in `seq 1 $nbrC`
			do 
			val=`expr $i - 1 `
			echo "set label \"`sed -n "$i p" /tmp/info.0 | tr -s " " | cut -f 1 -d' '`\" at  $val , 97" >> /tmp/label.txt
done 


echo "
reset
set ylabel '$2'
set terminal $3
set yrange [0:100] 
set xrange [0:$nbrC] 
set style fill solid border -1
`cat /tmp/label.txt`  
plot \"/tmp/info.0\"  using 2 title '$1' with boxes
 
" > gnuplot.spl

cat gnuplot.spl | gnuplot -p
}

GnuPlusGros(){
titre=

Max=`Max max`
Max=`expr $Max + 5000`
echo "
reset
set ylabel '$2'
set terminal $3
set yrange [0:$Max] 
set xrange [0:6] 
set label \"`sed -n "2 p" /tmp/info.0 | tr -s " " | cut -f 1 -d' '| tr : ' '`\" at 0.7,15000
set label \"`sed -n "3 p" /tmp/info.0 | tr -s " " | cut -f 1 -d' '| tr : ' '`\" at 1.7,15000
set label \"`sed -n "4 p" /tmp/info.0 | tr -s " " | cut -f 1 -d' '| tr : ' '`\" at 2.7,15000
set label \"`sed -n "5 p" /tmp/info.0 | tr -s " " | cut -f 1 -d' '| tr : ' '`\" at 3.7,15000
set label \"`sed -n "6 p" /tmp/info.0 | tr -s " " | cut -f 1 -d' '| tr : ' '`\" at 4.7,15000
plot \"/tmp/info.0\"  using 2 title '$1' with boxes

" > gnuplot.spl

cat gnuplot.spl gnuplot.spl | gnuplot -p

}

AfficheHelp(){
echo "NAME \n"
echo "\t gengp - Affichage avec Gnuplot d'information issue du script sysdata\n"		
echo "SYNOPSIS\n"
echo "\t ./sysdata [OPTION] ... | ./gengp [OPTION]\n"
echo "DESCRIPTION\n"
echo "\t Affiche le suivi de la consommation memoire a intervalle regulier defini \n \t ou non par l'utilisateur (defaut 5 secondes) d'un processus.\n \t Affiche un histogramme des 5 plus gros consommateurs en memoire au lancement du script. \n \t Fait un histogramme de l'utilisation processeur global et par unites (cores) en se basant dans un intervalle defini (defaut 5 secondes)\n"
echo "\t -t"
echo "\t\t Changer le titre du graphique.\n"
echo "\t -x"
echo "\t\t Changer le nom de l'abscisse.\n"
echo "\t -y"
echo "\t\t Changer le nom de l'ordonnee.\n"
echo "\t -h"
echo "\t\t Aide.\n"
echo "\t -o "
echo "\t\t changer le terminal de sortie (wxt,x11,png ...)\n"
echo "\t -f"
echo "\t\t Changer le nom du script gnuplot obtenu (par defaut gnuplot.spl).\n"
echo "\t -i"
echo "\t\t Changer le nom de l'image obtenu du graphique.\n"
}

if [ "`sed -n "1 p" /tmp/info.0`" = "5plusgros" ]
	then 
				TITRE="les cinq plus gros programmes en memoire"
				YLAB="memoire en KB"
			  TERM=x11
else if [ "`sed -n "1 p" /tmp/info.0`" = "suivre" ]
	then 
				TITRE=`sed -n "2 p" /tmp/info.0`
				YLAB="memoire en KB"
				XLAB=0
			  TERM="x11"
else if [ "`sed -n "1 p" /tmp/info.0`" = "cpu" ] 
	then 
				TITRE=`sed -n "1 p" /tmp/info.0 `
				YLAB="pourcentage Cpu"
			  TERM="x11"
fi fi fi 


while getopts t:x:y:o:f:i:h p
	do	
		case "$p" in
			t)	TITRE=$OPTARG ;;
			x)	XLAB=$OPTARG ;;
			y)	YLAB=$OPTARG ;;
			o)  TERM=$OPTARG;;
			f)  NOMSCRIPT=$OPTARG ;;
			i)	NOMIMAGE=$OPTARG;;
			h)	AfficheHelp ;;
			[?])	echo "Usage: $0 [OPTION] ..."
		exit 1;;
	esac
done



if [ -e /tmp/info.0 ]
	then
		if [ "`sed -n "1 p" /tmp/info.0`" = "5plusgros" ]
			then 
						GnuPlusGros "$TITRE" "$YLAB" $TERM
		else 
			if [ "`sed -n "1 p" /tmp/info.0`" = "suivre" ]
				then 
					GnuSuivrePID 5 "$TITRE" "$YLAB" "$XLAB" "$TERM"
					exit 0 
			else
				if [ "`sed -n "1 p" /tmp/info.0`" = "cpu" ] 
					then 
						GnuCpu "$TITRE" "$YLAB" $TERM
			fi fi fi
	else
		echo "USAGE: rediriger ./sysdata vers $0"
		exit 1
fi

mv gnuplot.spl $NOMSCRIPT 2>/dev/null
rm /tmp/PID.dat /tmp/PID.dat2 /tmp/info.0  /tmp/tmp.000 /tmp/label.txt 2>/dev/null
