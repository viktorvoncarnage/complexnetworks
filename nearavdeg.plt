clear
reset
set key on
set border 3
set logscale xy

set xlabel "degree, k"
set ylabel "Average nearest neighbors degree"
set key top left

#set xrange [1:100]

plot 'nearavdeg.txt' using 1:2 notitle w l lt rgb "dark-magenta" lw 2, 'nearavdeg.txt' using 1:2 notitle w p lt rgb "orchid4" lw 2