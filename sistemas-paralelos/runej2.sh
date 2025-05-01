#!/bin/sh

echo '----------------------------------------------------\nAlgoritmo secuencial';
./mmblk $1 $2;
echo '----------------------------------------------------\nAlgoritmo pthreads 2';
./mmblkthreads $1 $2 2;
echo '----------------------------------------------------\nAlgoritmo pthreads 4';
./mmblkthreads $1 $2 4;
echo '----------------------------------------------------\nAlgoritmo pthreads 8';
./mmblkthreads $1 $2 8;
