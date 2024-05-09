#!/bin/sh
# TODO: run this script from CK root
set -x

BUILD=build
EXE=$BUILD/bin/example_fmha_fwd
VALID=0

for hidden in 64 ; do
for seq in 922; do
for bs in 300; do
for prec in "bf16" ; do
for perm in 0 ; do
for hdim in 32 ; do
for tmask in 0 ; do

nhead=$(($hidden / $hdim))     # follow fav2 setup
$EXE -prec=$prec -b=$bs -h=$nhead -d=$hdim -s=$seq   -iperm=$perm -operm=$perm -mask=$tmask -v=$VALID ; sleep 3

done
done
done
done
done
done
done
