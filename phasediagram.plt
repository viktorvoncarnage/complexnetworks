clear
reset
set key on
set border 3
#set logscale xy

set xlabel "{/Symbol=l}"
set ylabel "stationary prevalence, {/Symbol=r}"
set key bottom
set yrange [0:1]
#set xrange [0:0.5]

plot 'pst.txt' u 1:2 w l lw 2 notitle
