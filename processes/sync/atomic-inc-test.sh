#! /bin/bash

sudo dmesg -c

for i in {1..10}
do
   sudo insmod atomic-inc.ko
   sudo rmmod atomic-inc
done

dmesg
