#!/bin/bash
rm -rf ./results/data/smoke-test || true
./run_all.sh --test-run --jobs 1 --b-smt --output-dir smoke-test --test-run-count 2 --timeout 60
