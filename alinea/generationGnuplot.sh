#! /bin/sh

dataGraphe(){
>data.txt

val=0
for i in `seq 3 $2`
	do 
		/usr/bin/time -f "%e" -a -o data.txt ./bin/main -s -$1 -t $i >> data.txt
	done

taille=`wc -l data.txt | cut -f 1 -d" "`
taille=`expr $taille + 3`

for i in `seq 3 $taille`
	do
		val="$i "
		sed -i "$i s/^/$val/" data.txt
	done
}

	
CreerScriptGnuplot(){
	

	echo "
	set terminal png size 800,400
	set output '$1 '
	set ylabel 'temps d execution en secondes'
	set xlabel 'dimension NxN'

	plot \"data.txt\"  title '$1' with linespoint 
	" > gnuplot.spl

}

dataGraphe "r" "11"
CreerScriptGnuplot "determinant calcule de maniere recursive.png" 
cat gnuplot.spl | gnuplot

dataGraphe "p" "500"
CreerScriptGnuplot "determinant calcule avec le pivot de gauss.png" 
cat gnuplot.spl | gnuplot

dataGraphe "i" "500"
CreerScriptGnuplot "inverse calculee avec le pivot de gauss.png" 
cat gnuplot.spl | gnuplot

dataGraphe "c" "11"
CreerScriptGnuplot "inverse calculee avec la comatrice (recursive).png" 
cat gnuplot.spl | gnuplot

dataGraphe "o" "50"
CreerScriptGnuplot "inverse calculee avec la comatrice optimisee.png" 
cat gnuplot.spl | gnuplot
