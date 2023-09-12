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


#ifndef DYN_CORE_RATEXP_CC
#define DYN_CORE_RATEXP_CC

#include <awali/dyn/core/ratexp.hh>
#include <awali/dyn/modules/context.hh>

namespace awali { namespace dyn {

  ratexp_t
  ratexp_t::operator+ (ratexp_t e2)
  {
    return (*this)->add(e2);
  }


  ratexp_t
  ratexp_t::operator* (ratexp_t e2)
  {
    return (*this)->mult(e2);
  }


  std::ostream&
  operator<< (std::ostream& o, ratexp_t e)
  {
    return e->print(o);
  }

  ratexp_t
  ratexp_t::from (std::string str,std::string weightset,std::string alphabet)
  { 
    return ratexp_t::from_context(
      str, 
      context::letterset(alphabet=="auto"?"":alphabet),
      context::weightset(weightset),
      alphabet != "auto"
    );
  }

  ratexp_t
  ratexp_t::from_context(std::string str, context::labelset_description ld, context::weightset_description wd, bool fixed_alphabet) 
  {
    return internal::make_ratexp_with_context(str,ld,wd,fixed_alphabet);
  }

  ratexp_t
  ratexp_t::from_context(std::string str, context_t ctx, bool fixed_alphabet) 
  {
    return internal::make_ratexp_with_context(str,ctx,fixed_alphabet);
  }

  ratexp_t
  ratexp_t::with_int_labels::from_range(std::string str, int l, int u,
                                        std::string weightset) 
  {
    return ratexp_t::from_context(
      str,
      context::intletterset(l,u),
      context::weightset(weightset)
    );
  }

  ratexp_t
  ratexp_t::with_int_labels::from_size(std::string str, unsigned n,
                                        std::string weightset) 
  {
    return ratexp_t::from_context(
      str,
      context::intletterset(n),
      context::weightset(weightset)
    );
  }
  
  ratexp_t
  ratexp_t::with_int_labels::from(std::string str, std::string weightset) 
  {
    return ratexp_t::from_context(
      str,
      context::intletterset(0),
      context::weightset(weightset),
      false
    );
  }

  ratexp_t
  ratexp_t::with_tuple_labels::from(std::string str, unsigned n, 
                                    std::string weightset)
  {
    return ratexp_t::from_context(
      str,
      context::ltupleset({
        context::nullableset(context::letterset("")),
        context::nullableset(context::letterset(""))
      }),
      context::weightset(weightset),
      false
    );
  }

  ratexp_t
  ratexp_t::with_tuple_labels::from(std::string str,  
                                    std::vector<std::string> alphabets, 
                                    std::string weightset)
  {
    std::vector<context::labelset_description> v;
    for (auto s : alphabets)
      v.push_back(context::letterset(s));
    return ratexp_t::from_context(
      str,
      context::ltupleset(v),
      context::weightset(weightset)
    );
  }



}}//end of ns awali::dyn


#endif
