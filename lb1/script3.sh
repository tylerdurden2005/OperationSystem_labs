#!/bin/bash

file_path="$(pwd)/$1"
ls -lR /home | grep "^l.*-> $file_path"

