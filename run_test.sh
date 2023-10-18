#!/bin/bash
BASEDIR=$(dirname "$0")
TIMEOUT=5
JOBS=6
METHODS=""

usage() { {
        [ $# -gt 0 ] && echo "error: $1"
        echo "usage: ./run_test.sh [opt1, ..., optn]"
        echo "options:"
        echo "  -m|--methods                will measure only selected tools"
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
    esac
done

start_time=$SECONDS

# "$BASEDIR/scripts/run_pyco.sh" $METHODS --test-run --config "$BASEDIR/jobs/tacas-24-bool-comb-ere.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" inputs/bench-variadic-bool-comb-ere.input
# read -p "Press enter to continue"
# "$BASEDIR/scripts/run_pyco.sh" $METHODS --test-run --config "$BASEDIR/jobs/tacas-24-automata-inclusion.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" inputs/bench-double-automata-inclusion.input
# read -p "Press enter to continue"
# "$BASEDIR/scripts/run_pyco.sh" $METHODS --test-run --config "$BASEDIR/jobs/tacas-24-bool-comb-cox-diff.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" inputs/bench-double-bool-comb-cox.input
# read -p "Press enter to continue"
# "$BASEDIR/scripts/run_pyco.sh" $METHODS --test-run --config "$BASEDIR/jobs/tacas-24-bool-comb-cox-inter.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" inputs/bench-double-bool-comb-cox.input
# read -p "Press enter to continue"
# "$BASEDIR/scripts/run_pyco.sh" $METHODS --test-run --config "$BASEDIR/jobs/tacas-24-bool-comb-cox-union.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" inputs/bench-double-bool-comb-cox.input
# read -p "Press enter to continue"
# "$BASEDIR/scripts/run_pyco.sh" $METHODS --test-run --config "$BASEDIR/jobs/tacas-24-email-filter.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" inputs/bench-quintuple-email-filter.input
# read -p "Press enter to continue"
# "$BASEDIR/scripts/run_pyco.sh" $METHODS --test-run --config "$BASEDIR/jobs/tacas-24-z3-noodler.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" inputs/bench-single-z3-noodler.input
# read -p "Press enter to continue"
# "$BASEDIR/scripts/run_pyco.sh" $METHODS --test-run --config "$BASEDIR/jobs/tacas-24-z3-noodler.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" inputs/bench-single-z3-noodler-big.input
# read -p "Press enter to continue"
# "$BASEDIR/scripts/run_pyco.sh" $METHODS --test-run --config "$BASEDIR/jobs/tacas-24-z3-noodler-concat.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" inputs/bench-single-z3-noodler.input
# read -p "Press enter to continue"
# "$BASEDIR/scripts/run_pyco.sh" $METHODS --test-run --config "$BASEDIR/jobs/tacas-24-presburger-complement.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" inputs/bench-single-presburger-complement.input
# read -p "Press enter to continue"
# "$BASEDIR/scripts/run_pyco.sh" $METHODS --test-run --config "$BASEDIR/jobs/tacas-24-presburger-complement.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" inputs/bench-single-presburger-explicit-complement.input
# read -p "Press enter to continue"
# "$BASEDIR/scripts/run_pyco.sh" $METHODS --test-run --config "$BASEDIR/jobs/tacas-24-bool-comb-intersect-union.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" inputs/bench-variadic-bool-comb-intersect.input
# read -p "Press enter to continue"
# "$BASEDIR/scripts/run_pyco.sh" $METHODS --test-run --config "$BASEDIR/jobs/tacas-24-bool-comb-intersect.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" inputs/bench-variadic-bool-comb-intersect.input
# read -p "Press enter to continue"
"$BASEDIR/scripts/run_pyco.sh" $METHODS --test-run --config "$BASEDIR/jobs/tacas-24-bool-comb-intersect.yaml" --timeout "$TIMEOUT" --jobs "$JOBS" inputs/bench-variadic-z3-noodler-intersect.input

secs=$((SECONDS - start_time))
formated_elapsed=$(printf "%dd:%dh:%dm:%ds\n" $((secs/86400)) $((secs%86400/3600)) $((secs%3600/60)) $((secs%60)))
eval "echo [!] Test benchmarking done in $formated_elapsed"
