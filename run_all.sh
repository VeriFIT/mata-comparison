#!/bin/bash
BASEDIR=$(dirname "$0")
TIMEOUT=60
JOBS=6
SUFFIX="--suffix .forty-jalib-automata-again"
METHODS=""
EXCLUDE=""

usage() { {
        [ $# -gt 0 ] && echo "error: $1"
        echo "usage: ./run_all.sh [opt1, ..., optn]"
        echo "options:"
        echo "  -m|--methods                will measure only selected tools"
        echo "  -e|--exclude                will not measure selected tools"
        echo "  -j|--jobs                   will run only number of jobs"
    } >&2
}

while [ $# -gt 0 ]; do
    case "$1" in
        -h|--help)
            usage
            exit 0;;
        -m|--methods)
            METHODS="-m $2"
            shift 2;;
        -e|--exclude)
            EXCLUDE="-e $2"
            shift 2;;
        -j|--jobs)
            JOBS=$2
            shift 2;;
    esac
done

start_time=$SECONDS

## Inclusion comparison
# "$BASEDIR/scripts/run_pyco.sh" $EXCLUDE $METHODS --config "$BASEDIR/jobs/tmp-automata-inclusion.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" $SUFFIX inputs/bench-double-automata-inclusion.input
# "$BASEDIR/scripts/run_pyco.sh" $EXCLUDE $METHODS --config "$BASEDIR/jobs/tmp-email-filter.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" $SUFFIX inputs/bench-quintuple-email-filter.input


"$BASEDIR/scripts/run_pyco.sh" $EXCLUDE $METHODS --config "$BASEDIR/jobs/tacas-24-automata-inclusion.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" $SUFFIX inputs/bench-double-automata-inclusion.input
# "$BASEDIR/scripts/run_pyco.sh" $EXCLUDE $METHODS --config "$BASEDIR/jobs/tacas-24-email-filter.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" $SUFFIX inputs/bench-quintuple-email-filter.input
"$BASEDIR/scripts/run_pyco.sh" $EXCLUDE $METHODS --config "$BASEDIR/jobs/tacas-24-z3-noodler.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" $SUFFIX inputs/bench-single-z3-noodler.input
"$BASEDIR/scripts/run_pyco.sh" $EXCLUDE $METHODS --config "$BASEDIR/jobs/tacas-24-z3-noodler.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" $SUFFIX inputs/bench-single-z3-noodler-big.input
"$BASEDIR/scripts/run_pyco.sh" $EXCLUDE $METHODS --config "$BASEDIR/jobs/tacas-24-z3-noodler-concat.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" $SUFFIX inputs/bench-single-z3-noodler.input
"$BASEDIR/scripts/run_pyco.sh" $EXCLUDE $METHODS --config "$BASEDIR/jobs/tacas-24-presburger-complement.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" $SUFFIX inputs/bench-single-presburger-complement.input
"$BASEDIR/scripts/run_pyco.sh" $EXCLUDE $METHODS --config "$BASEDIR/jobs/tacas-24-presburger-complement.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" $SUFFIX inputs/bench-single-presburger-explicit-complement.input
"$BASEDIR/scripts/run_pyco.sh" $EXCLUDE $METHODS --config "$BASEDIR/jobs/tacas-24-bool-comb-ere.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" $SUFFIX inputs/bench-variadic-bool-comb-ere.input
"$BASEDIR/scripts/run_pyco.sh" $EXCLUDE $METHODS --config "$BASEDIR/jobs/tacas-24-bool-comb-cox-union.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" $SUFFIX inputs/bench-double-bool-comb-cox.input
"$BASEDIR/scripts/run_pyco.sh" $EXCLUDE $METHODS --config "$BASEDIR/jobs/tacas-24-bool-comb-cox-inter.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" $SUFFIX inputs/bench-double-bool-comb-cox.input
"$BASEDIR/scripts/run_pyco.sh" $EXCLUDE $METHODS --config "$BASEDIR/jobs/tacas-24-bool-comb-intersect-union.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" $SUFFIX inputs/bench-variadic-bool-comb-intersect.input
"$BASEDIR/scripts/run_pyco.sh" $EXCLUDE $METHODS --config "$BASEDIR/jobs/tacas-24-bool-comb-intersect.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" $SUFFIX inputs/bench-variadic-bool-comb-intersect.input
"$BASEDIR/scripts/run_pyco.sh" $EXCLUDE $METHODS --config "$BASEDIR/jobs/tacas-24-bool-comb-intersect.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" $SUFFIX inputs/bench-variadic-z3-noodler-intersect.input

## Not run, everything timeouts
# "$BASEDIR/scripts/run_pyco.sh" $EXCLUDE $METHODS --config "$BASEDIR/jobs/tacas-24-bool-comb-cox-diff.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" $SUFFIX inputs/bench-double-bool-comb-cox.input
secs=$((SECONDS - start_time))
formated_elapsed=$(printf "%dd:%dh:%dm:%ds\n" $((secs/86400)) $((secs%86400/3600)) $((secs%3600/60)) $((secs%60)))
eval "echo [!] All benchmarks done in $formated_elapsed"
