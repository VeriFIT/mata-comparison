#!/bin/bash
binary=$1
shift
program=$1
shift
automata="$@"

java -jar $binary $program $(python3 ~/scripts/massage_arguments.py $automata)
