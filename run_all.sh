#!/bin/bash
BASEDIR=$(dirname "$0")

"$BASEDIR/scripts/run_pyco.sh" --config "$BASEDIR/jobs/tacas-24-automata-inclusion.yaml" --timeout 60 --jobs 6 inputs/bench-double-automata-inclusion.input
