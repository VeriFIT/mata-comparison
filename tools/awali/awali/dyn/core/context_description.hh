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

#ifndef DYN_CONTEXT_DESCRIPTION_HH
#define DYN_CONTEXT_DESCRIPTION_HH

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <awali/common/json/node.hh>

//Definitions and factories for
// -labelset_description
// -weightset_description
// -context_description
#include <awali/dyn/core/context_description/description_types.hh>

//Weightset descriptions are handled by object corresponding to
//each semiring
#include <awali/dyn/core/context_description/weightsets/abstract_weightset.hh>
//The simples case (B, Q, Z-max-plus, etc...) are basic_weightset
#include <awali/dyn/core/context_description/weightsets/basic_weightset.hh>
//The other cases may have specific type
#include <awali/dyn/core/context_description/weightsets/cyclic_weightset.hh>
#include <awali/dyn/core/context_description/weightsets/bounded_weightset.hh>

namespace awali {
  namespace dyn {
    namespace context {
      //To be register in abstract_weighset, each semiring requires
      //the instanciation of a static object

      inline
      const std::vector<abstract_weightset*>& instances() {
        static basic_weightset _b_description("B","b",{}, // In spite of {}, B has
                    "Boolean semiring");      //  promotion to every weightset
        static basic_weightset _n_description("N","n",{"N-oo","Z","Q","R","C"},
                    "Natural integer semiring, ie non-negative integers");
	static basic_weightset _z_description("Z","z",{"Q","R","C"},
                    "Ring of the integers  -- a principal ideal domain indeed");
	static basic_weightset _q_description("Q","q",{"R","C"},
                    "Field of the rational numbers");
	static basic_weightset _r_description("R","r",{"C"},
                    "Field of the real numbers");
	static basic_weightset _c_description("C","c",{},
                    "Field of the complex numbers");
	static basic_weightset _f2_description("F2","f2",{},
                    "Field with 2 elements, 0 and 1 -- isomorphic with Z/2Z");
	static basic_weightset _noo_description("N-oo","noo",{},
                    "Semiring of the natural integers completed with "
                    "an infinite element (hence starable)");
	static basic_weightset _zmin_description("Z-min-plus","zmin",{},
                    "Semiring of the integers equipped with min (for addition) "
                    "and + (for multiplication)");
	static basic_weightset _zmax_description("Z-max-plus","zmax",{},
                    "Semiring of the integers equipped with max (for addition) "
                    "and + (for multiplication)");
	static basic_weightset _rmaxp_description("R-max-prod","pmax",{},
                    "Semiring of the real numbers equipped with max "
                    "(for addition) and . (for multiplication)");
	static basic_weightset _maxminp_description("Fuzzy","maxmin",{},
                    "Semiring of the integers equipped with max (for addition) "
                    "and min (for multiplication) -- locally finite");
	static cyclic_weightset _cyclic_description;
	static bounded_weightset _bounded_description;
	static std::vector<abstract_weightset*> v{
	  &_b_description,
	    &_n_description,
	    &_z_description,
	    &_q_description,
	    &_r_description,
	    &_c_description,
	    &_f2_description,
	    &_noo_description,
	    &_zmin_description,
	    &_zmax_description,
	    &_rmaxp_description,
	    &_maxminp_description,
	    &_cyclic_description,
	    &_bounded_description};
	return v;
      }

      labelset_description oneset();

      labelset_description letterset(std::string s);

      labelset_description intletterset(int a, int b);

      labelset_description intletterset(int n);

      labelset_description wordset(std::string s);

      labelset_description nullableset(labelset_description ls1);

      labelset_description ltupleset(std::vector<labelset_description> lss);
    
      weightset_description weightset(const std::string &k);

      weightset_description wtupleset(std::vector<weightset_description> wss);

      weightset_description ratweight(context_description cd);

      context_description c_desc(labelset_description ls, 
                                 weightset_description ws);

      context_description parse_context(json::object_t const* jp);

      weightset_description parse_weightset(json::object_t const* jp);
    
      labelset_description parse_labelset(json::object_t const* jp);

      context_description parse_context(json::object_t const* p);

      context_description parse_context(std::istream& i);

      std::string tostring(labelset_description ls, bool dynamic);

      std::string tostring(context_description ct, bool dynamic);

      std::string tostring(weightset_description ws, bool dynamic);

      std::string tostring(context_description ct, bool dynamic);

      std::vector<std::string> all_weightset_public_static_names();
      
      std::vector<std::string> const&  weightset_promotion_public_static_names(weightset_description ws);

      std::vector<std::string> all_weightset_public_descriptions();

      std::string all_weightset_public_static_names_as_string();

      bool is_promotable(weightset_description src, weightset_description dest);
    }
  }
}   //end of namespaces awali::dyn::context, awali::dyn, and awali

#endif
