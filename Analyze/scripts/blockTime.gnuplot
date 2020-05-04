set datafile separator ','

set key autotitle columnhead
set grid


set style line 1 linetype 1 lw 2
set style line 2 linetype 1 lw 2 linecolor rgb 'green'
set style line 3 linetype 1 lw 2 linecolor rgb 'blue'
set style line 4 linetype 4 lw 2 linecolor rgb 'red'
set style line 5 linetype 3 lw 2 linecolor rgb 'blue'

set xdata time # tells gnuplot the x axis is time data
set timefmt "%H:%M"
set format x "%H:%M"
set xtics rotate # rotate labels on the x axis
set xlabel "Time of Day"

# plot the data in one graph
set term qt 1
set key outside
set key right top
set title 'Blocked Heating'
set ytics ("no-data" -0.1, "not-blocked" 0, "blocked" 1)
set yrange [-0.2:1.1]
#plot for [col = 2:*] '../../HeatingData/BlockingTimeSummary.csv' using 1:col

set term pngcairo size 1000, 1100
set output '../../HeatingData/BlockingTimeSummary.png'
plot for [col = 2:*] '../../HeatingData/BlockingTimeSummary.csv' using 1:col
