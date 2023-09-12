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

#ifndef DYN_MODULES_TRANSDUCER_CC
#define DYN_MODULES_TRANSDUCER_CC

#include <awali/dyn/modules/transducer.hh>
#include <awali/dyn/modules/context.hh>
#include <awali/dyn/core/transducer.hh>

namespace awali {
  namespace dyn {

//     transducer_t make_transducer(std::vector<std::string> alphabets,
//                                  std::string semiring)
//     {
//       std::vector<context::labelset_description> v;
//       for(auto al : alphabets)
//         v.emplace_back(context::nullableset(context::letterset(al)));
//       return automaton_t::from_context(context::ltupleset(v),
//                                             context::weightset(semiring));
//     }

//     transducer_t make_transducer(std::vector<std::string> alphabets) {
//       return make_transducer(alphabets, "B");
//     }


    namespace internal {
      unsigned num_tapes(automaton_t tdc)
      {
        return loading::call1<unsigned>("num_tapes", "transducer", tdc);
      }

      transition_t set_tdc_transition(automaton_t tdc, state_t src,
                                      state_t dst, const std::vector<std::string>& labels)
      {
        return loading::call1<transition_t, automaton_t, state_t, state_t,
               const std::vector<std::string>&>
               ("set_tdc_transition", "transducer", tdc, src, dst, labels);
      }

      transition_t get_tdc_transition(automaton_t tdc, state_t src, state_t dst,
                                      const std::vector<std::string>& labels)
      {
        return loading::call1<transition_t, automaton_t, state_t, state_t, const std::vector<std::string>&>("get_tdc_transition",
               "transducer", tdc, src, dst, labels);
      }

      bool has_tdc_transition(automaton_t tdc, state_t src, state_t dst,
                              const std::vector<std::string>& labels)
      {
        return loading::call1<bool, automaton_t, state_t, state_t, const std::vector<std::string>&>("has_tdc_transition",
               "transducer", tdc, src, dst, labels);
      }

      weight_t add_tdc_transition(automaton_t tdc, state_t src, state_t dst,
                                  const std::vector<std::string>& labels)
      {
        return loading::call1<weight_t, automaton_t, state_t, state_t, const std::vector<std::string>&>("add_tdc_transition",
               "transducer", tdc, src, dst, labels);
      }

      void del_tdc_transition(automaton_t tdc, state_t src, state_t dst,
                              const std::vector<std::string>& labels)
      {
        loading::call1<void, automaton_t, state_t, state_t, const std::vector<std::string>&>("del_tdc_transition",
            "transducer", tdc, src, dst, labels);
      }

      transition_t set_tdc_transition(automaton_t tdc, state_t src, state_t dst,
                                      const std::vector<std::string>& labels, weight_t w)
      {
        return loading::call1<transition_t, automaton_t, state_t, state_t, const std::vector<std::string>&, weight_t>("set_wtdc_transition",
               "transducer", tdc, src, dst, labels, w);
      }

      weight_t add_tdc_transition(automaton_t tdc, state_t src, state_t dst,
                                  const std::vector<std::string>& labels, weight_t w)
      {
        return loading::call1<weight_t, automaton_t, state_t, state_t, const std::vector<std::string>&, weight_t>("add_tdc_wtransition",
               "transducer", tdc, src, dst, labels, w);
      }

      std::vector<std::string> get_tdc_label(automaton_t tdc, transition_t tr)
      {
        return loading::call1<std::vector<std::string>>("get_tdc_label", "transducer",
               tdc, tr);
      }

      std::vector<state_t> tdc_successors(automaton_t tdc, state_t s, const
                                          std::vector<std::string>& label)
      {
        return loading::call1<std::vector<state_t>, automaton_t, state_t, const std::vector<std::string>&>("tdc_successors",
               "transducer", tdc, s, label);
      }

      std::vector<state_t> tdc_predecessors(automaton_t tdc, state_t s,
                                            const std::vector<std::string>& label)
      {
        return loading::call1<std::vector<state_t>, automaton_t, state_t, const std::vector<std::string>&>("tdc_predecessors",
               "transducer", tdc, s, label);
      }

      std::vector<transition_t> tdc_in(automaton_t tdc, state_t s,
                                       const std::vector<std::string>& label)
      {
        return loading::call1
               < std::vector<transition_t>, automaton_t, state_t,
               const std::vector<std::string>& >
               ("tdc_in", "transducer", tdc, s, label);
      }

      std::vector<transition_t> tdc_out(automaton_t tdc, state_t s,
                                        const std::vector<std::string>& label)
      {
        return loading::call1
               < std::vector<transition_t>, automaton_t, state_t,
               std::vector<std::string>const& >
               ("tdc_out", "transducer", tdc, s, label);
      }

      std::vector<char> input_alphabet(automaton_t tdc)
      {
        return loading::call1<std::vector<char>>("input_alphabet", "transducer", tdc);
      }

      std::vector<char> output_alphabet(automaton_t tdc)
      {
        return loading::call1<std::vector<char>>("output_alphabet", "transducer", tdc);
      }

      void set_final_output(automaton_t tdc, state_t src,
                            const std::string& output)
      {
        loading::call1<void, automaton_t, state_t, const std::string&>("set_final_output",
            "transducer", tdc, src, output);
      }

      std::string get_final_output(automaton_t tdc, state_t src)
      {
        return loading::call1<std::string>("get_final_output", "transducer", tdc, src);
      }


      std::vector<std::vector<char>> alphabets(automaton_t tdc)
      {
        return loading::call1<std::vector<std::vector<char>>>("alphabets", "transducer",
               tdc);
      }

      bool has_label(automaton_t tdc, unsigned i, std::string l)
      {
        auto v=alphabets(tdc);
        auto al=v[i];
        for(auto k: al)
          if(k==l[0])
            return true;
        return false;
      }

      bool has_input_label(automaton_t tdc, std::string l)
      {
        return has_label(tdc, 0, l);
      }

      bool has_output_label(automaton_t tdc, std::string l)
      {
        return has_label(tdc, 1, l);
      }

      automaton_t lift_tdc(dyn::transducer_t tdc)
      {
        return loading::call1<automaton_t>("lift_tdc", "transducer", tdc);
      }
    } //end of namespace awali::dyn::internal

    automaton_t domain(transducer_t tdc)
    {
      return loading::call1<automaton_t>("domain", "transducer", tdc);
    }

    automaton_t image(transducer_t tdc)
    {
      return loading::call1<automaton_t>("image", "transducer", tdc);
    }

    transducer_t images(transducer_t tdc)
    {
      return loading::call1<transducer_t>("images", "transducer", tdc);
    }

    automaton_t projection(transducer_t tdc, unsigned i)
    {
      if(i==0)
        return domain(tdc);
      else
        if(i==1)
          return image(tdc);
        else return projection(images(tdc), i-1);
    }

    transducer_t inverse(transducer_t tdc)
    {
      return loading::call1<transducer_t>("inverse", "transducer", tdc);
    }

    transducer_t compose(transducer_t tdc1, transducer_t tdc2)
    {
      return loading::call2<transducer_t>("compose", "compose", tdc1, tdc2);
    }

    bool is_functional(transducer_t tdc)
    {
      return loading::call1<bool>("is_functional", "transducer", tdc);
    }

    bool is_synchronizable(transducer_t tdc)
    {
      return loading::call1<bool>("is_synchronizable", "transducer", tdc,
                                  &is_synchronizable);
    }

    transducer_t synchronize(transducer_t tdc)
    {
      return loading::call1<transducer_t>("synchronize", "transducer", tdc);
    }

    transducer_t realtime(transducer_t tdc)
    {
      return loading::call1<transducer_t>("realtime", "transducer", tdc);
    }

    bool is_realtime(transducer_t tdc)
    {
      return loading::call1<bool>("is_realtime", "transducer", tdc);
    }

    bool is_of_finite_image(automaton_t tdc, unsigned i)
    {
      return loading::call1<bool>("is_of_finite_image", "transducer", tdc, i);
    }

    automaton_t letterize_tape(automaton_t tdc, unsigned i)
    {
      return loading::call1<automaton_t>("letterize_tape", "transducer", tdc, i);
    }
  }
}//end of ns awali::dyn, and awali

#endif
