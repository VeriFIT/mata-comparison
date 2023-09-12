// This file is part of Awali.
// Copyright 2016-2021 Sylvain Lombardy, Victor Marsault, Jacques Sakarovitch
//
// Awali is a free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.


/*****************************************************************************
 See template-docfile.hh for documentation and how characters are preprocessed
******************************************************************************/

#ifndef AWALI_COMMON_DOCSTRING_JSONFORMAT_HH
#define AWALI_COMMON_DOCSTRING_JSONFORMAT_HH

#include <awali/common/docstring/entry.hh>

namespace awali { namespace docstring {

static entry_t json_format = {
/* Name:  */ "json-format",
/* Description: */ "The json format used in awali for automata and expressions",
/* Title: */ "Json format for automata and expressions",
/* Content: */ 
R"---doxytoken---(  
In Awali, automata and rational expressions are described (or linearised)
with __json files__. 

Their syntax is described here by means of a (CF) grammar.
Non-terminals are written between double square brakets, e.g. `[[kind]]`.
Lists of objects in a json *array* are described by the term `list`.
E.g. `[ [[state]] list ]` stands for ` [ ([[state]],)*[[state]] ] | [] `,
that is, a list may be empty.


## 1. Awali objects

A json file in Awali is an object with 4 fields, two of which are optional.

`[[Awali json file]] ::= { [[kind]], [[metadata]], [[context]], [[data]] }`
    
### 1.1 The 4 first level fields

* `[[kind]]` , *mandatory* , is an object with two possible values only:

`[[kind]] ::= {"kind":"Automaton"} | {"kind":"Rational Expression"}`

* `[[metadata]]` , *optional* , is an object with six fields, all *optional*:

* `[[context]]` , *mandatory* , describes the _type_ of the awali object,
                  automaton or expression, with the same syntax in both cases.

* `[[data]]` , *optional* , describes the _content_ of the automaton or 
               expression. Does not appear for empty automaton or expression.

`[[data]] ::= [[automaton content]] | [[ratexp content]]`


### 1.2 Metadata

The metadata fields contains descriptive informations on the object, automaton
or expression. All fields are optional.

`[[metadata]] ::= { [[name]], [[caption]], [[description]], `
                   ` [[timestamp]], [[author]], [[history]] }`


* `[[name]] ::= "name":"<string>"`   
`<string>` is the name of the automaton or expression. It is recommended that
this name coincides with the name of the json file.

* `[[caption]] ::= "caption":"<string>"`  
`<string>` is a one line description of the automaton (or expression).

* `[[description]] ::= "description":[("<string>",)*"<string>"]`  
The value of this field is an *array* of *strings* which give a long
description of the automaton (or expression).

* `[[timestamp]] ::= "timestamp":{"day":"<string>", "UTCtime":"<string>"}`

* `[[author]] ::= "author":"<string>"`   


## 2. Context of an automaton or an expression

[[context]] is an *object* that contains two mandatory *fields*:

`[[context]] ::= "context":{ [[labels]], [[weights]] }`

 * `[[labels]]`  describes the type of the labels of the transitions 
                  of the automaton or of the atoms of expression.
                                  
 * `[[weights]]` tells in which semiring the weights of the automaton or 
                  of the expression are taken.

### 2.1 Label set description

[[labels]] is an *object* that contains various *fields* whose appearance 
or values may depend on each other:

`[[labels]] ::= "labels": { [[labelKind]], [[letterType]], [[alphabet]], `
                           `[[tapeLabels]], [[allowEpsilon]] }`

 * The `labelKind` field takes one of 4 possible values :  
  `[[labelKind]] ::= "labelKind":[[LBK-str]]`  
  `[[LBK-str]] ::= "Letters" | "Words" | "No Label" | "Multi-tape" `
 
     * `"Letters" `  if the labels are __letters__   
     * `"Words" `    if the labels are __words__    
     * `"No Label"`  if every transition is labelled by the __empty word__    
                     (for automata only: the automaton is then a weighted
                     directed graph)
     * `"Multi-tape"` for __transducers__

* The `letterType` field exists if the value of `labelKind` is either
  `"Letters"` or `"Words"`; the value of this field is either `"Char"` or 
  `"Integer"`
 
  `[[letterType]] ::= "letterType": "Char" | "letterType": "Integer" `  
         
* The `alphabet` field exists if the value of `"labelKind"` is either
  `"Letters"` or `"Words"`. Its value is an array of elements which must be
  of the type given in `letterType`. Example:

  `"labels": {"labelKind":"Letters", "letterType":"Char", `
              `"alphabet":["a","b"]}` 
      
* The `tapeLabels` field exists if the value of `labelKind` is `Multi-tape`,
  that is, if the automaton is a __transducer__. Its value is an array of 
  `labels`, each one describing the labels on one tape.

  `[[tapeLabels]] ::= [ [[labels]] list ]`
 
  __NB1__ As indicated by the grammar, a transducer may have 0 or 1 tape only.
  A 0-tape transducer is rather a weird object. A 1-tape transducer is 
  isomorphic to, but of different (C++) type than, a plain automaton.
 
  __NB2__ With the `cora` interface, only 2-tape transducers can be dealt with. 
 
* The `allowEpsilon` field exists if the value of `labelKind` is either
  `Letters` or `Multi-tape`.

  `[[allowEpsilon]] ::= "allowEpsilon":false | "allowEpsilon":true `

  The default value is `false`; it can be set to `true` to indicate that the
  automaton may have transitions labelled with the empty word.

In the data part of the Awali object, the representation of the label of a 
transition depends on the nature of labels. See below 3.2.


### 2.2 Weight set description

The weight set of an automaton or an expression is either a *basic* semiring,
or a *series* semiring, or a direct product of weight sets. 
Hence the production:

`[[weights]] ::= "weights":{ ([[basicSR]]|[[paramSR]]|[[expSR]]|[[tupleSR]]) }`

The four non-terminals develop in the following way.

* `[[basicSR]] ::= "semiring": ("B"|"N"|"N-oo"|"Z"|"Q"|"R"|"C"|"F2"`
                              `"Z-min-plus"|"Z-max-plus"|"R-max-prod"|"Fuzzy")`

 The description of these semirings is to be found in the documentation on
 weight sets.
                                                           
* `[[paramSR]] ::= "semiring": ("Cyclic"|"Bounded"), "characteristic": <int>`

  * A `"weights": { "semiring":"Cyclic", "characteristic":<int> }` refers to 
    the semiring `Z/<int>Z` and `<int>` must be equal to, or greater than, 2. 
  
  * A `"weights": { "semiring":"Bounded", "characteristic":<int> }` refers to 
    the qotient of `N` by the congruence generated by `<int> = <int>+1`.
    The integer `<int>` must be (strictly) positive. 

* `[[expSR]] ::= "expressionSR": { [[context]] }`

  Series are represented by *rational expressions* whose __type__ is given
  by `[[context]]`. These expressions are considered modulo *natural 
  identities* (see documentation on rational expressions), and hence
  form a semiring.

* `[[tupleSR]] ::=  { "tupleSR": [ ([[weights]],)* [[weights]] ] }`
                                 
  In this case, the weight semiring is the direct product of the semirings 
  given in the list (which must be of length at least 1), that is, the 
  operation are realised component wise.
  
In the data part of the Awali object, the representation of a weight depends on
the nature of weights. 


## 3. Content of an automaton

An `automaton content` is an *object* that contains two mandatory *fields*:

`[[automaton content]] ::= "data" : { [[states]], [[transitions]]}`

### 3.1 States

`[[states]]` is a object whose value is the __list__ of states of the automaton.
`[[state]]`  gives information on every state. 

`[[states]] ::= {"states": [ [[state]] list ] }`

`[[state]] ::= { "id":<int>, "name":<string> (opt.), "history":<string> (opt.),`
                `"initial":<weight> (opt.), "final":<weight> (opt.) }`


* `"id":<int>` :  `<int>` is the __identifier__ of the state.  
                 It must be unique w.r.t. the other states. *(mandatory field)*

*  `"name":<string>` : `<string>` labels the state.

*  `"history":<string>` : `<string>` gives a description on the way the state
                         has been built (from product, determinization, ...).
                                                 
*  `"initial":<weight>` : `<weight>` is the *initial weight* of the state.  
                         The syntax for writing `<weight>` depends on the 
                         weightset described in the `context`. See Sec. 3.4.

*  `"final":<weight>` : `<weight>` is the *final weight* of the state. 
                        Idem *initial*.


### 3.2 Transitions

`[[transitions]]` is a object whose value is the __list__ of transitions of the
automaton. `[[transition]]`  gives information on every transition. 

`[[transitions]] ::= {"transitions": [ [[transition]] list ] }`

`[[transition]] ::= { "id":<int> (opt.), "source":<int>, "destination":<int>,` 
                     `"label":<label>, "weight":<weight> (opt.) }`

* `"id":<int>` : `<int>` is the __identifier__ of the transition.
             It must be unique w.r.t. the other transitions. *(optional field)*

* `"source":<int>` : `<int>` is the __identifier__ of the source state of 
                     the transition.

* `"destination":<int>` : `<int>` is the __identifier__ of the destination state
                          of the transition.
                                                  
* `"label":<label>` : `<label>` is the *label* of the transition.
                      The syntax for writing `<label>` depends on the 
                      *labelset* described in the `context`. See Sec. 3.3.

  This field is __mandatory__ except if `labelKind` is set to `"No   Labels"`.  
  For *epsilon transitions*, `<label>` is set to `null`.
                      

* `"weight":<weight>` : `<weight>` is the *weight* of the transition.  
                        The syntax for writing `<weight>` depends on the 
                        weightset described in the `context`. See Sec. 3.4.					 
						 
  This field is __optional__ except if `labelKind` is set to `"No Labels"`, in
  which case it is forbidden; if it is not present, `<weight>` is set to 
  the identity of the weightset.
  
  __NB__ The value *zero* of the weightset is forbidden for `<weight>`: it is
  equivalent to *no transition* and the transition should thus not appear in
  the list. Accordingly, the writing of the weights in the 2-element 
  semirings B or F2=Z/<2>Z is not relevant since the default value is the 
  only possible one.
  

### 3.3 Writing of labels

The writing of `<label>` depends on `labelKind` and `letterType`.  
If `labelKind` is `Letters` or `Words`, the representation is given by the
following table:

|             | letterType | Char | Integer         |   
|-------------|------------|------|-----------------|    
|__labelKind__|            |      |                 |   
| Letters     |            |string|string or integer|   
| Words       |            |string|array of Letters |    


If `labelKind` is `Multi-tape`, the representation is an array of labels.  

In any case, `<label>` must be written with the *letters* defined in `alphabet`.


### 3.4 Writing of weights

A value in json may be a __string__, a __Boolean__, an __integer__,
a __float__, or an array of the former.

|          |     string      | integer | float | bool |      array of        |
|----------|-----------------|---------|-------|------|----------------------|
|  B       |"0" or "1"       | 0 or 1  |       | Yes  |                      |
|  N       |integer rep.     | Yes     |       |      |                      |
|  N-oo    |integer rep.     | Yes     |       |      |                      |
|  Z       |integer rep.     | Yes     |       |      |                      |
|  R       |int or float rep.| Yes     |  Yes  |      |                      |
|  Q       |integer rep.     | Yes     |       |      | 2 integers or strings|
|  C       |int or float rep.| Yes     |  Yes  |      | 2 floats or strings  |
|  F2      |"0" or "1"       | 0 or 1  |       | Yes  |                      |
|Z-min-plus|int rep. or "oo" | Yes     |       |      |                      |
|Z-max-plus|int rep. or "-oo"| Yes     |       |      |                      |
| Fuzzy    |int rep.         | Yes     |       |      |                      |
|R-max-prod|int or float rep.| Yes     |  Yes  |      |                      |
| Cyclic   |integer rep.     | Yes     |       |      |                      |
| Bounded  |integer rep.     | Yes     |       |      |                      |

If the *weightset* is a set of series or expressions, `<weight>` is written as 
an expression, see Sec. 4 below. If the *weightset* is a tuple of weightsets,
`<weight>` is an *array* whose elements must be congruent with the description
given in the `context`.


## 4. Content of a rational expression

A `ratexp content` is an *object* whose value is a rational expression.
The grammar is almost self-explanatory. 

`[[ratexp content]] ::= "data":{ [[ratexp]] }`

`[[ratexp]] ::= { [[ratexpNode]], [[lweight]] (opt.), [[rweight]] (opt.) }`

`[[ratexpNode]] ::=  [[zero]]|[[one]]|[[label]]|[[sum]]|[[prod]]|[[star]] `

`[[lweight]] ::= "lweight":<weight>`

`[[rweight]] ::= "rweight":<weight>`

The value `<weight>` of the field `lweight` (resp. `rweight`) is the 
coefficient that multiplies on the left (resp. on the right) the expression
represented by `[[ratexpNode]]` and allows to build *weighted expressions*.
The rest of the grammar is classical rational (regular) expressions.

`[[zero]] ::= "zero":null`

`[[one]] ::= "one":null`

`[[label]] ::= "label":<label>`

`[[sum]] ::= "sum":[ [[ratexp]] list ]`   __list__ of lenght at least 2 

`[[prod]] ::= "prod":[ [[ratexp]] list ]` __list__ of lenght at least 2

`[[star]] ::= "star":{ [[ratexp]] }`


* `"zero":null`  is the representation of the expression 0. 
  It never occurs but for the case of that expression (see doc on rational
  expressions).

* `"one":null`  is the representation of the expression 1 (hence denoting
   the empty word).
   
* `"label":<label>` : `<label>` denotes an atom of the expression. Its writing 
                      must follow the syntax described in the `context`.

* `[[sum]]`  : the definition in the grammar makes `sum` an *n-ary* operator, 
               with `n>=2`.

* `[[prod]]`  : the definition in the grammar makes `prod` an *n-ary* operator, 
               with `n>=2`.

)---doxytoken---"
};

}} //End of namespaces awali::docstring and awali


#endif

