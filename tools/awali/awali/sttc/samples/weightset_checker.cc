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

#include <awali/sttc/weightset/b.hh>
/*
#include <awali/sttc/weightset/f2.hh>
#include <awali/sttc/weightset/pmax.hh>
#include <awali/sttc/weightset/q.hh>
#include <awali/sttc/weightset/r.hh>
#include <awali/sttc/weightset/z.hh>
#include <awali/sttc/weightset/zmax.hh>
#include <awali/sttc/weightset/zmin.hh>
#include <awali/sttc/weightset/zz.hh>
*/


template<typename W,
         typename ST = typename W::self_type,
         std::string (*SNAME)() = &W::sname,
         ST (*MAKE)(std::istream&) = &W::make,
         std::string (W::*VNAME)(bool) const = &W::vname,
         typename VALUE = typename W::value_t,
         VALUE (W::*ZERO)() const = &W::zero,
         VALUE (W::*ONE)() const = &W::one,
         VALUE (W::*ADD)(const VALUE&, const VALUE&) const = &W::add,
         VALUE (W::*MUL)(const VALUE&, const VALUE&) const = &W::mul,
         VALUE (W::*STAR)(const VALUE&) const = &W::star,
         bool (EQUALS)(const VALUE&, const VALUE&) = &W::equals,
         bool (LESS_THAN)(const VALUE&, const VALUE&) = &W::less_than,
         bool (W::*IS_SPECIAL)(const VALUE&) const = &W::is_special,
         bool (W::*IS_ZERO)(const VALUE&) const = &W::is_zero,
         bool (W::*IS_ONE)(const VALUE&) const = &W::is_one,
         bool (*IS_COMMUTATIVE)() = &W::is_commutative_semiring,
         bool (*SHOW_ONE)() = &W::show_one,
         awali::star_status_t (*STAR_STATUS)() = &W::star_status,
         VALUE (W::*TRANSPOSE)(const VALUE&) const = &W::transpose,
         size_t (*HASH)(const VALUE&) = &W::hash,
         VALUE (W::*CONV1)(ST,const VALUE&) const = &W::conv,
         VALUE (W::*CONV2)(std::istream&) const = &W::conv,
         VALUE (W::*PARSE)(const std::string&, size_t&) const = &W::parse,
         std::ostream& (W::*PRINT)(const VALUE&, std::ostream&, const std::string&) const = &W::print,
         std::ostream& (W::*PRINT_SET)(std::ostream&, const std::string&) const = &W::print_set,
         std::ostream& (W::*JS_PRINT)(std::ostream&) const = &W::js_print,
         VALUE (W::*JS_PARSE)(std::istream&) const = &W::js_parse>
struct checker {};


/* For simple weights,
 * 1. the size of values is small and arguments are passed by value;
 * 2. the behaviour of methods are independant from any dynamic information,
 *    hence, methods can be static
 */
template<typename W,
         typename ST = typename W::self_type,
         std::string (*SNAME)() = &W::sname,
         ST (*MAKE)(std::istream&) = &W::make,
         std::string (*VNAME)(bool) = &W::vname,
         typename VALUE = typename W::value_t,
         VALUE (*ZERO)() = &W::zero,
         VALUE (*ONE)() = &W::one,
         VALUE (*ADD)(VALUE, VALUE) = &W::add,
         VALUE (*MUL)(VALUE, VALUE) = &W::mul,
         VALUE (*STAR)(VALUE) = &W::star,
         bool (EQUALS)(VALUE, VALUE) = &W::equals,
         bool (LESS_THAN)(VALUE, VALUE) = &W::less_than,
         bool (*IS_SPECIAL)(VALUE) = &W::is_special,
         bool (*IS_ZERO)(VALUE) = &W::is_zero,
         bool (*IS_ONE)(VALUE) = &W::is_one,
         bool (*IS_COMMUTATIVE)() = &W::is_commutative_semiring,
         bool (*SHOW_ONE)() = &W::show_one,
         awali::star_status_t (*STAR_STATUS)() = &W::star_status,
         VALUE (*TRANSPOSE)(VALUE) = &W::transpose,
         size_t (*HASH)(VALUE) = &W::hash,
         VALUE (*CONV1)(ST, VALUE) = &W::conv,
         VALUE (*CONV2)(std::istream&) = &W::conv,
         VALUE (*PARSE)(const std::string&, size_t&) = &W::parse,
         std::ostream& (*PRINT)(VALUE, std::ostream&, const std::string&) = &W::print,
         std::ostream& (*PRINT_SET)(std::ostream&, const std::string&) = &W::print_set,
         std::ostream& (*JS_PRINT)(std::ostream&) = &W::js_print,
         VALUE (*JS_PARSE)(std::istream&) = &W::js_parse>
struct checker_simple {};

int main() {
  checker_simple<awali::sttc::internal::b_impl> x1;
  /*
  checker<awali::sttc::internal::f2_impl> x2;
  checker<awali::sttc::internal::pmax_impl> x3;
  checker<awali::sttc::internal::q_impl> x4;
  checker<awali::sttc::internal::r_impl> x5;
  checker<awali::sttc::internal::z_impl> x6;
  checker<awali::sttc::internal::zmax_impl> x7;
  checker<awali::sttc::internal::zmin_impl> x8;
  checker<awali::sttc::internal::zz_impl> x9;
  */
  return 0;
}
