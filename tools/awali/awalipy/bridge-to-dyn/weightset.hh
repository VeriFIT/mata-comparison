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

#ifndef PY_WEIGHTSET_HH
#define PY_WEIGHTSET_HH


namespace awali { namespace py {

  struct weightset_t {
   protected:
    dyn::context_t context_;
  
    
   public:
    weightset_t(const dyn::context_t &context) : context_(context) {}
   
    
    weightset_t() {}

    std::string zero() {
      return context_->weight_to_string(context_->weight_zero());
    }
  
    std::string one() {
      return context_->weight_to_string(context_->weight_one());
    }
    
    std::string mul_weights(std::string l, std::string r) {
      return context_->weight_to_string(
        context_->mul_weights(
          context_->get_weight(l),
          context_->get_weight(r)
        )
      );
    }

    std::string add_weights(std::string l, std::string r) {
      return context_->weight_to_string(
        context_->add_weights(
          context_->get_weight(l),
          context_->get_weight(r)
        )
      );
    }

    std::string div_weights(std::string l, std::string r) {
      return context_->weight_to_string(
        context_->div_weights(
          context_->get_weight(l),
          context_->get_weight(r)
        )
      );
    }
    std::string name(std::string format = "text") {
      return context_->weightset_name(format);
    }
  };



  weightset_t make_simple_weightset (const std::string& ws) {
    return weightset_t(
      dyn::internal::make_context(
        dyn::context::c_desc(
          dyn::context::letterset("ab"), //This won't be used.
          dyn::context::weightset(ws)
        )
      )
    );
   }

}}

#endif
