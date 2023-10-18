#!/bin/bash
binary=$1
shift
program=$1
shift
automata="$@"

$binary $program $(python3 ~/scripts/to_mona_mata.py $automata)
