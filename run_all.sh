#!/bin/bash
BASEDIR=$(dirname "$0")
TIMEOUT=60
JOBS=6
SUFFIX="--suffix _first-experiments"

"$BASEDIR/scripts/run_pyco.sh" --config "$BASEDIR/jobs/tacas-24-automata-inclusion.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" $SUFFIX inputs/bench-double-automata-inclusion.input
"$BASEDIR/scripts/run_pyco.sh" --config "$BASEDIR/jobs/tacas-24-bool-comb-diff.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" $SUFFIX inputs/bench-double-bool-comb-cox.input
"$BASEDIR/scripts/run_pyco.sh" --config "$BASEDIR/jobs/tacas-24-bool-comb-cox-inter.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" $SUFFIX inputs/bench-double-bool-comb-cox.input
"$BASEDIR/scripts/run_pyco.sh" --config "$BASEDIR/jobs/tacas-24-email-filter.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" $SUFFIX inputs/bench-quintuple-email-filter.input
"$BASEDIR/scripts/run_pyco.sh" --config "$BASEDIR/jobs/tacas-24-bool-comb-intersect.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" $SUFFIX inputs/bench-variadic-bool-comb-intersect.input
