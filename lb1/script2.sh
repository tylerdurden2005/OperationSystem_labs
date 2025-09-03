#!/bin/bash

inode=$(ls -i $1 | awk '{print $1}')
ls -ilR /home 2>/dev/null | grep $inode
