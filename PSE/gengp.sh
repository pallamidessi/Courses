#! /bin/sh

sleep 0.1

trap 'PID.dat PID.dat2 info.0 gnuplot.spl tmp.000 label.txt >/dev/null; echo "marche";exit ' HUP INT TERM QUIT
ValminmPax(){
sed s/' '/'\n'/g PID.dat > PID.dat2 
echo e >> PID.dat2
nbrVAL=`wc -l PID.dat2 | cut -f 1 -d " "`
min=`sed -n "1 p" PID.dat2|  tr  " " "\n"`
max=`sed -n "1 p" PID.dat2|  tr  " " "\n"`
val=0

for i in `seq 1 $nbrVAL`
	do
		val=`sed -n "$i p" PID.dat2`
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
titre=`sed -n "2 p" info.0`
temps=`sed -n "3 p" info.0`
Min=`Valminmax min`
Max=`Valminmax max`
Min=`expr $Min - 50`
Max=`expr $Max + 50`
echo "
reset
set ylabel 'memoire en KB'
set xlabel 'Temps par $temps secondes'
set terminal x11
set yrange [$Min:$Max] 
set xrange [0:`wc -l PID.dat2 |  cut -f 1 -d " "`] 


plot \"PID.dat2\"  using 1 title '$titre' with linespoint 
 
" > gnuplot.spl
}


GnuSuivrePID(){

while [ 1 -eq 1 ]
	do
	sleep $1
  Creergnuscript 
	
	cat gnuplot.spl gnuplot.spl
done | gnuplot -persist

}
Max(){
nbrVAL=`wc -l info.0 | cut -f 1 -d " "`

max=`sed -n "2 p" info.0 | tr -s " " | cut -f 2 -d' '`
val=0

for i in `seq 2  $nbrVAL`
	do
		if [ $val = "e" ]
			then 
				break
		else
			val=`sed -n "$i p" info.0 | cut -f 2 -d' '`
		
			if [ $val -ge $max ]
				then max=$val
			fi
		fi
	done
	echo "$max"
}
GnuCpu(){

sl=`sed -n "2 p" info.0 `
sl=`expr $sl + 3`
sleep $sl
nbrC=`wc -l info.0 | cut -f 1 -d' '`
nbrC=` expr $nbrC - 2 `
titre=`sed -n "1 p" info.0`
sed '1d' info.0 > tmp.000
cat tmp.000 > info.0
sed '1d' info.0 > tmp.000
cat tmp.000 > info.0

if [ -e label.txt ]
	then rm label.txt
fi

for i in `seq 1 $nbrC`
			do 
			val=`expr $i - 1 `
			echo "set label \"`sed -n "$i p" info.0 | tr -s " " | cut -f 1 -d' '`\" at  $val , 97" >> label.txt
done 


echo "
reset
set ylabel 'pourcentage Cpu'
set terminal x11
set yrange [0:100] 
set xrange [0:$nbrC] 
set style fill solid border -1
`cat label.txt`  
plot \"info.0\"  using 2 title '$titre' with boxes
 
" > gnuplot.spl

cat gnuplot.spl | gnuplot -p
}

GnuPlusGros(){
titre="les cinq plus gros programmes en memoire"

Max=`Max max`
Max=`expr $Max + 5000`
echo "
reset
set ylabel 'memoire en KB'
set terminal x11
set yrange [0:$Max] 
set xrange [0:6] 
set label \"`sed -n "2 p" info.0 | tr -s " " | cut -f 1 -d' '| tr : ' '`\" at 0.7,15000
set label \"`sed -n "3 p" info.0 | tr -s " " | cut -f 1 -d' '| tr : ' '`\" at 1.7,15000
set label \"`sed -n "4 p" info.0 | tr -s " " | cut -f 1 -d' '| tr : ' '`\" at 2.7,15000
set label \"`sed -n "5 p" info.0 | tr -s " " | cut -f 1 -d' '| tr : ' '`\" at 3.7,15000
set label \"`sed -n "6 p" info.0 | tr -s " " | cut -f 1 -d' '| tr : ' '`\" at 4.7,15000
plot \"info.0\"  using 2 title '$titre' with boxes

" > gnuplot.spl

cat gnuplot.spl gnuplot.spl | gnuplot -p

}
if [ -e info.0 ]
	then
		if [ "`sed -n "1 p" info.0`" = "5plusgros" ]
			then 
				GnuPlusGros
		else 
			if [ "`sed -n "1 p" info.0`" = "suivre" ]
				then 
					GnuSuivrePID 5
					exit 0 
			else
				if [ "`sed -n "1 p" info.0`" = "cpu" ] 
					then 
						GnuCpu
			fi fi fi
	else
		echo "USAGE: rediriger ./sysdata vers $0"
		exit 1
fi
rm PID.dat PID.dat2 info.0 gnuplot.spl tmp.000 label.txt >/dev/null
