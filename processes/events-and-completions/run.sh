#! /bin/bash

sudo dmesg -c
sudo insmod $1
sudo rmmod $1
dmesg
