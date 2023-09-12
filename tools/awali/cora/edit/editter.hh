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

#ifndef CORA_EDIT_EDITTER_HH
#define CORA_EDIT_EDITTER_HH

#include <ncurses.h>
#include <unordered_map>
#include <cstring>
#include <stack>
#include <fstream>
#include <awali/dyn.hh>

#include <cora/edit/area.hh>
#include <cora/edit/bless.hh>
#include <cora/edit/queued_ostream.hh>
#include <cora/edit/enums.hh>
#include <cora/edit/input.hh>
#include <cora/edit/notifiable.hh>
#include <cora/edit/tip.hh>
#include <cora/edit/token.hh>
#include <cora/edit/transition_pack.hh>
#include <cora/edit/validator.hh>

using namespace awali::cora::editter;
using namespace awali::dyn;

namespace awali { 
namespace cora {
  
  
  class editter_t : notifiable_t {

    dyn::automaton_t aut;
    dyn::transducer_t tdc;
    dyn::context_t ctx;
    std::vector<std::vector<std::string>> alphabets;
    bool is_transducer;
    bool is_int_automaton;
    bool has_epsilon;
    bool is_over_B;
    std::string weightset_name;
    queued_ostream_t message_queue;

    
    bool is_saved;
    bool ever_saved;
    void notify_modified();
    std::string filename;
    io_format_t output_format;
    
    tip_handler_t tip_handler;


    std::vector<dyn::state_t> states;
    std::vector<dyn::transition_t> transitions;

    
    dyn::state_t current_state() const;
    std::string current_state_name() const;
    dyn::transition_t current_transition() const;

    bool continue_edit = true;
    bool require_refresh = true;

    std::vector<sentence_t> transition_data;
    void clear_transition_data();
    sentence_t& get_transition_data (unsigned i);

    std::vector<sentence_t> state_data;
    void clear_state_data();
    sentence_t& get_state_data (unsigned i);
    
    std::string eps_repr = "_"; //Representation of epsilon
    std::string tdc_lbl_sta = "[";
    std::string tdc_lbl_sep = ",";
    std::string tdc_lbl_end = "]";
    std::string weight_sta = "<";
    std::string weight_end = ">";
    token_t ini_arr_format;
    token_t fin_arr_format ;
    token_t weight_format =   "<%s>";
    sentence_t label_format;
    token_t tr_sta; 
    token_t tr_end;
    sentence_t tr_format; 

  
/* w = width of terminal, 
   h = height of terminal
                        1                 1
                       >-<               >-<
         <-------------> <---------------> <-------------------------------->
  v         ~ w/4             ~ w/4                       ~w/2
0 |     +---------------+-----------------+----------------------------------+
  ^ ^   |               |                 |                                  |
    |   |               |                 |                                  |
    |   |               |                 |                                  |
    |   |               |                 |                                  |
    |   |               |                 |                                  |
h-6 |   |               |                 |                                  |
    |   |  infos_area   |   state_area    |            trans_area            |
    |   |               |                 |                                  |
    |   |               |                 |                                  |
    |   |               |                 |                                  |
    |   |               |                 |                                  |
    |   |               |                 |                                  |
  v v   |               |                 |                                  |
0 |     +---------------+-----------------+--------+-------------------------+
  ^     |          comment_area (2 line)           |                         |
        +------------------------------------------|  message_area (4 lines) |
  v     |           query_area (2 lines)           |                         |
0 |     +------------------------------------------+-------------------------+
  ^     |                             tips_area                              |
  v     |                             (2 lines)                              |
0 |     +--------------------------------------------------------------------+
  ^
         <----------------------------------------> <----------------------->
                            ~2w/3                              ~w/3
       >-<                                        >-<                       >-<
        0                                          1                         0
*/
    area_t screen, 
           infos_area,
           query_area,
           comment_area,
           message_area,
           tips_area,
           long_string_area;
   
    list_area_t state_area, 
                trans_area;


    input_kind_t wait_for_char(char& c); 

    /* width = 0 means auto */ 
    answer_kind_t wait_for_string(std::string& s, size_t width = 0);
    
//    /* General function for asking information to the user.
//     * A new string is asked to the user until given string passes the
//     * given validator.
//     * The field the user has to fill is initially containing the string
//     * `result`.
//     * If `wait_for_string` returns `DEFAULT`, `deflt` is copied in `result`.
//     * Legacy function, DO NOT USE.
//     */
//    bool ask_data (std::string& result, 
//      std::string const & question, std::string const & default_value,
//      string_validator_t& validator, std::string const & error_msg );


    /** General function to ask the user for data (when this data is not too
     * long).  
     *
     * @param result The string in which will be written the data
     *               entered by the user.  It is also the string written in the
     *               input area at the start of acquisition.
     * @param default_value Value that is copied in `result` if the users
     *                      accepts an empty string.  If this is set to "", the
     *                      behavior is thus the same as if there were no
     *                      default value.
     * @param comment Information to put in the {@link comment_area}.
     *        If non-empty, {@link comment_area} will be erased before and
     *        after data acquisition.
     * @param question Information to put in the {@link query_area}.
     * @param validator Object that validates if a strings conforms
     * @returns `true` if the user accepted the result, `false` if he cancelled
     * acquisition.
     *
     * */
    bool ask_data (
      std::string& result, 
      std::string const & default_value,
      sentence_t const& comment,
      sentence_t const& question, 
      abstract_validator_t& validator
    );
    
    /** Same as previous without a comment. */
    bool ask_data (
      std::string& result, 
      std::string const & default_value,
      sentence_t const& question, 
      abstract_validator_t& validator
    ) {
      static sentence_t comment;
      return ask_data(result, default_value, comment, question, validator);
    } 



    bool ask_long_string(std::string& result, std::string const & question);

    /* Ask a weight compatible with the context of {@link aut}. */
    bool ask_weight (std::string& result, 
                     std::string const & default_value, 
                     sentence_t const& question);
      
    /* Ask a label compatible with the ith-tape of the automaton. */
    bool ask_label (std::string& result, std::string const & def, 
                    unsigned i, sentence_t const& question);
    
    /* Ask a filename. No verification is done*/
    bool ask_filename (std::string& result, std::string const& default_value, 
                       sentence_t const& question);

    /* Ask aut name. No verification is done*/
    bool ask_state_name (std::string& result, std::string const& default_value, 
                         sentence_t const& question);

    /* Ask a state name. Verify that all char all in [a-zA-Z0-9] or '-' */
    bool ask_aut_name (std::string& result, std::string const& default_value, 
                       sentence_t const& question);

    /* Ask the name of an existing state */
    bool ask_state (std::string& result, std::string const& default_value, 
                    sentence_t const& question);    

    /* Ask an alphabet. */
    bool ask_alphabet (std::string& result, sentence_t const& question);

    /* Ask confirmation for an action */
    bool ask_confirmation(bool& b, bool default_value, 
                          sentence_t const& comment);

    token_t format_weight_of(dyn::weight_t tr, font_style_t fs = NO_STYLE);

    void initialize();
    void cleanup();

    void populate_alphabets();

    /* returns `true` if a name was changed. */
    bool ensure_suitable_state_names();
    void deal_with_underscore_and_epsilon();
    std::string fresh_state_name();

    void refresh_infos(bool clear_data = false);
    void refresh_states(bool clear_data = false);
    void refresh_transitions(bool clear_data = false);
    void refresh_tips();
    void refresh_message();

    sentence_t state_display_name(dyn::state_t s, bool b = false);

    void compute_area_geometry();
    void compute_formats();

    void action_refresh(bool clear_data = false);
    
    void action_meta_name();
    void action_meta_caption();
    void action_meta_promote();
    void action_allow_epsilon();
    void action_meta_addletter();

    void action_trans_add();
    void action_trans_delete();
    void action_trans_edit();

    void action_state_transfrom();
    void action_state_add();
    void action_state_rename();
    void action_state_delete();
    void action_state_initial();
    void action_state_final();
    
    void action_save();
    void action_quit();
    void action_display();

    void action_main_meta();
    void action_main_state();
    void action_main_trans();


    
    public:
    void print_message(std::string const &);
    void print_message(sentence_t);

    private:
    std::stack<editstate_t> editstate_stack;
    editstate_t editstate();
    void enter_state(editstate_t);
    void leave_state();


    void apply(const transition_pack_t& tr);
    bool edit_tr_pack(transition_pack_t& tr, int off = 0) ; 

   public:
    /* If `filename` is empty, assumed as "no filename".*/
    editter_t(dyn::automaton_t a, 
              io_format_t output_format, 
              std::string filename = ""); 
    void run();

    ~editter_t();

    /* to implement interface `notifiable_t` */
    virtual void notify(void* notifier);

  }; //end of class editter_t



}} //end of namespaces awali::cora and awali

#endif // CORA_EDIT_HH
