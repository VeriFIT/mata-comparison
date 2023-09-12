Doxygen Mainpage{#mainpage}
================

# Introduction
**Awali** is a software platform for computing with **finite automata**, in the
broadest meaning of the term.
It includes classical finite automata (rather called here Boolean automata);
**finite weighted automata** with the instantiation of a large panel of weight
semirings, **finite (weighted) transducers**, that is, automata over tuples of
words rather than over words; and **rational expressions** for all these.

**Awali** is organised into three layers: the static layer, the dynamic layer,
and the interface layers.
This doxygen documentation is about the former two. 
(The documentation of **Cora** may be found with the `help` command; the
documentation of the Python interface is directly available in the docstrings).

- The **static layer** is part of namespace #awali::sttc.  It focuses on
  efficiency rather than user-friendlyness.  As a result all types and
  functions are highly templated.  Experts only.
- The **dynamic layer** is contained in namespace #awali::dyn.  All types
  are erased in order to have only a few types.
  Recommended for C++ programmers.
- A few constructs that are common to both layers (mostly enums and hash
  functions) are directly in namespace #awali.


# Executive summary of the dynamical layer
## Getting started
- It is recommended to use the following lines in your programs (it is usually
  assumed in examples).
```
  #include <awali/dyn.hh>
  using namespace awali::dyn;
  using namespace awali; // Mostly useful with the Options mechanism
```
- Compile with option `-lawalidyn`
- Take a look at files in directory `awali/dyn/examples` for a few examples.
- At runtime, static modules are compiled the very first time an algorithm
  is executed in a given context.  It takes up to twenty seconds.
## Brief description of content
- **Automata** are of type #awali::dyn::automaton_t (see this class for
  constructors) but basic methods for automata are in class
  #awali::dyn::abstract_automaton_t.  The same goes for **weighted automata**.
- **Transducers** are of type #awali::dyn::transducer_t.
- **Rational expressions** are of type #awali::dyn::ratexp_t (see this class
  for constructors) but basic methods are in class
  #awali::dyn::abstract_ratexp_t.
- See also group {@link Factories} for a list of all **factories and
  constructors**.
- For the list of **available algorithms** at the dynamical layers, see
  #awali::dyn.
- The labelset and weightset  of an automaton (or transducer, or expression)
  are contained in a statical object wrapped into a #awali::dyn::context_t (see
  #awali::dyn::abstract_context_t for provided methods) and may be described
  pre-construction by a #awali::dyn::context::labelset_description and a
  #awali::dyn::context::labelset_description.
