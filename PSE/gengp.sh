#! /bin/sh
#titre=`grep title gnuplot.spl` \n
#terminal=`grep terminal gnuplot.spl `\n
#ylabel=`grep ylabel gnuplot.spl`\n
#xlabel=`grep xlabel gnuplot.spl`\n


while [ 1 -eq 1 ]
	do
	cat PID.dat |  tr -s " " "\n"
	Creer_gnuscript
done 

Valminmax(){
nrbrVAL=`wc -l PID.dat | cut -f 1 -d " "`
min=`sed -n "1 p" PID.dat`
max=0
val=0

for i in `seq 2 $nbrVAL`
	do
		val=`sed -n "$i p" PID.data`
		if [ $val -le $min ]
			then min=$val
		fi
		
		if [ $val -ge $max ]
			then max=$val
		fi
	done

if [ $1 = "min" ]
	then 
		echo "$min"
else 
	if [ $1 = "max"]
		then echo "$max"
fi fi
}


Creer_gnuscript(){

echo "
set terminal $1
set title $titre

set xlabel `Valminmax min `
set ylabel `Valminmax max `

plot PID.dat using 1 with lines
" > gnuplot.spl
}
