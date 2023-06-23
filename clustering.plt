clear
reset
set key on
set border 3
set logscale xy

set xlabel "degree, k"
set ylabel "Clustering"
set key top left

#set xrange [1:100]

plot 'clustering.txt' using 1:2 notitle w p lt rgb "dark-magenta" lw 2