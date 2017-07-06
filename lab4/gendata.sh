#! /bin/bash

while [ true ]; do
  n=$RANDOM
  let "n = 1 + $n % 10"
  echo "$1:$n"
  sleep $n
done 
