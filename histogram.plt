clear
reset
set key on
set border 3

set xlabel "degree, k"
set ylabel "P(k)"
set yrange [0:1]
set logscale xy

# Add a vertical dotted line at x=0 to show centre (mean) of distribution.
set yzeroaxis

#histrogram
# Each bar is half the (visual) width of its x-range.
set boxwidth 0.75 absolute
#set style data linespoints
set style fill solid 1.0 noborder

bin_width = 0.8;

bin_number(x) = floor(x/bin_width)

rounded(x) = bin_width * ( bin_number(x) + 0.5 )

#normalized histrogram
stats 'degrees.txt'
sum=351

#plot	

plot 'degrees.txt' using (rounded($2)):(1.0/(bin_width*sum)) notitle smooth frequency with lines lt rgb "dark-magenta" lw 2