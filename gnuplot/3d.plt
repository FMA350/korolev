#!/bin/gnuplot
#(c) Francesco M. Moneta

#options
set datafile separator ";"
set xlabel "x(meters)"
set ylabel "y(meters)"
set terminal pngcairo


#input and output files
INFILE=ARG1.".slog"
OUTFILE=ARG1."3d.png"
set output OUTFILE


splot INFILE using 5:6:7 with points pt 2 ps 1
