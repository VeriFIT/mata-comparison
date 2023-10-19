#!/bin/bash
./git-archive-all.sh
gzip -9 experiments.tar
mv experiments.tar.gz tacas24-artifact.tar.gz
