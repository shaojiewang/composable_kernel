#!/bin/sh
# TODO: run this script from CK root
BUILD=build
EXE=$BUILD/bin/example_fmha_fwd
VALID=0

for hidden in 5120 2048 ; do
for seq in 256 512 1024 2048; do
for bs in 1 2 4 8 16 32; do
for prec in "bf16" ; do
for perm in 0 ; do
for hdim in 128 ; do
for tmask in 1 ; do

nhead=$(($hidden / $hdim))     # follow fav2 setup
$EXE -prec=$prec -b=$bs -h=$nhead -d=$hdim -s=$seq   -iperm=$perm -operm=$perm -mask=$tmask -v=$VALID ; sleep 3

done
done
done
done
done
done
done
