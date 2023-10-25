#!/bin/bash
rm -rf ./results/data/smoke-test || true
./run_all.sh --test-run --jobs 1 --b-smt --timeout 60 --output-dir smoke-test --test-run-count 2
