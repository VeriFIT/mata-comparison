## Virtual Machine for reproducing of results in paper "Reasoning about Regular Properties: A Comparative Study"

Tip: You can run `frogmouth README.md` for more smooth reading of this file.

This virtual machine contains data, tools, benchmark and scripts for
replicating (and processing) tool paper describing `libmata` library, which
was submitted to TACAS'24.
 
  * [Benchmarks](https://github.com/VeriFIT/nfa-bench): source benchmarks that were used in the comparison.

## Overview

This virtual machine is meant to be used for running benchmarks on set of
libraries that supports various types of Nondeterministic Finite Automata.

The available tools are listed in `jobs/*.yaml` (each file corresponds to
a~group of similar experiments); each tool is installed in `tools` directory,
and takes as input automata in various formats (each librarytends to use
different format). We use `pycobench` (a python script) to measure the time of
each library and output a report in `csv` (delimited by `;`) which lists
individual measurements, results and some other metrics (e.g., time of
mintermization or optionaly time of some formula preprocessing) for each
library. For each library we prepared particular source codes, that compiles
to binary executable which models our experiments.

Note, that some tools/libraries are run either as a wrapper script (to simplify
the usage in benchmarking or fine-tuning the measurement) or specific binary
program that either takes an input automaton (and processes it) or might take
an input program that specifies sequence of automata operation (and executes
them in order; e.g., load automaton `A`; load automaton `B`; create
intersection of `A` and `B`).

The virtual machine contains basic packages, tools, as well as some additional
helper scripts. Each helper script is accompanied by its own (sometimes short)
documentation.

For the sake of simplicity we prepared a master script `run_all.sh` which
should (to certain degree) replicate the results presented in TACAS'24.

## Technical Specification

We ran our experiments on Debian GNU/Linux 11, with IntelCore 3.4GHz process, 8
CPU cores, and 20 GB RAM. Such specification should be enough to reproduce our
results.

Increasing the amount of RAM or number of cores might lead to better results.
The virtual machine machine requires at least 35GB of spaces, however, we
recommend users to allocate at least 70GB.

It is recommended to allow network access to this virtual machine, since we
track our benchmarks in remote repository. This way one can pull the latest
version of benchmarks.

## <a id="access"></a> Access Credentials 

The virtual machine contains a single user `experiments` with admin rights, hence,
you can install additional packages, change configurations, etc. The password
for the user is `Auto#Bench`.

> :warning: We strongly advise to either backup the image, or proceed carefully,
> before doing any major changes to the virtual machine. We are not responsible
> for any damage you will done by making major changes to the machine.

## File and directory Structure

The virtual machine has following file and directory structure:

  * `nfa-bench/`: a~directory, which contains our benchmarks. 
    See [VeriFIT/automata-bench](https://github.com/nfa-bench) or `./nfa-bench/README.md` 
    for more information about each benchmark.
  * `results/`: contains a link to repository for storing our results. 
  * `tools/`: directory, which contains installed tools. We refer the reader to `tools/README.md` for
    information about each tool that we have installed in this directory. 
  * `README.md`: this README.
  * `scripts/`: contains various helper scripts
      * `./scripts/pycobench`: is a local and modified version of [pycobench](https://github.com/ondrik/pycobench) script
        used for small benchmarking. `./scripts/pycobench` can be used for 
        running list of benchmarks (one per line specified in text file or passed by `<` operator from 
        stdin) on list of tools (specified in `.yaml` format; see `tools.yaml` to see how to add new a~tool) 
        and outputs a `.output` file which contains the log of the benchmarking, and `./scripts/pyco_proc`
        which takes `.output` file and creates either `csv`, `html` or `text` report of the results.
        We generate results in `csv` file, which list times and other metrics for each tool on each benchmark
        delimited by `;`.
          * You can specify concrete tools delimited by `;` using `-m` parameter, e.g. 
            (`./pycobench/bin/pycobench -m jaltimpact;mata-nfa`)
          * You can specify timeout for each tool by `-t` parameter (e.g., `./scripts/pycobench -t 60`)
          * We recommend to use `./scripts/run_pyco.sh` instead.
      * `run_pyco.sh`:  The script takes list
        of paths to `.input` files, which contains paths to benchmarks (see `automata-bench/` for examples
        of `.input` files). The script generates files according to the used benchmark, timeout,
        measured tools, number of jobs and time/date. The script supports following options:
        * `-c|--config conf.yaml` to change the measured tools to a specification in `conf.yaml` file.
        * `-t|--timeout TIME` to change the internal timeout of benchmarking to `TIME` seconds.
        * `-v|--verbose` to print additional information about the benchmarking process (for debug mostly).
        * `-m|--methods a;b;c` to change the measured tools to only measure tools `a`, `b` and `c`.
        * `-j|jobs JOBS` to change the benchmarking to run with `JOBS` threads.
          We advise not to use all 8 CPUs.  In our experience, it leads to some unexpected behaviour.
        * `-s|suffix STRING` to append additional string to results (this is mostly used for better
          tagging of the results).
      * `process_pyco.sh`: don't use this at all, use `run_pyco.sh` instead.
      * `compare_profiles.py`: run as `python3 compare_profiles [file1.csv, file2.csv, ..., filen.csv]`;
        prints short summary of the results in form of tables.
      * `run_test.sh`: a verifier script, that runs one single benchmark to verify everything is 
         running correctly
      * `run_all.sh`: a master script that should replicate the results of TACAS'24. The script runs all 
        benchmarks that we used in the paper with default timeout `60s` and with all tools specified in
        `jobs/*.yaml`. :warning: The script takes quite a time. 
  * `jobs/*.yaml`: specification of tools in `yaml` format which is supported by `pycobench`.
    You can specify tool as follows:

      > tool:
      >   cmd: run_tool.sh $1.fmt

    This will register tool under the name `tool` which is run as `run_tool.sh`, takes `*.fmt` files on
    input. The `$1` will be substituted by concrete benchmarks. We recommend writing wrapper shell 
    scripts.
  * `inputs/*.input`: list of automata (one per line; multiple automata can be delimited with `;`), which
    are fed to compiled binaries/programs using pycobench.

## Running benchmarks

To test that the enviroment is working correctly run the following script:

    ./run_test.sh

To run all of the benchmarks, simply execute the following script:

    ./run_all.sh

If the VM is in pristine condition, this should replicate (to a certain degree)
results presented in our paper in TACAS'24. You can make your own changes, and
run the benchmarks, or extend the script with other benchmarks (by default, it
runs XX benchmarks distributed over XX directories that we described in the
paper or in the `nfa-bench/README.md`). This runs all tools on all experiments
with 6 parallel workers. The output is saved in the `results/` directory. 
This generates `csv` files for each benchmark directory. Note that for some
benchmarks and tools we do not have corresponding benchmark file, which is marked
in the `csv` file as either by `SKIPPED` or `MISSING`.

You can run the benchmark with more options by running `./run_experiments.sh`.
For example, to run tools `mata`, `vata`, `brics` and `automata` on
`bool_comb` and `automata_inclusion` benchmark directories with 7 workers and
`120s` timeout you can run the following:

    ./run_experiments.sh -m mata;vata;brics;automata-j 7 -t 120s inputs/bool_comb.input inputs/automata_inclusion.input

(Note that this command takes some time.) This should generate two files:
`results/automata_inclusion-DATE-timeout-120-jobs-7-m-mata;vata;brics;automata.csv`
and
`results/data/bool_comb-DATE-timeout-120-jobs-7-m-mata;vata;brics;automata.csv`,
where `DATE` is the time of starting the measurement.

For more fine tuning of the process read the `./run_experiments.sh`, `automata-bench/README.md` 
or `results/README.md`.


## Interpretation of results

We advise to check `results/README` or
[VeriFIT/afa-comparison-results](https://github.com/VeriFIT/mata-comparison-results)
repository for more details about interpretation of results.

The results are in `.csv` format delimited by `;`. We recommend using `python3` and
`pandas` library to further interact with the results:

```python3
    import pandas
    pandas.read_csv("results/data.csv")
```

This loads a `csv` file in `DataFrame` representation, that can be used easily to
handle common grouping, statistics, and many other data operations.
See [pandas](https://pandas.pydata.org/) for more information.

## Adding new tool

If you wish to add a new tool to this virtual machine, please follow these recommendations:

  1. Install any prerequisites you need (see [Access Credentials](#access) if you need 
  sudo). We strongly advise to keep it simple and prefer isolated environments (e.g.
  using python `venv`).
  2. Install your tool in `tools\` directory. Consider adding your tool to `$PATH`.
  3. We advise to create a wrapping script that takes at least one argument (name
  of the benchmark), and internally handles errors and outputs. Be default, `pycobench` times
  the runtime of the whole tool, but one can measure additional metrics by including `key: value\n`
  lines in the output. Such lines in output will be recorded in the final `.csv` as `tool-key`, with
  cells corresponding to `value`.
  4. Register your tool in `tools.yaml`.

      > tool:
      >   cmd: run_tool.sh $1.fmt

  5. Now you can run your tool.

## Adding new benchmark

  1. Copy your benchmark to `nfa-bench` directory.
  2. Generate `.input` files that will list benchmarks one per line. 
  3. You can add your benchmark to `run_all.sh` or run it by itself.

      > ./run_experiments.sh automata-bench/my_bench

## Troubleshooting

  1. *You found bug in some tool*: contact us. There might be some error
     that we missed.
  2. *You found inconsitency in results*: try re-running the tools. While, we are
     quite confident that the results should be consistent, there might be some
     nondeterministic errors that manifest under certain circumstances.
  3. *The tool fails in benchmarking, however, when manually run it simply takes too long:*
     try lowering the number of jobs below 7. `pycobench` seems to have some
     internal problems when run with the maximal number of jobs converting 
     timeouts to errors instead.
  4. *The tool performs better, when run alone with single job only*: this
     might happen, as some tools could (e.g. `mata`) potentially compete
     over resources, such as for memory, resulting into worse performance.
  5. *The script that converts benchmarks to other formats prints some warning/error*:
     some warnings are expected (because of symlinks). If you encounter anything
     that seems to be unexpected, contact us.
  6. *Your benchmark cannot be converted to other formats*: please, send us report
     with your benchmark and output of the conversion script.

# Threats to Validity (known limitations)

  * We compared the tools only wrt overall time only; we didn't measure
  individual steps in solving, such as parsing, reductions, or
  complementations. The individual steps in each tool could explain some of the
  observations in more details. 

  * Comparing the tools based on other metrics (e.g., memory
  peak) could further highlight the strenghts of each tools. Nowadays, limiting
  the memory peak in exchange of machine time can be useful in order to employ
  the tools in cloud infrastructure. Metrics other than time could be measured
  to compare the tools and their methods better (e.g., explored state space),
  however, we argue that the machine time is the most generic metric for
  comparing such a wide, diverse range of tools.

  * We compared the tools in a virtualized environment running in parallel.
  The virtualized environment could introduce some overhead to running some of
  the tools, however, it should not be as significant. 

  * Running the tools in parallel could potentially slow down some of the tools
  or their steps (e.g, by competing over the resources or interfering with
  cache access). 

  * Due to the time limitations, we did not run enough warm-ups, hence the
  results could be less stable. However, due to the sheer size of the
  benchmarks, we believe the results are stable enough to provide fair
  comparison. 

  * Increasing the timeout could help the tools to solve more
  problems, however, in our experience, this did not lead to significant
  improvements.

  * The selected benchmarks represent only a limited class of problems, and,
  moreover, some contain only a selected range of inputs (e.g., in \bincl{},
  \bparam{} or \aparam{}).  There may be other existing suitable benchmarks for
  comparison. However, we believe the selected benchmarks are quite diverse:
  there are parametric benchmarks, benchmarks that come from practice or
  benchmarks that were used to evaluate other approaches published in papers.  

  * Other kinds of parametric formulae (e.g., introducing alternations, or other
  patterns that can potentially be hard for solvers) would yield further insight
  on compared tools.

  * In previous section, we outlined, that the comparison of the tools might be
  unfair, since the tools: (1) supports different formats, (2) requires
  hand-written parser, (3) are implemented in different languages. 

  * E.g., \dotnet{} library could perform better on native Windows system, instead
  of Unix system employing .NET architecture that might be less efficient. The
  results, however, suggests that despite these challenges, each tool could
  perform well. 

  * Implementing the corresponding tools in same language, with same format could
  indeed provide more fair comparison. 

  * Implementing support of the uniform format could also reduce some of the
  overhead of the parsing, as well as mitigate possible overhead of conversion
  between formats.

  * Most of the tools can be run with different optimizations, heuristics,
  or arguments. Fine tuning these parameters could potentially enhance the
  results for the tools. Better evaluation of different parameters could be
  interesting, however, it currently is beyond the scope of this project. We
  argue, that using the default settings of the tools is still fair enough.

  * There might be tools or libraries that could potentially outperform
  the selected tools or provide a~more meaningful comparison. However, the chosen
  tools represent well the approaches presented in the paper. 

  * Some of the existing tools are either not well-maintained and could not be
  built or are proprietary.

## Changelog

  * **August 2023**: Basic version

## Contact Us

  - **Lukáš Holík** ([kilohsakul](https://github.com/kilohsakul)): the supreme leader, the emperor of theory;
  - **Ondřej Lengál** ([ondrik](https://github.com/ondrik)): prototype developer and the world's tallest hobbit;
  - Martin Hruška ([martinhruska](https://github.com/martinhruska)): library maintainer;
  - Tomáš Fiedor ([tfiedor](https://github.com/tfiedor)): python binding maintainer;
  - David Chocholatý ([Adda0](https://github.com/Adda0)) library and binding developer;
  - Juraj Síč ([jurajsic](https://github.com/jurajsic)): library developer;
  - Tomáš Vojnar ([vojnar](https://github.com/vojnar)): the spiritual leader;

## Acknowledgements

This work has been supported by the Czech Ministry of Education, Youth and Sports
ERC.CZ project LL1908, and the FIT BUT internal project FIT-S-20-6427.
