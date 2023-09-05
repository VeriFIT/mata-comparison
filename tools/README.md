# Tools

The following briefly introduces the tools and directories.

## Automata

This is a C# library implementing symbolic NFA/DFA; available at [Automata](https://github.com/AutomataDotNet/Automata).

  * In results and tools `tools.yaml` this is denoted as: `automata`; the tool is stored in `Automata-minimal` directory 

  * Takes `.emp` files.

  * Either use `run_automata.sh` or run directly as follows:

```shell
  /home/debian/tools/Automata-minimal/bin/linux-x64/BoolCombinationEmptiness input.emp
```

  * The output is either `EMPTY` or `NOT EMPTY`

## Bricks

This is a baseline automata library primarly focusing on DFA; available at [dk.brics.automaton](https://www.brics.dk/automaton/).

  * In results and tools `tools.yaml` this is denoted as: `bricks`; the tools is stored in `emptiness-brics` directory.

  * Takes `.emp` files.

  * Run directly as follows:

```shell
  java -jar /home/debian/tools/emptiness-brics/target/brics-emptiness.jar [--minimize] input.emp
```
  
  * You can use `--minimize` to further minimize the automata (this might lead to some optimization, but in our experience, it does not have much effect on our BRE and AFA problems).

  * The output is either `EMPTY` or `NOT EMPTY`

## libmata

This is our own implementation of NFA. This is built over our library
[libmata](https://github.com/VeriFIT/mata).

  * In results and tools `tools.yaml` this is denoted as: `mata-nfa`: the tool is stored in `afa-emptiness-checker` and `afa-emptiness-checker-inclusion`.

  * Takes `.emp` files.

  * The two version differs in different handling of inclusion.

  * Run `./build/src/nfa-emptiness-checker` in corresponding directories to run the tool.

  * The output is either `EMPTY` or `NOT EMPTY`

## Mona

This is an optimized implementation of DFA; available at [The Mona project](https://www.brics.dk/mona/).
The parser for `.emp` files is our own.

  * In results and tools `tools.yaml` this is denoted as: `mona`: the tool is stored in `mata2mona`.

  * Takes `.emp` files.

  * Either use `run_mona.sh` script or run directly as follows:

```shell
  /home/debian/tools/mata2mona/main input.emp
```

  * The output is either `EMPTY` or `NOT EMPTY`

## VATA

This is a library implementing NFA; available at [libvata](https://github.com/ondrik/libvata).

  * In results and tools `tools.yaml` this is denoted as: `vata`; the tools is stored in `afa-emptiness-checker-vata` or `afa-emptiness-checker-vata-inclusion`.

  * Takes `.emp` files.

  * The version differes in handling the inclusion operations.

  * Run directly as follows:

```shell
  /home/debian/tools/afa-emptiness-checker-vata/build/src/nfa-emptiness-checker input.emp
```

  * The output is either `EMPTY` or `NOT EMPTY`
