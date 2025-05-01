#!/bin/sh

echo '----------------------------------------------------\nAlgoritmo secuencial';
./vectoradd $1;
echo '----------------------------------------------------\nAlgoritmo pthreads 2';
./threadvectoradd $1 2;
echo '----------------------------------------------------\nAlgoritmo pthreads 4';
./threadvectoradd $1 4;
echo '----------------------------------------------------\nAlgoritmo pthreads 8';
./threadvectoradd $1 8;
