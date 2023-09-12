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

#ifndef DYN_FACTORIES_HH
#define DYN_FACTORIES_HH

#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/core/abstract_context.hh>
#include <awali/dyn/loading/handler.hh>
#include <awali/dyn/modules/context.hh>
#include <awali/dyn/core/any.hh>
#include <vector>

namespace awali {
  namespace dyn {
    namespace factory {

    //(a+b)*a(a+b){n}
    static automaton_t n_ultimate(context_t ctx, label_t l, unsigned n) {
      return loading::call0<automaton_t>("n_ultimate", "factories", ctx->sname(), ctx, l, n);
    }
    
    static automaton_t divkbaseb(context_t ctx, unsigned k, unsigned b) {
      return loading::call0<automaton_t>("divkbaseb", "factories", ctx->sname(), ctx, k, b);
    }
    
    static automaton_t double_ring(context_t ctx, unsigned n, const std::vector<unsigned>& finals) {
      return loading::call0<automaton_t>("double_ring", "factories", ctx->sname(), ctx, n, finals);
    }
    
    static automaton_t ladybird(context_t ctx, unsigned n) {
      return loading::call0<automaton_t>("ladybird", "factories", ctx->sname(), ctx, n);
    }

    static automaton_t cerny(context_t ctx, unsigned n) {
      return loading::call0<automaton_t>("cerny", "factories", ctx->sname(), ctx, n);
    }

    static automaton_t witness(context_t ctx, unsigned n) {
      return loading::call0<automaton_t>("witness", "factories", ctx->sname(), ctx, n);
    }
    ///
    
    automaton_t n_ultimate(unsigned n, const std::string& alphabet, const std::string& semiring) {
      context_t ctx=internal::make_context(c_desc(context::letterset(alphabet), context::weightset(semiring)));
      return n_ultimate(ctx, alphabet[0], n);
    }
    
    automaton_t divkbaseb(unsigned k, unsigned b, const std::string& alphabet, const std::string& semiring) {
      auto cletterset = context::letterset(alphabet);
      if(alphabet=="auto") {
        if(b>36)
          throw std::runtime_error("Explicit alphabet required for base > 36");
        std::string alph(b,'0');
        for(unsigned i=0; i<b; i++) {
          if(i<10)
            alph[i]='0'+i;
          else
            alph[i]='A'+(i-10);
        }
        cletterset = context::letterset(alph);
      }
      context_t ctx= internal::make_context(
                c_desc(cletterset, context::weightset(semiring)));
      return divkbaseb(ctx, k, b);
    }
    
    automaton_t double_ring(unsigned n, const std::vector<unsigned>& finals, const std::string& alphabet, const std::string& semiring) {
      context_t ctx=internal::make_context(c_desc(context::letterset(alphabet), context::weightset(semiring)));
      return double_ring(ctx, n, finals);
    }

    automaton_t ladybird(unsigned n, const std::string& alphabet, const std::string& semiring) {
      context_t ctx=internal::make_context(c_desc(context::letterset(alphabet), context::weightset(semiring)));
      return ladybird(ctx, n);
    }

    automaton_t cerny(unsigned n, const std::string& alphabet, const std::string& semiring) {
      context_t ctx=internal::make_context(c_desc(context::letterset(alphabet), context::weightset(semiring)));
      return cerny(ctx, n);
    }

    //Brzozowski universal witness
    automaton_t witness(unsigned n, const std::string& alphabet, const std::string& semiring) {
      context_t ctx=internal::make_context(c_desc(context::letterset(alphabet), context::weightset(semiring)));
      return witness(ctx, n);
    }

    //with int letters
    
    automaton_t n_ultimate(unsigned n, int a, int b, const std::string& semiring) {
      context_t ctx=internal::make_context(c_desc(context::intletterset(a,b), context::weightset(semiring)));
      return n_ultimate(ctx, a, n);
    }
    
    automaton_t int_divkbaseb(unsigned k, unsigned b, const std::string& semiring) {
      context_t ctx=internal::make_context(c_desc(context::intletterset(0,b-1), context::weightset(semiring)));
      return divkbaseb(ctx, k, b);
    }
    
    automaton_t double_ring(unsigned n, const std::vector<unsigned>& finals, int a, const std::string& semiring) {
      context_t ctx=internal::make_context(c_desc(context::intletterset(a,a+1), context::weightset(semiring)));
      return double_ring(ctx, n, finals);
    }

    automaton_t ladybird(unsigned n, int a, const std::string& semiring) {
      context_t ctx=internal::make_context(c_desc(context::intletterset(a,a+2), context::weightset(semiring)));
      return ladybird(ctx, n);
    }

    automaton_t cerny(unsigned n, int a, int b, const std::string& semiring) {
      context_t ctx=internal::make_context(c_desc(context::intletterset(a,b), context::weightset(semiring)));
      return cerny(ctx, n);
    }

    //Brzozowski universal witness
    automaton_t witness(unsigned n, int a, const std::string& semiring) {
      context_t ctx=internal::make_context(c_desc(context::intletterset(a,a+2), context::weightset(semiring)));
      return witness(ctx, n);
    }
    } //end of ns awali::dyn::factory
  } // end of ns awali::dyn
}//end of ns awali

#endif
