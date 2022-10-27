#!/bin/bash

cd ..

rm -f main
make main

type=1        # 0 = sequential , 1 = native thread , 2 = fastflow
perc=0.1      # perc of frame number 
time=1        # 0 = not show timing , 1 = show timing
overhead=0    # 0 = not , 1 = yes

file_log="./script/executions_result.txt"

echo -e "\n--------\ncompute time of each function\n-----------\n"| tee -a $file_log

for (( nw = 1; nw <= 1; nw++ ))      ### Outer for loop ###
do
    ./main 0 $nw $perc $time 1;

done | tee -a $file_log



echo -e "\n--------\noverhead native thread execution\n-----------\n"| tee -a $file_log

for (( nw = 1; nw <= 32; nw++ ))      ### Outer for loop ###
do
    ./main 1 $nw $perc $time 1;

done | grep Overhead: | tee -a $file_log

echo -e "\n--------\noverhead fastflow execution\n-----------\n"| tee -a $file_log

for (( nw = 1; nw <= 32; nw++ ))      ### Outer for loop ###
do
    ./main 2 $nw $perc $time 1;

done | grep Time: | tee -a $file_log



echo -e "\n--------\nsequential execution\n-----------\n"| tee -a $file_log

for (( nw = 1; nw <= 1; nw++ ))      ### Outer for loop ###
do
    ./main 0 $nw $perc $time $overhead;

done | grep nw: | tee -a $file_log

echo -e "\n--------\nnative thread execution\n-----------\n"| tee -a $file_log

for (( nw = 1; nw <= 32; nw++ ))      ### Outer for loop ###
do
    ./main 1 $nw $perc $time $overhead;

done | grep nw: | tee -a $file_log

echo -e "\n--------\nfastflow execution\n-----------\n"| tee -a $file_log

for (( nw = 1; nw <= 32; nw++ ))      ### Outer for loop ###
do
    ./main 2 $nw $perc $time $overhead;

done | grep nw: | tee -a $file_log
