#!/bin/bash
i=5

NUMNODES=(25 50 100)
for i in "${NUMNODES[@]}";
do
    # 1M
    echo ${i} 1.1 24ms 1M
    python test.py 1.1 1025 5 5 ${i} 24ms small.txt || echo dead at ${i} 1.0 6ms 1M
    mv *txt t1_24ms_11_1M/${i}

    echo ${i} 1.1 6ms 1M
    python test.py 1.1 1025 5 5 ${i} 6ms small.txt || echo dead at ${i} 1.0 6ms 1M
    mv *txt t1_6ms_11_1M/${i}

    echo ${i} 1.0 24ms 1M
    python test.py 1.0 1025 5 5 ${i} 24ms small.txt || echo dead at ${i} 1.0 24ms 1M
    mv *txt t1_24ms_10_1M/${i}

    echo ${i} 1.0 6ms 1M
    python test.py 1.0 1025 5 5 ${i} 6ms small.txt || echo dead at ${i} 1.0 6ms 1M
    mv *txt t1_6ms_10_1M/${i}

    # index.html
    
    echo ${i} 1.0 24ms in
    python test.py 1.0 1025 5 5 ${i} 24ms || echo dead at ${i} 1.0 24ms in
    mv *txt t1_24ms_10_in/${i}

    echo ${i} 1.0 6ms in
    python test.py 1.0 1025 5 5 ${i} 6ms || echo dead at ${i} 1.0 6ms in
    mv *txt t1_6ms_10_in/${i}

    echo ${i} 1.1 6ms in 
    python test.py 1.1 1025 5 5 ${i} 6ms || echo dead at ${i} 1.1 6ms in
    mv *txt t1_6ms_11_in/${i}

    echo ${i} 1.1 24ms in
    python test.py 1.1 1025 5 5 ${i} 24ms || echo dead at ${i} 1.1 24ms in
    mv *txt t1_24ms_11_in/${i}

    # colors.jpg
    
    echo ${i} 1.0 24ms col
    python test.py 1.0 1025 5 5 ${i} 24ms colors.jpg || echo dead at ${i} 1.0 24ms col
    mv *txt t1_24ms_10_col/${i}

    echo ${i} 1.0 6ms col
    python test.py 1.0 1025 5 5 ${i} 6ms colors.jpg || echo dead at ${i} 1.0 6ms col
    mv *txt t1_6ms_10_col/${i}

    echo ${i} 1.1 6ms col
    python test.py 1.1 1025 5 5 ${i} 6ms colors.jpg || echo dead at ${i} 1.1 6ms col
    mv *txt t1_6ms_11_col/${i}

    echo ${i} 1.1 24ms col
    python test.py 1.1 1025 5 5 ${i} 24ms colors.jpg || echo dead at ${i} 1.1 24ms col
    mv *txt t1_24ms_11_col/${i}

done
