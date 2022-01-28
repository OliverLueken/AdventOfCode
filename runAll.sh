#!/bin/bash

for year in 2015 2016 2020 2021
do
    for day in $(seq -f "%02g" 1 25)
    do
        echo "Running year $year day $day:"
        cd $year/Day$day
        /usr/bin/time ./tasks.out
        cd ../..
        echo ""
    done
    echo ""
done