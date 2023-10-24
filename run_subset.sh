#!/bin/bash
#
echo "[!] Running subset of results (could take about 4 hours)"
read -p "Press <any> key to starts subset of experiments"
rm -rf ./results/data/partial-experiments || true
./run_all.sh --jobs 4 --timeout 60 --armc-incl --lia-explicit --lia-symbolic --output-dir partial-experiments
