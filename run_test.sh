#!/bin/bash
BASEDIR=$(dirname "$0")
TIMEOUT=5
JOBS=6

"$BASEDIR/scripts/run_pyco.sh" --test-run --config "$BASEDIR/jobs/tacas-24-automata-inclusion.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" inputs/bench-double-automata-inclusion.input
"$BASEDIR/scripts/run_pyco.sh" --test-run --config "$BASEDIR/jobs/tacas-24-bool-comb-cox-diff.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" inputs/bench-double-bool-comb-cox.input
"$BASEDIR/scripts/run_pyco.sh" --test-run --config "$BASEDIR/jobs/tacas-24-bool-comb-cox-inter.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" inputs/bench-double-bool-comb-cox.input
"$BASEDIR/scripts/run_pyco.sh" --test-run --config "$BASEDIR/jobs/tacas-24-bool-comb-intersect.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" inputs/bench-variadic-bool-comb-intersect.input
"$BASEDIR/scripts/run_pyco.sh" --test-run --config "$BASEDIR/jobs/tacas-24-email-filter.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" inputs/bench-quintuple-email-filter.input
"$BASEDIR/scripts/run_pyco.sh" --test-run --config "$BASEDIR/jobs/tacas-24-z3-noodler.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" inputs/bench-single-z3-noodler.input
"$BASEDIR/scripts/run_pyco.sh" --test-run --config "$BASEDIR/jobs/tacas-24-presburger-complement.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" inputs/bench-single-presburger-complement.input
