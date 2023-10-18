// This file is part of Awali.
// Copyright 2016-2023 Sylvain Lombardy, Victor Marsault, Jacques Sakarovitch
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

#ifndef AWALI_DYN_CORE_ANY_CC
#define AWALI_DYN_CORE_ANY_CC

#include <list>

#include <awali/common/tuple.hh>

#include <awali/dyn/core/any.hh>

#include <awali/common/priority.hh>
#include <awali/common/parse_exception.hh>

namespace awali { namespace dyn {


  std::ostream&
  operator<< (std::ostream& o, const any_t& a){
    return a.output(o);
  }

  namespace internal {


    template<typename M>
    typename M::value_t extract_value(const any_t& a, M const& set, 
      std::string const& value_name= "value" )
    {
      try { return any_cast<typename M::value_t>(a); } 
      catch(any_cast_exception const& e) {
        try { /* Trying to parse */
          std::stringstream ss;
          ss << a << std::flush;
          return awali::internal::to_value_of(set,ss.str()); 
        } 
        catch (parse_exception const&) { 
          std::stringstream ss;
          ss << e.what();
          ss << " \tAlso failed to parse string \"" << a 
             << "\" as a "<< value_name << " of " << set.vname() << ".";
          throw any_cast_exception(ss.str().c_str()); 
        }
        catch (std::domain_error const&) {
          std::stringstream ss;
          ss << e.what();
          ss << " \tAlso, parsing string \"" << a 
             << "\" as a " << value_name << " of " << set.vname()
             << " was successful but resulting "
             << value_name 
             << " does not belong to the "
             << value_name << "set.";
          throw any_cast_exception(ss.str().c_str()); 
        }
      }
    }

    template<typename M>
    typename M::genset_t::word_t extract_word(const any_t& a, M const& set){
      try { return any_cast<typename M::genset_t::word_t>(a); } 
      catch(any_cast_exception const& e) {
        try {
          std::stringstream ss;
          ss << a << std::flush;
          return awali::internal::to_word_of(set,ss.str()); 
        } 
        catch (parse_exception const&) { 
          std::stringstream ss;
          ss << e.what();
          ss << " \tAlso failed to parse string \"" << a 
             << "\" as a word of " << set.vname() << ".";
          throw any_cast_exception(ss.str().c_str()); 
        }
        catch (std::domain_error const&) {
          std::stringstream ss;
          ss << e.what();
          ss << " \tAlso, parsing string \"" << a 
             << "\" as a word of " << set.vname()
             << " was successful but resulting word is deemed invalid.";
          throw any_cast_exception(ss.str().c_str()); 
        }
      }
    }

    template<unsigned i, typename M, typename P>
    auto extract_(const any_t& a, unsigned n, const M& sets, priority::TWO<P>) 
    -> decltype((sets.template set<i>()),any_t(true)) 
    {
      if (n == i) {
        auto tuple = any_cast<typename M::value_t>(a);
        return std::get<i>(tuple);
      } 
      else if (n >= sets.size())
        throw std::out_of_range("Has: "+std::to_string(sets.size())
                                +". Asked: "+std::to_string(n)+".");
      else
        return extract_< (i+1)< M::size() ? i+1 : 0>(a,n,sets, priority::value);
    }

    template<unsigned i, typename M, typename P>
    any_t extract_(const any_t&, unsigned, const M& sets, priority::ONE<P>) 
    {
      throw any_cast_exception (
        "Values in set "+sets.vname()+" are not tuples.");
    }

    template<typename M>
    any_t extract(const any_t& a, unsigned n, const M& sets) 
    {
      return extract_<0>(a,n,sets,priority::value);
    }

    template<typename M, std::size_t... I>
    std::list<any_t>
    expand__(const any_t& a, const M&, 
        awali::internal::index_sequence<I...>) 
    {
      typename M::value_t tuple = any_cast<typename M::value_t>(a);
      const any_t& x 
      = any_t(std::get<I>(std::forward<typename M::value_t>(tuple))...);
      return any_cast<std::list<any_t>>(x);
    }

    template <typename P, typename M>
    auto
    expand_(any_t const& a, M const& sets, awali::priority::TWO<P>) 
    -> decltype((sets.indices),std::list<any_t>{}) 
    {
      return expand__(a, sets, sets.indices);
    }

    template <typename P, typename M>
    std::list<any_t>
    expand_(any_t const&, M const& sets, awali::priority::ONE<P>)
    {
      throw any_cast_exception("Values in set "+sets.sname()+" are not tuples."); 
    }

    template <typename M>
    std::list<any_t>
    expand(any_t const& a, M const& sets)
    {
      return expand_(a, sets, awali::priority::value);
    }

  } // end of namespace awali::dyn::internal

}}//end of ns awali::dyn and awali


#endif
