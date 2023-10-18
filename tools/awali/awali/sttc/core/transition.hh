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

#ifndef AWALI_CORE_TRANSITION_HH
# define AWALI_CORE_TRANSITION_HH

#include <awali/sttc/empty.hh>

#include <awali/sttc/misc/attributes.hh>

namespace awali {
  namespace sttc {

    namespace internal {
      /*,------------------------------------.
        | possibly_labeled_transition_tuple. |
        `-----------------------------------*/

      template<class State, class Label>
      struct possibly_labeled_transition_tuple
      {
        State src;
        State dst;

        using label_t = Label;
        label_t get_label() const { return label; }
        void set_label(label_t& l) { label = l; }

      private:
        Label label;
      };

      template<class State>
      struct possibly_labeled_transition_tuple<State, empty_t>
      {
        State src;
        State dst;

        using label_t = empty_t;
        label_t get_label() const { return {}; }
        void set_label(label_t) {}
      };
    }

  /*-------------------.
  | transition_tuple.  |
  `-------------------*/

  template<class State, class Label, class Weight>
  struct transition_tuple
    : internal::possibly_labeled_transition_tuple<State, Label>
  {
    using weight_t = Weight;
    weight_t get_weight() const { return weight; }
    void set_weight(weight_t& k) { weight = k; }

  private:
    weight_t weight;
  };

  // We do not store the Boolean weights, which are assumed to be
  // always true.  This is correct for weight in the Boolean ring, as
  // well as for those in the F₂ (a.k.a. ℤ/2ℤ) field, both encoded
  // using the bool type.
  template<class State, class Label>
  struct transition_tuple<State, Label, bool>
    : internal::possibly_labeled_transition_tuple<State, Label>
  {
    using weight_t = bool;
    weight_t get_weight() const { return true; }
    void set_weight(weight_t& k) { (void) k; assert(k == true); }
  };

}}//end of ns awali::stc

#endif // !AWALI_CORE_TRANSITION_HH
