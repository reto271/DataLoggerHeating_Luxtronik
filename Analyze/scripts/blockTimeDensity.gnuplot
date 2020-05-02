set datafile separator ','

set key autotitle columnhead
set grid

#set style line 1 linecolor rgb 'blue' linetype 2 pointtype 2
set style line 1 lc rgb '#0060ad' lt 1 lw 1 pt 6 pi 1 ps 1.5

#lt 7 lw 1 pi 1 ps 1.0

#set style line 1 linetype
#pointtype 2 linewidth 1 linecolor rgb 'blue'
#set linetype   5 lc rgb "blue"        lw 2 pt 8

#set style line 1 linetype 1 lw 1
#set style line 2 linetype 1 lw 1 linecolor rgb 'green'
#
#set style line 4 linetype 4 lw 1 linecolor rgb 'red'
#set style line 5 linetype 3 lw 1 linecolor rgb 'blue'

set xdata time # tells gnuplot the x axis is time data
set timefmt "%H:%M"
set format x "%H:%M"
set xtics rotate # rotate labels on the x axis
set xlabel "Time of Day"


# density plot
set term qt 0
set key inside
set key left center
set title 'Probablity of Blocked Heating'
set ylabel 'Probalbility in %'
set yrange [-10:110]
#plot  '../../HeatingData/BlockingTimeCumSum.csv'  using 1:2

set term pngcairo size 1600,1200
#set terminal postscript eps color colortext
set output '../../HeatingData/BlockingTimeCumSum.png'
plot  '../../HeatingData/BlockingTimeCumSum.csv'  using 1:2 with linespoints ls 1
