# First program


## Prerequisites

We assume that awali is compiled and installed.


## Content of file `firstprogram.cc`

```c++
#include<iostream>

//Header file for awali::dyn
#include<awali/dyn.hh>

using namespace awali::dyn;

int main(int argc, char** argv) {
    // Building an empty automaton over alphabet {a,b,c}:
    automaton_t aut = automaton_t::from("abc");

    // Adding one initial state:
    state_t s0 = aut->add_state();
    aut->set_initial(s0);

    // Adding one final state:
    state_t s1 = aut->add_state();
    aut->set_final(s1);

    // Adding three transitions:
    aut->add_transition(s0,s0,'a');
    aut->add_transition(s0,s0,'b');
    aut->add_transition(s0,s0,'c');
    aut->add_transition(s0,s1,'c');

    // `aut` accepts all words that end with letter c,
    // as shown by the rational expression:
    ratexp_t expr = aut_to_exp(aut);
    std::cout << expr << std::endl;

    // `aut` is nondeterministic; we may compute an equivalent deterministic 
    // automaton (`aut` is not modified):
    automaton_t aut_det = determinize(aut);

    // We may also compute its minimization
    // (`aut` and `aut_det` are not modified):
    automaton_t aut_det_min = minimal_automaton(aut_det);

    // Finally, we save the automaton to file `firstprogram.json`:
    // (Default format of awali is json.)
    save(aut_det_min,"firstprogram.json");

    // It can be retrieved later on with:
    automaton_t aut2 = load("firstprogram.json");
}
```


## Compiling

```sh
    c++ firstprogram.cc -o firstprogram -lawalidyn
```


## Going further

* Several example programs using the dynamical C++ library of Awali can be found in directory `awali/dyn/examples/` of the tarball, or of the `share` directory (usually `/usr/share/` or `/usr/local/share/` )
* Available functions may be found in the following files of the tarball or
in the include directory of your system (usually `/usr/include/` or `/usr/local/include/`):
    * `awali/dyn/core/automaton.hh` describes the type `automaton_t` and
    associated methods.
    * `awali/dyn/core/abstract_ratexp.hh` describes the type `ratexp_t` and
    associated methods.
    * `awali/dyn/modules/*.hh` and `awali/dyn/algos/*.hh` gives the     
    available algorithms.
    * the main header file, `awali/dyn.hh`, gives a brief description of the
    content of each of the above files.





