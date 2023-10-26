# A Replication package for reproducing the results of paper "Mata: A Fast and Simple Finite Automata Library"

The link to the Zenodo archive: [10.5281/zenodo.10040695](https://doi.org/10.5281/zenodo.10040695)

This repository contains data, tools, benchmark and scripts for
replicating (and processing) tool paper describing `libmata` library.
This package is meant to be used for running benchmarks on set of
libraries that supports various types of Nondeterministic Finite Automata.

The available tools are listed in `jobs/*.yaml` (each file corresponds to
a~group of similar experiments); each tool is installed in `tools` directory,
and takes as input automata in various formats (each library tends to use
different format). We use `pycobench` (a python script) to measure the time of
each library and output a report in `csv` (delimited by `;`) which lists
individual measurements, results and some other metrics (e.g., time of
mintermization or optionally time of some preprocessing) for each
library. For each library we implemented particular source codes, that compiles
to an executable which models our experiments.

Note, that some tools/libraries are run either as a wrapper script (to simplify
the usage in benchmarking or fine-tuning the measurement) or specific binary
program that either takes an input automaton (and processes it) or might take
an input program that specifies sequence of automata operation (and executes
them in order; e.g., load automaton `A`; load automaton `B`; create
intersection of `A` and `B`).

The package contains dependencies, tools, as well as some additional
helper scripts. Each helper script is accompanied by its own (sometimes short)
documentation.

For the sake of simplicity we prepared several master scripts `run_all.sh` 
(to replicate---to a certain degree---the results presented in our paper),
`run_smoke_test.sh` (to test that package and tools were initialized correctly),
`run_subset.sh` (to replicate a subset of results presented in our paper), and
`generate_figs.py` (to replicate the figures and tables presented in the paper).

## Availability

We make our replication package (and some of its parts) as available as possible:

  * [10.5281/zenodo.10040695](https://doi.org/10.5281/zenodo.10040695): downloadable replication package.
  * [VM](https://zenodo.org/records/7113223): virtual machine, where one can run the experiments.
  * [The Package Repository](https://github.com/VeriFIT/mata-comparison): the repository with this package.
  * [Benchmarks](https://github.com/VeriFIT/nfa-bench): source benchmarks that were used in the comparison.
  * [Results](https://github.com/VeriFIT/mata-comparison-results): measured results presented in paper..
  * [libmata](https://github.com/VeriFIT/mata): our `libmata` library.
  * [automata.net](https://github.com/VeriFIT/Automata/tree/tacas24): our fork of `automata.net` library.
  * [nfa-program-parser](https://github.com/VeriFIT/nfa-program-parser/tree/tacas24-ae): our interpreter for `mata`, `vata`, `awali`, `fado` and `automata.py`.
  * [brics](https://github.com/VeriFIT/mata-comparison-brics/tree/tacas24): our interpreter for `brics` and `automatalib`.

## Replicating the results 

In the following we describe necessary steps for replicating (to a certain degree) our results
presented in the tool paper.

Download the `tacas-artifact.tar.gz` from [10.5281/zenodo.10040696](https://doi.org/10.5281/zenodo.10040696). Then unpack the artifact and change directory.

```shell
tar -xzvf tacas-artifact.tar.gz
cd tacas-artifact
```

We assume, that everything is executed from the root (`tacas-artifact`) folder, executing from other folders might not work.

### Installing requirements

For `offline` setup, we prepared all necessary requirements in `packages` and `pip-packages` directories.
You can run  the following script to install all the presented packages (this will take some time, between 20 and 30 minutes):

```shell
sudo ./install_requirements.sh
```

This will install the requirements for running `java` applications, `.NET` aplications, `libmata` and `awali`
libraries, necessary `python` packages (including python binding and `pyinterpret` script that runs python automata libraries.

If you are, running the experiments in TACAS'23 VM, you might need to run the following command (if you are running
as other user than `tacas23` than change the name to your username):

```shell
export PATH=$PATH:/home/tacas23/.local/bin
```

You can test, that everything is set up correctly by running `pyintepret` command, which should output
`usage: python3 interpreter.py <ENGINE> <PROGRAM.emp> [AUT1.mata, ..., AUTn.mata]`.

### Running smoke test (10 minutes)

To test that the environment is working correctly run the following script:

```shell
./run_smoke_test.sh
```

This will run only `b-smt` benchmark, with timeout `60s`, and runs only 
first two instances, in total 22 jobs should be run, and should take
at most 10 minutes. The results are stored in `./results/data/smoke-test`
directory.

You can then test replicating of the images as follows:

```shell
./generate_figs.py ./results/data/smoke-test
```

This will generate two figures (in `.png` and `.pdf` format), and five tables (in `.tex` format)
stored in `./results/data/smoke-test/figs` directory. The script also print the five tables 
in human readable to standard output.

Note, that the graphs will be not interesting, as they will contain only about two data points per tool.

### Running subset of benchmarks (~1-4 hours)

To run a selected subset of our experiments run the following script:

```shell
./run_subset.sh
```

This will run only three selected benchmarks (`armc-incl`, `lia-explicit` and `lia-symbolic`), with `4` parallel
jobs and 60 seconds timeout. This should take between one and four hours (depending on the number of parallel jobs),
timeout, and machine specification. (We run our experiments on Intel Core 3.4 GHz processor with 20GB RAM)

Note, that you might need to increase the RAM or number of CPU (if you are running the replication in VM),
or lower the number of parallel jobs (you can modify `run_subset.sh` and change `--jobs 4` to lower number of jobs,
where `--jobs 1` corresponds to sequential execution of the benchmarks).

Again, you can generate the figures and tables as follows:

```shell
./generate_figs.py ./results/data/partial-experiments
```

This will generate two figures (in `.png` and `.pdf` format), and five tables (in `.tex` format)
stored in `./results/data/partial-experiments/figs` directory as well as human readable tables to standard output. 

Note, that the graphs should replicate parts of the figures, for three benchmarks.

### Running all of the benchmarks (~13-24 hours; or even more)

To run all of the benchmarks, simply execute the following script:

    ./run_all.sh --output-dir replication

This should replicate (to a certain degree) results presented in our paper. 
Feel free, to make your own changes, and
run the benchmarks, or extend the script with other benchmarks (by default, it
runs about 11 benchmarks distributed over several directories that are described in the
paper or in the `nfa-bench/README.md`). This runs all tools on all experiments
with 6 parallel workers. The output is saved in the `results/data/replication` directory.
This generates `csv` files for each benchmark directory. 

Finally, you can replicate the figures and tables from our paper as follows:

```shell
./generate_figs.py ./results/data/replication
```

This will generate the figures and tables as follows:
  * **Table 1**: `stats-per-bench-{1, 2}.tex` contains two parts of the Table 1.
  * **Table 2**: `stats-relative.tex` contains the Table 2.
  * **Table 3**: `stats-per-op-{1, 2}.tex` contains two parts of the flipped Table 3.
  * **Figure 4**: `paper-cactus-plot-per-bench-sum-fairest-of-them-all-4-x-3.{png, pdf}`: contains cactus plots of Figure 4.
  * **Figure 5**: `paper-cactus-plot-per-operation-sum.{png, pdf}` contains cactus plots of Figure 5.

The output will look something like:

```shell
Processing: ./results/data/partial-experiments
100% (6 of 6) |########################################################| Elapsed Time: 0:00:00 Time:  0:00:00
 - Computing fair overall times
100% (5214 of 5214) |##################################################| Elapsed Time: 0:00:09 Time:  0:00:09
 - Generating Figure IV.
100% (3 of 3) |########################################################| Elapsed Time: 0:00:00 Time:  0:00:00
Saving to ./results/data/partial-experiments/figs/paper-cactus-plot-per-bench-sum-fairest-of-them-all-4-x-3.png
Saving to ./results/data/partial-experiments/figs/paper-cactus-plot-per-bench-sum-fairest-of-them-all-4-x-3.pdf
 - Generating Figure V.
100% (7 of 7) |########################################################| Elapsed Time: 0:00:00 Time:  0:00:00
Saving to ./results/data/partial-experiments/figs/paper-cactus-plot-per-operation-sum.png
Saving to ./results/data/partial-experiments/figs/paper-cactus-plot-per-operation-sum.pdf
 - Generating Table I.
100% (33 of 33) |######################################################| Elapsed Time: 0:00:00 Time:  0:00:00
100% (33 of 33) |######################################################| Elapsed Time: 0:00:00 Time:  0:00:00
benchmark     mata         awali         vata          automata.net
armc-incl     0|211|3|1 s  7|1 s|23|5 s  0|408|72|702  9|664|79|2 s
lia-explicit  0|39|6|330   6|18|18|13    0|91|40|455   0|65|67|25
lia-symbolic  1|2|1|6      1|8|6|21      1|7|6|8       1|73|71|31
Saved to ./results/data/partial-experiments/figs/stats-per-bench-1.tex

100% (33 of 33) |######################################################| Elapsed Time: 0:00:00 Time:  0:00:00
benchmark     mata         brics         automatalib    fado               automata.py
armc-incl     0|211|3|1 s  5|915|46|4 s  5|2 s|479|8 s  58|10 s|25 s|12 s  10|1 s|191|3 s
lia-explicit  0|39|6|330   0|46|46|48    0|310|223|866  1|857|424|2 s      1|229|105|543
lia-symbolic  1|2|1|6      0|42|42|35    0|127|131|49   1|114|104|253      1|34|28|64
Saved to ./results/data/partial-experiments/figs/stats-per-bench-2.tex

 - Generating Table II.
100% (116 of 116) |####################################################| Elapsed Time: 0:00:00 Time:  0:00:00
100% (116 of 116) |####################################################| Elapsed Time: 0:00:00 Time:  0:00:00
operation    mata       awali       vata        automata.net
complement   20|2|234   9|5|16      37|7|298    62|62|28
emptiness    ~0|~0|~0   1|1|1       5|1|14      ~0|~0|~0
inclusion    211|3|1 s  1 s|23|5 s  408|72|702  656|71|2 s
trim         ~0|~0|~0   2|1|1       5|1|14      6|6|1
Saved to ./results/data/partial-experiments/figs/stats-per-op-1.tex

100% (116 of 116) |####################################################| Elapsed Time: 0:00:00 Time:  0:00:00
100% (116 of 116) |####################################################| Elapsed Time: 0:00:00 Time:  0:00:00
operation    mata       brics       automatalib    fado            automata.py
complement   20|2|234   44|44|42    26|22|13       233|70|658      158|23|2 s
emptiness    ~0|~0|~0   ~0|~0|~0    2|2|~0         16|6|22         ~0|~0|~0
inclusion    211|3|1 s  915|46|4 s  63|55|29       10 s|25 s|12 s  528|101|1 s
trim         ~0|~0|~0   -           -              3|1|4           -
Saved to ./results/data/partial-experiments/figs/stats-per-op-2.tex
 - Generating Table III.
bench           mata    mata-sim    awali    vata    automata.net    brics    automatalib     fado    automata.py    (py)mata
armc-incl          1        1.04    29.77    1.93           14.46    18.37          38.09  4585.83          20.86        0.95
lia-explicit       1        0.61     2.12    2.33            1.68     1.18           7.94    62.4           16.72        0.89
lia-symbolic       1        0.96     4.03    3.68           35.44    20.75          61.63    55.32          16.91        1.46

Saved to ./results/data/partial-experiments/figs/stats-relative.tex
```

### Checking our results

In `results/data/tacas24` we provide the original data in `.csv` formats that we used to generate our results.
You can inspect them yourself, or run:

```shell
./generate_figs.py ./results/data/tacas24
```

### Running custom experiments (who knows how long)

You can run `./run_all.sh --help` to print short help and parameters, that can be used to fine tune the experiments.

You can run the benchmark with more options by running `./run_all.sh`.
For example, to run tools `mata`, `vata`, `brics` and `automata` on
`b-smt` and `armc-incl` benchmark directories with 7 workers and
`120s` timeout and store the results in `results/data/my_experimenst` directory 
you can run the following:

    ./run_experiments.sh -m mata;vata;brics;automata-j 7 -t 120s --b-smt --armc-incl --output-dir my_experiments

(Note that this command takes some time.) This should generate two files:
`results/data/my_experiments/tacas-24-automata-inclusion-bench-double-automata-inclusion-DATE-timeout-120-jobs-7-m-mata;vata;brics;automata.csv`
and
`results/data/my_experiments/tacas-24-bool-comb-ere-bench-variadic-bool-comb-ere-DATE-timeout-120-jobs-7-m-mata;vata;brics;automata.csv`,
where `DATE` is the time of starting the measurement.

For more fine tuning of the process read the `./run_all.sh --help`, or `nfa-bench/README.md`.

### Interpretation of results

We provide two ways of interpreting the results.

You can either run `./generate_figs.py <PATH_TO_DIRECTORY_CONTAINING_CSV>` that will generate figures and tables
to `figs` dir or you can use our jupyter notebook by running:

```shell
cd results/
jupyter notebook
```

This will run instance of jupyter notebook accessible through your web browser (the exact address will be printed
to standard output). You can then run the notebook to replicate the figures and tables in same way as using
`generate_figs.py`.

All results are in `.csv` format delimited by `;`. To manually inspect the data, we recommend using `python3` and
`pandas` library to further interact with the results:

```python3
import pandas
pandas.read_csv("results/data/<DIR>/<FILE>.csv")
```

This loads a `csv` file in `DataFrame` representation, that can be used easily to
handle common grouping, statistics, and many other data operations.
See [pandas](https://pandas.pydata.org/) for more information.

### Estimated Runtimes of Benchmarks

We estimate the runtimes of each benchmark (if run sequentially, i.e., run with `--jobs 1`) as follows (sorted by runtime):

  1. `lia-explicit`: around half hour;
  2. `lia-symbolic`: around 1 hour;
  3. `armc-incl`: around 2 hours;
  4. `noodler-compl`: around 2 hours;
  5. `email-filter`:  around 4 hours;
  6. `noodler-concat`: around 4 hours;
  7. `b-smt`: around 6 hours;
  8. `noodler-inter`: around 10 hours;
  9. `param-union`: around 12 hours.
  10. `param-inter`: around 30 hours;

Note, this is estimated for 8 cores and 20GB, hence, if run in VM this might take longer. Running instances in parallel will speed up the experiments (however, the results might be less stable).

## Troubleshooting

1. *The `awali` tool takes much more time than others/in results*: When run for the first time,
   `awali` needs to create a certain context for each operation. Running the instance again
   should be fast again. 
2. *`automata` fails with error, that `BoolCombinationEmptiness.dll` does not exists*: this might
   be incorrectly compiled `automata` library or issue with symbolic links.
3. *There is some weird error*: we strongly advise to not unpack the package in shared directory.
   There are known issues in creating symlinks in shared directories (i.e., directories shared
   between virtual machine and host). We advise to move the package directly to virtual machine and
   rerun the experiments.
4. *You found bug in some tool*: Naturally, There might be some error
   that we missed.
5. *You found inconsitency in results*: try re-running the tools
   or decrease number of jobs or decrease/increase the timeout.
   We cannot guarantee that the experiments will behave the same on all architectures
   and machines. The tools and experiments are quite complex, and there might
   be contest for resources between the tools, as well as some tools are quite resource-hungry. 
   While, we are quite confident that the results should be consistent, there might be some
   nondeterministic errors that manifest under certain circumstances.
6. *The tool fails in benchmarking, however, when manually run it simply takes too long:*
   try lowering the number of jobs below 7. `pycobench` seems to have some
   internal problems when run with the maximal number of jobs converting
   timeouts to errors instead.
7. *The tool performs better, when run alone with single job only*: this
   might happen, as some tools could (e.g. `mata`) potentially compete
   over resources, such as for memory, resulting into worse performance.
8. *The script that converts benchmarks to other formats prints some warning/error*:
   some warnings are expected (because of symlinks). If you encounter anything
   that seems to be unexpected, please tell us.
9. *Your custom benchmark cannot be run*: please, tell us report
   with your benchmark and output of the conversion script.

## Technical Specification / Requirements

We ran our experiments on Debian GNU/Linux 11, with IntelCore 3.4GHz process, 8
CPU cores, and 20 GB RAM. Such specification should be enough to reproduce our
results.

Increasing the amount of RAM or number of cores might lead to better results.
The virtual machine machine requires at least 35GB of spaces, however, we
recommend users to allocate at least 70GB.

It is recommended to allow network access to this virtual machine, since we
track our benchmarks in remote repository. This way one can pull the latest
version of benchmarks.

## <a id="access"></a> Access Credentials (if using TACAS23 VM)

The virtual machine contains a single user `tacas23` with admin rights, hence,
you can install additional packages, change configurations, etc. The password
for the user is `tacas23`.

> :warning: We strongly advise to either backup the image, or proceed carefully,
> before doing any major changes to the virtual machine. We are not responsible
> for any damage you will done by making major changes to the machine.

## File and directory Structure

The package has following files in the root directory:

  * `install_requirements.sh`: prepares the environmen.t
  * `run_smoke_test.sh`: runs short (~1-10min) reproducibility test; requires confirmation.
  * `run_subset.sh`: runs moderate (1-4hours) experiments; requires initial confirmation.
  * `run_all.sh`: runs all experiments (13+ hours).
  * `generate_figs.py`: generates figure and tables for directory with `.csv` results.

The package has following directory structure:

  * `bin`: a~directory containing precompiled binaries and commands; these can be used right after installing
    all the requirements (using `install_requirements.sh`)
  * `nfa-bench/`: a~directory, which contains our benchmarks. 
    See [VeriFIT/automata-bench](https://github.com/nfa-bench) or `./nfa-bench/README.md` 
    for more information about each benchmark.
  * `results/`: contains jupyter notebook for managing the results and `data` repository containing the measured results in `.csv` format.
  * `tools/`: directory, which contains installed tools. 
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
        * `-j|--jobs JOBS` to change the benchmarking to run with `JOBS` threads.
          We advise not to use all 8 CPUs.  In our experience, it leads to some unexpected behaviour.
        * `-s|--suffix STRING` to append additional string to results (this is mostly used for better
          tagging of the results).
      * `process_pyco.sh`: don't use this at all, use `run_pyco.sh` instead.
      * `compare_profiles.py`: run as `python3 compare_profiles [file1.csv, file2.csv, ..., filen.csv]`;
        prints short summary of the results in form of tables.
      * `build_all.sh`: script that builds all the tools from `tools/` directory; this should not be needed at all
        since the binaries in `bin/` should be executable. Run this, if you made any changes to the source codes;
        we are not responsible for any screw ups during the build.
      * `massage_arguments.py`: this is used to convert the `.mata` files into `.mata.minterm` files, which are 
        automata in `mata` format after mintermization algorithm, that are used as inputs for some tools.
      * `pack_me.sh`: creates archive of this replication package.
      * `run_alib.sh`/`run_automata.sh`/`run_brics.sh`: wrapper scripts for running selected tools.
      * Other scripts are not needed.
  * `jobs/*.yaml`: specification of tools in `yaml` format which is supported by `pycobench`.
    You can specify tool as follows:

      > tool:
      >   cmd: run_tool.sh $1.fmt

    This will register tool under the name `tool` which is run as `run_tool.sh`, takes `*.fmt` files on
    input. The `$1` will be substituted by concrete benchmarks. We recommend writing wrapper shell 
    scripts.
  * `inputs/*.input`: list of automata (one per line; multiple automata can be delimited with `;`), which
    are fed to compiled binaries/programs using pycobench.
  * `programs`: contains programs, that are interpreted by binaries in `bin` or python commands; these corresponds
    to sequence of automata operations (e.g. loading automata, complementing automata, intersecting automata).
  * `packages`: contains debian packages necessary for replicating the results.
  * `pip-packages`: contains python packages necessary for replicating the results.

## Adding new tool

If you wish to add a new tool to this package, please follow these recommendations:

  1. Install any prerequisites you need (see [Access Credentials](#access) if you need 
  sudo). We strongly advise to keep it simple and prefer isolated environments (e.g.
  using python `venv`).
  2. Install your tool in `tools\` directory. Consider adding your tool to `$PATH`.
  3. We advise to create a wrapping script that takes at least one argument (name
  of the benchmark), and internally handles errors and outputs. Be default, `pycobench` times
  the runtime of the whole tool, but one can measure additional metrics by including `key: value\n`
  lines in the output. Such lines in output will be recorded in the final `.csv` as `tool-key`, with
  cells corresponding to `value`.
  4. Register your tool in `jobs`*.yaml`.

      > tool:
      >   cmd: run_tool.sh $1.fmt

  5. Now you can run your tool.

## Adding new benchmark

  1. Copy your benchmark to `nfa-bench` directory.
  2. Generate `.input` files that will list benchmarks one per line. 
  3. Add your benchmark to `run_all.sh`.

## Threats to Validity (known limitations)

  * Comparing the tools based on other metrics (e.g., memory
  peak) could further highlight the strengths of each tools. Nowadays, limiting
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
  moreover, some contain only a selected range of inputs.  
  There may be other existing suitable benchmarks for
  comparison. However, we believe the selected benchmarks are quite diverse:
  there are parametric benchmarks, benchmarks that come from practice or
  benchmarks that were used to evaluate other approaches published in papers.  

  * Other kinds of parametric formulae (e.g., introducing alternations, or other
  patterns that can potentially be hard for solvers) would yield further insight
  on compared tools.

  * The comparison of the tools might be
  unfair, since the tools: (1) supports different formats, (2) requires
  hand-written parser, (3) are implemented in different languages. 

  * E.g., `automata.net` library could perform better on native Windows system, instead
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
  tools represent the state-of-the-art well. 

  * Some of the existing tools are either not well-maintained and could not be
  built or are proprietary.

## Contact Us

  - **Lukáš Holík** ([kilohsakul](https://github.com/kilohsakul)): the supreme leader, the emperor of theory;
  - **Ondřej Lengál** ([ondrik](https://github.com/ondrik)): prototype developer and the world's tallest hobbit;
  - Martin Hruška ([martinhruska](https://github.com/martinhruska)): library maintainer;
  - Tomáš Fiedor ([tfiedor](https://github.com/tfiedor)): python binding maintainer;
  - David Chocholatý ([Adda0](https://github.com/Adda0)) library and binding developer;
  - Vojtěch Havlena ([vhavlena](https://github.com/vhavlena)) library developer;
  - Juraj Síč ([jurajsic](https://github.com/jurajsic)): library developer;

## Acknowledgements

This work has been supported by the Czech Ministry of Education, Youth and Sports
ERC.CZ project LL1908, and the FIT BUT internal project FIT-S-20-6427.
