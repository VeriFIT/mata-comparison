#!/bin/bash
binary=$1
shift
program=$1
shift
automata="$@"

BASEDIR=$(dirname "$0")

$binary $program $(python3 $BASEDIR/massage_arguments.py $automata)
