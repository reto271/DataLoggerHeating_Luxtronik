set datafile separator ','

set key autotitle columnhead
set grid




set style line 6 lc rgb "blue"
set boxwidth 0.25
set style fill solid

set style line 1 linetype 1 lw 2
set style line 2 linetype 1 lw 2 linecolor rgb 'green'
set style line 3 linetype 1 lw 2 linecolor rgb 'blue'
set style line 4 linetype 4 lw 2 linecolor rgb 'red'
set style line 5 linetype 3 lw 2 linecolor rgb 'blue'

set xdata time # tells gnuplot the x axis is time data
set timefmt "%Y-%m-%d"
set format x "%Y-%m-%d"
set xtics rotate # rotate labels on the x axis
set xlabel "Day"

# plot the data in one graph
set term qt 1
set key inside
set key left top
set title 'Blocking Time per Day'
set ylabel "Blocking Time in Minutes"
#set ytics ("no-data" -0.1, "not-blocked" 0, "blocked" 1)
#set yrange [-0.2:1.1]
#plot for [col = 2:*] '../../HeatingData/BlockingTimeSummary.csv' using 1:col

set term pngcairo size 2000, 1000
set output '../../HeatingData/BlockingTimePerDay.png'
plot  '../../HeatingData/BlockingTimePerDay.csv'  using 1:2:xtic(1) with boxes ls 3
