reset
unset key
unset tics 
unset border 
set size ratio 1
set pointsize 0.4
set term pdf enhanced 
set out 'build/pontos.pdf'
plot 'build/input2.txt' with points pt 7
