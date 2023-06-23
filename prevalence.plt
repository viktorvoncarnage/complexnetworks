clear
reset
set key on
set border 3
#set logscale xy

set xlabel "time, t"
set ylabel "density, {/Symbol=r}"
set key bottom
set yrange [0:1]

plot 'density.txt' u 1:2 w l lw 2 t '{/Symbol=l}=0.3'
