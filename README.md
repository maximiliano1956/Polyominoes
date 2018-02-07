# polyominoes

This is a program that i wrote long ago.

polymin try to fill a shape with a set of polyominoes.

The definition of the set of pieces and the fields to fill are in the polymin.dat file; the format is self-explanatory.

The solutions are saved in the solution folder and you can choose among three possibile format : ASCII,XPM,JPEG.

The pixmap folder contains xpm files that are used to view the polyminoes

It's possibile to activate a check to find some simmetry in the solutions.

It's also possibile to save a snapshot of the search and then reload it to continue later.

The program run on Linux. To compile run "make"

To compile correctly you have to install the libraries:


lesstif
xpm
xmu
jpeg


It's possibile to crosscompilng under windows with cygwin with
X-windows installed.

The program is not optimized at all and the search runs very slow.

(Especially if you choose to view the search in continous view mode)
