#!/bin/gnuplot
#(c) Francesco M. Moneta

#options
set datafile separator ";"
set xlabel "x(meters)"
set ylabel "y(meters)"
set terminal pngcairo


#input and output files
INFILE=ARG1.".slog"
OUTFILE=ARG1."2d.png"
set output OUTFILE


plot INFILE using 5:6 with points pt 2 ps 1
