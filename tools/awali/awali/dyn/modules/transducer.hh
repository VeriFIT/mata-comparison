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

#ifndef DYN_MODULES_TRANSDUCER_HH
#define DYN_MODULES_TRANSDUCER_HH

#include <awali/dyn/modules/automaton.hh>
#include <awali/dyn/core/transducer.hh>

namespace awali {
  namespace dyn {

    //to move in module 'context' for consistency ?
//     transducer_t make_transducer(std::vector<std::string> alphabets,
//                                  std::string semiring = "B");
//     transducer_t make_transducer(std::vector<std::string> alphabets);


    // "internal" functions takes automaton_t while "public" functions takes
    // transducer_t
    namespace internal {
      unsigned num_tapes (automaton_t tdc);
      transition_t set_tdc_transition (automaton_t tdc, state_t src, state_t dst,
                                       const std::vector<std::string>& labels);

      transition_t get_tdc_transition(automaton_t tdc, state_t src, state_t dst,
                                      const std::vector<std::string>& labels);

      bool has_tdc_transition(automaton_t tdc, state_t src, state_t dst,
                              const std::vector<std::string>& labels);

// //     weight_t add_tdc_transition(automaton_t tdc, state_t src, state_t dst,
//         const std::vector<std::string>& labels);

      void del_tdc_transition(automaton_t tdc, state_t src, state_t dst,
                              const std::vector<std::string>& labels);

      transition_t set_tdc_transition(automaton_t tdc, state_t src, state_t dst,
                                      const std::vector<std::string>& labels,
                                      weight_t w);

      weight_t add_tdc_transition(automaton_t tdc, state_t src, state_t dst,
                                  const std::vector<std::string>& labels,
                                  weight_t w);

      std::vector<std::string> get_tdc_label(automaton_t tdc, transition_t tr);

      std::vector<state_t> tdc_successors(automaton_t tdc, state_t s,
                                          const std::vector<std::string>& label);
      std::vector<state_t> tdc_predecessors(automaton_t tdc, state_t s,
                                            const std::vector<std::string>& label);
      std::vector<transition_t> tdc_in(automaton_t tdc, state_t s,
                                       const std::vector<std::string>& label);

      std::vector<transition_t> tdc_out(automaton_t tdc, state_t s,
                                        const std::vector<std::string>& label);

      std::vector<char> input_alphabet(automaton_t tdc);

      std::vector<char> output_alphabet(automaton_t tdc);

      void set_final_output(automaton_t tdc, state_t src,
                            const std::string& output);

      std::string get_final_output(automaton_t tdc, state_t src);


      std::vector<std::vector<char>> alphabets(automaton_t tdc);

      bool has_label(automaton_t tdc, unsigned i, std::string l);
      bool has_input_label(automaton_t tdc, std::string l);

      bool has_output_label(automaton_t tdc, std::string l);

      automaton_t lift_tdc(automaton_t tdc);
    } // end of namespace awali::dyn::internal

    /** \ingroup Transducer
     * Returns the automaton corresponding to the second tape of the
     * transducer.
     * @param tdc
     * @see domain,projection
     **/
    automaton_t domain(transducer_t tdc);


    /** \ingroup Transducer
     * Returns the automaton corresponding to the second tape of the
     * transducer.
     * @param tdc
     * @see domain,projection
     **/
    automaton_t image(transducer_t tdc);


    /** \ingroup Transducer
     * Projects out the very first tape of the transducer.
     * @param tdc
     * @return a new transducer
     * @beware The returned transducer may have 0 or 1 tape, but still be a
     * transducer
     */
    transducer_t images(transducer_t tdc);

    /** \ingroup Tranducer
     * Projects \p tdc on the tape \p i.
     * @param tdc Transducer to project
     * @param i tape on which to project
     * @return Return a new automaton
     */
    automaton_t projection(transducer_t tdc, unsigned i);

    /** \ingroup Transducer */
    transducer_t inverse(transducer_t tdc);

    /** \ingroup Transducer */
    transducer_t compose(transducer_t tdc1, transducer_t tdc2);

    /** Tests whether \p tdc is functional.
     *
     * @param tdc
     * @return `true` if \p tdc is functional.
     */
    bool is_functional(transducer_t tdc);


    /** \ingroup Transducer */
    bool is_synchronizable(transducer_t tdc);

    /** \ingroup Transducer */
    transducer_t synchronize(transducer_t tdc);

    /** \ingroup Transducer */
    transducer_t realtime(transducer_t tdc);

    /** \ingroup Transducer */
    bool is_realtime(transducer_t tdc);

    /** \ingroup Transducer */
    bool is_of_finite_image(automaton_t tdc, unsigned i=0);

    /** \ingroup Transducer */
    automaton_t letterize_tape(automaton_t tdc, unsigned i=1);

  }
}//end of ns awali::dyn
#endif

