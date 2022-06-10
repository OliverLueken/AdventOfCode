#!/bin/bash

for path in $(find -type d -path "./20*/Day*" | sort)
do
    cd $path
    if test -f ./tasks.out; then
        if [[ ( $path = './2015/Day04' ) || ( $path = './2016/Day05' ) || ( $path = './2016/Day14' ) ]]; then #excluding long running tasks
            echo ""
        else
            echo "Running $path:"
            /usr/bin/time ./tasks.out
            echo ""
        fi
    fi
    cd ../..
done