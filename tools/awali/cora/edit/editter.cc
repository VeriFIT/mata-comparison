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


#ifndef CORA_EDIT_EDITTER_CC
#define CORA_EDIT_EDITTER_CC

#include <sstream>
#include <unistd.h>
#include <unordered_map>

#include <cora/edit/queued_ostream.hh>
#include <cora/edit/editter.hh>
#include <cora/edit/messages.hh>

using namespace awali::cora::editter;

namespace awali {
namespace cora {


void
editter_t::clear_state_data()
{
  states= aut->states();
  state_area.set_size(states.size());
  state_data.clear();
  state_data = std::vector<sentence_t>(states.size());
}


sentence_t&
editter_t::get_state_data(unsigned int i)
{
  dyn::state_t s = states[i];
  sentence_t& got = state_data[i];
  if (got.size() == 0)
    got = state_display_name(s);
  return got;
}


void
editter_t::clear_transition_data()
{
  transitions= aut->transitions();
  trans_area.set_size(transitions.size());
  transition_data.clear();
  transition_data = std::vector<sentence_t>(transitions.size());
}


sentence_t&
editter_t::get_transition_data(unsigned int i)
{
  dyn::transition_t tr = transitions[i];
  sentence_t& got = transition_data[i];
  if (got.size() == 0) {
    dyn::state_t src = aut->src_of(tr);
    got << aut->get_state_name(src);
    got << tr_sta;
    dyn::weight_t w = aut->weight_of(tr);
    got << format_weight_of(aut->weight_of(tr));
    sentence_t labels = label_format;
    if (is_transducer)
      for(std::string const& l: tdc.label_of(tr))
        labels /=  (l == ""? eps_repr : l);
    else {
      if (aut->is_eps_transition(tr))
        labels /= eps_repr;
      else
        labels /= ctx->label_to_string(aut->label_of(tr));
    }
    got << labels;
    got << tr_end;
    dyn::state_t dst = aut->dst_of(tr);
    got << token_t(aut->get_state_name(dst));
  }
  return got;
}


void
editter_t::populate_alphabets()
{
  alphabets.clear();
  if (is_transducer) {
    for (std::vector<char> const& alphabet : tdc.alphabets()) {
      std::vector<std::string> alph = {};
      for (char const& c : alphabet)
        alph.push_back(std::string(1, c));
      alphabets.push_back(alph);
    }
  }
  else {
    std::vector<std::string> alph;
    for (dyn::any_t const& a : aut->alphabet())
      alph.push_back(
          ctx->label_to_string(a)
      );
    alphabets.push_back(alph);
  }
  deal_with_underscore_and_epsilon();
  if (is_transducer || has_epsilon)
    for (std::vector<std::string>& v: alphabets)
      v.push_back(eps_repr);
}


editter_t::editter_t(dyn::automaton_t a, io_format_t of, std::string f)
  : aut(a), tdc(a), 
    message_queue(*this), 
    output_format(of),
    tip_handler(aut)
{
  ctx = a->get_context();
  states = aut->states();
  transitions = aut->transitions();
  is_transducer = aut->is_transducer();
  is_int_automaton = aut->is_int_automaton();
  has_epsilon = is_transducer || aut->is_eps_allowed();
  weightset_name = ctx->weightset_name();
  is_over_B = weightset_name == "B";
  if (f != "") {
    filename = f;
    is_saved = true;
    ever_saved = true;
  }
  else {
    filename = "example."+default_extension_of(output_format);
    is_saved = false;
    ever_saved = false;
  }
  //populate_alphabets(false);
  // tips
}


void
editter_t::run()
{
  dyn::set_warning_stream(this->message_queue);
  initialize();
  populate_alphabets();
  compute_formats();
  ensure_suitable_state_names();

  action_refresh(true);
  while (continue_edit) {
    if (require_refresh)
      action_refresh();
    if (editstate() == MAIN_TRANS)
      action_main_trans();
    else {
      if (editstate() == MAIN_STATE)
        action_main_state();
      else
        action_main_meta();
    }
  }
  cleanup();
}


editter_t::~editter_t()
{
  dyn::set_warning_stream(std::cerr);
}


void
editter_t::initialize()
{
  initscr();    // Start curses mode
  cbreak();

  // Additional escape sequences
  // The constants are defined in `enums.hh`
  define_key("\033Op", KEY_NUMPAD_0);  // numpad 0
  define_key("\033Oq", KEY_NUMPAD_1);  // numpad 1
  define_key("\033Or", KEY_NUMPAD_2);  // numpad 2
  define_key("\033Os", KEY_NUMPAD_3);  // numpad 3
  define_key("\033Ot", KEY_NUMPAD_4);  // numpad 4
  define_key("\033Ou", KEY_NUMPAD_5);  // numpad 5
  define_key("\033Ov", KEY_NUMPAD_6);  // numpad 6
  define_key("\033Ow", KEY_NUMPAD_7);  // numpad 7
  define_key("\033Ox", KEY_NUMPAD_8);  // numpad 8
  define_key("\033Oy", KEY_NUMPAD_9);  // numpad 9
  define_key("\033OM", KEY_NUMPAD_ENTER);  // numpad Enter


  keypad(stdscr, TRUE); // Enable arrow keys
  curs_set(0);
  set_escdelay(50);
  noecho();
  compute_area_geometry();
  clear_transition_data();
  clear_state_data();
  require_refresh=true; // Draw window
}


void
editter_t::compute_formats()
{
  tr_sta = " %-%- ";
  tr_end = " %-%-%> ";
  ini_arr_format = "%-%s%-%> ";
  fin_arr_format = " %-%s%-%>";

  sentence_t new_label_format;
  if (is_transducer)
    new_label_format << tdc_lbl_sta;
  bool b = false;
  for (unsigned i = 0; i < alphabets.size(); i++) {
    if (b)
      new_label_format << tdc_lbl_sep;
    new_label_format << token_t("%s");
    b = true;
  }
  if (is_transducer)
    new_label_format << tdc_lbl_end;
  label_format = new_label_format;

  sentence_t new_tr_format;
  new_tr_format << "%s" << tr_sta;
  if (!is_over_B)
    new_tr_format << weight_sta << "%s" << weight_end;
  new_tr_format << label_format << tr_end << "%s" ;
  tr_format = new_tr_format;
}


void
editter_t::cleanup()
{
  endwin();
}


/* Debug function */
void
show_area(area_t area)
{
  std::string blank = std::string(area.w, '#');
  attrset(A_NORMAL);
  for (int x = 0; x<area.h; x++)
    mvprintw(area.r+x, area.c, "%s", blank.c_str());
  mvprintw( area.r+(area.h-1)/2,
            area.c+(area.w-1)/2-4,
            "(%d,%d)(%d,%d)", area.r, area.c, area.h, area.w);
}


void
editter_t::compute_area_geometry()
{
  screen.c = screen.r = 0;
  getmaxyx(stdscr, screen.h, screen.w);

  infos_area.r = state_area.r = trans_area.r = screen.r = 0;

  trans_area.w = (screen.w-2)/2;
  state_area.w = (screen.w-trans_area.w-1)/2;
  infos_area.w = screen.w - trans_area.w - state_area.w - 2;

  infos_area.c = screen.c;
  state_area.c = infos_area.c+infos_area.w+1;
  trans_area.c = state_area.c+ state_area.w+1;

  tips_area
    = { screen.h-2,     0,           2, screen.w };
  query_area
    = { tips_area.r-2,  0,           2, (screen.w*2-1)/3};
  comment_area
    = { query_area.r-2, 0,           2, query_area.w};
  message_area
  = { comment_area.r,              comment_area.w+1,
      comment_area.h+query_area.h, screen.w-comment_area.w-1
    };

  infos_area.h = state_area.h = trans_area.h = comment_area.r-infos_area.r;

  long_string_area.w = query_area.w;
  long_string_area.h = 5;
  long_string_area.r = query_area.r-4;
  long_string_area.c = 0;

}


void
editter_t::action_refresh(bool force)
{
  clear();
  compute_area_geometry();
  attron(DIM);
  for (int x = 0; x< trans_area.h; x++)
    mvaddch(trans_area.r+x, trans_area.c-1, ACS_VLINE);
  for (int x = 0; x< trans_area.h; x++)
    mvaddch(state_area.r+x, state_area.c-1, ACS_VLINE);
  attroff(DIM);

  refresh_message();
  refresh_infos(force);
  refresh_states(force);
  refresh_transitions(force);
  refresh_tips();
  refresh();
  require_refresh=false;
}


token_t
editter_t::format_weight_of(dyn::weight_t w, font_style_t fs)
{
  if ((is_over_B) || (w == ctx->weight_one() && !ctx->show_one()))
    return token_t("", fs);
  token_t res(weight_format);
  res.font_style = fs;
  return (res /= ctx->weight_to_string(w));
}


sentence_t
editter_t::state_display_name(dyn::state_t s, bool b)
{
  std::string name = aut->get_state_name(s);
  sentence_t res;
  if (aut->is_initial(s)) {
    token_t ini = ini_arr_format;
    res << (ini /= format_weight_of(aut->get_initial_weight(s)));
  }
  else
    res << "";
  res << token_t(aut->get_state_name(s), b?BOLD:NO_STYLE);
  if (aut->is_final(s)) {
    token_t fin = fin_arr_format;
    res << (fin /= format_weight_of(aut->get_final_weight(s)));
  }
  else
    res << "";
  return res;
}


void
editter_t::refresh_message()
{
  using action_t = queued_ostream_t::action_t;
  int x=0, y=0;
  action_t action = message_queue.expected_action();
  switch (action) {
    case action_t::NOTHING:
      return;
    case action_t::ERASE: {
      getyx(stdscr, y, x);
      message_queue.pop();
      message_area.erase();
      break;
    }
    case action_t::REPLACE:
      message_queue.pop();
    // fallthrough
    case action_t::PRINT: {
      getyx(stdscr, y, x);
      message_area.erase(STANDOUT);
      message_area.print_left(message_queue.front());

      size_t n_waiting = message_queue.size() - 1;
      if (n_waiting > 0) {
        std::string msg =  " [" + std::to_string (n_waiting) + " more]";
        size_t x2 = message_area.c + message_area.w - msg.size();
        size_t y2 = message_area.r + message_area.h - 1;
        print_at(STANDOUT, y2, x2, msg);
      }
      break;
    }
  }
  refresh();
  move(y, x);
}


void
editter_t::refresh_states(bool clear_data)
{
  if (clear_data)
    clear_state_data();
  state_area.erase();
  bool selected = editstate() == MAIN_STATE;
  state_area.print_title("States", selected,
                         (selected && states.size() == 0) ? STANDOUT
                         : NO_STYLE);
  if (states.size() == 0)
    return;
  long src = (transitions.size() >0)?aut->src_of(current_transition()):-1;
  long dst = (transitions.size() >0)?aut->dst_of(current_transition()):-1;

  unsigned size = 0;
  for(int i = state_area.start; i <= state_area.end(); i++)
    size += (++(get_state_data(i).cbegin()))->size();
  size /= (state_area.end()-state_area.start+1);

  state_area.set_size(states.size());
  for (int i = state_area.start; i <= state_area.end(); i++) {
    dyn::state_t s = states[i];
    bool b = ((long)s == src) || ((long)s == dst);
    sentence_t& tokens
      = get_state_data(i);
    if ((editstate() == MAIN_TRANS) && b)
      (++(tokens.begin()))->font_style = BOLD;

    std::pair<int, int> offsets
      = tokens.align_before(1, (state_area.w-2)/2-(size+1)/2, state_area.w-2);
    state_area.print_row(
        editstate() == MAIN_STATE,
        i,
        tokens,
        offsets.first,
        offsets.second
    );
    (++(tokens.begin()))->font_style = NO_STYLE;
  }
}


void
editter_t::refresh_transitions(bool clear_data)
{
  if (clear_data)
    clear_transition_data();
  trans_area.erase();
  bool selected = editstate() == MAIN_TRANS;
  trans_area.print_title("Transitions", selected,
                         selected &&  (transitions.size() == 0) ? STANDOUT
                         : NO_STYLE);
  for (int i = trans_area.start; i <= trans_area.end(); i++) {
    sentence_t& ss = get_transition_data(i);
    if (editstate() == MAIN_STATE) {
      if (aut->src_of(transitions[i]) == current_state())
        ss.front().font_style = BOLD;
      if (aut->dst_of(transitions[i]) == current_state())
        ss.back().font_style = BOLD;
    }
    std::pair<int, int> offsets 
      = ss.align_before(3, (trans_area.w-2)/2-2, trans_area.w-2);
    trans_area.print_row( editstate() == MAIN_TRANS, i, ss, offsets.first,
                          offsets.second);

    if (editstate() == MAIN_STATE) {
      ss.front().font_style = NO_STYLE;
      ss.back().font_style = NO_STYLE;
    }
  }
}


void
editter_t::refresh_tips()
{
  tip_list_t const& cur_tips = tip_handler.for_state(editstate());
  tips_area.erase();
  size_t max = cur_tips.size();
  size_t i = max;
  size_t j = 1;
  int col_width = screen.w/5;
  int x = 3 + (screen.w-col_width*5)/2+col_width*4;
  size_t prev_key_size = 2;
  while (i-->0) {
    if (cur_tips[i]->available_for(aut)) {
      print_at(true,
               tips_area.r+j,
               x-cur_tips[i]->key.size(),
               cur_tips[i]->key);
      print_at(false, tips_area.r+j, x+1,
               cur_tips[i]->hint(col_width-prev_key_size-3) );
      prev_key_size = cur_tips[i]->key.size();
    }
    else
      prev_key_size = 0;
    x-= col_width;
    if ( x < 0 ) {
      prev_key_size = 2;
      x = 3 + (screen.w-col_width*5)/2+col_width*4;
      j--;
    }
  }
}


std::string
alph_displayname(std::vector<std::string> alph)
{
  std::stringstream ss;
  std::string sep = "{";
  for(std::string const& str : alph) {
    ss << sep;
    ss << str;
    sep = ",";
  }
  ss << "}";
  return ss.str();
}


void
editter_t::refresh_infos(bool force)
{
  static bool init = false;
  static sentence_t file_line;
  static sentence_t weightset_line;
  static std::vector<sentence_t> label_lines;
  static sentence_t eps_line;
  static bool vert_compress = false;
  if (!init || force) {
    init = true;
    file_line = sentence_t("", NO_STYLE, " ");
    weightset_line = sentence_t("", NO_STYLE, " ");
    label_lines.clear();
    eps_line = sentence_t("NB: label \""+eps_repr+"\" represents epsilon, "
                          +"the empty word");
    eps_line.front().font_style = BOLD;
    

    file_line = sentence_t("", NO_STYLE, " ");
    file_line << token_t{"File:", BOLD};
    if (!ever_saved)
      file_line << "<Never Saved>";
    else {
      file_line << filename;
      if (!is_saved)
        file_line <<  "<Unsaved changes>";
    }
    if (weightset_line.size() == 0)
      weightset_line << token_t("Weightset:", BOLD) << weightset_name;


    if (label_lines.size() == 0) {
      if (is_transducer)
        label_lines.push_back (sentence_t("Labels:", BOLD));
      int n_tape = 0;
      for (std::vector<std::string> const& alph : alphabets) {
        sentence_t label("", NO_STYLE, "");
        if (is_transducer)
          label << "- tape "+std::to_string(n_tape) +": ";
        else {
          if (is_int_automaton)
            label << token_t("Labels (int): ", BOLD);
          else
            label << token_t("Labels: ", BOLD);
        }
        label << "{";
        for(unsigned i = 0; i< alph.size(); i++)
          label << std::string(alph[i]) + (i != alph.size()-1?",":"");
        label << "}";
        label_lines.push_back(label);
        n_tape++;
      }
    }
  }
  
  infos_area.erase();
  
  bool selected = editstate() == MAIN_META;
  infos_area.print_title("Automaton", selected, selected?STANDOUT:NO_STYLE);
  
  int v_sep = (vert_compress)?1:2;
  int n_empty_line = 0;

  size_t y = v_sep, x = 0;
  n_empty_line++;

  infos_area.print_left(file_line, y, x);
  infos_area.new_line(y, x, v_sep);
  n_empty_line++;

  infos_area.print_left(weightset_line, y, x);
  infos_area.new_line(y, x, v_sep);
  n_empty_line++;

  for (sentence_t const& line: label_lines) {
    infos_area.print_left(line, y, x);
    infos_area.new_line(y, x, 1);
  }

  infos_area.new_line(y, x, v_sep-1);
  n_empty_line++;
  
  if (has_epsilon) {
    infos_area.print_left(eps_line, y, x);
    infos_area.new_line(y, x, v_sep);
    n_empty_line++;
  }

  sentence_t name_line("", NO_STYLE, " ");
  name_line << token_t{"Name:", BOLD};
  std::string name = aut->get_name();
  if (!name.compare(""))
    name = "<None>";
  name_line << name;
  infos_area.print_left(name_line, y, x);

  infos_area.new_line(y, x, v_sep);
  n_empty_line++;

  sentence_t caption_line("", NO_STYLE, " ");
  caption_line << token_t{"Caption:", BOLD};
  std::string caption = aut->get_desc();
  if (!caption.compare(""))
    caption = "<None>";
  caption_line << sentence_t(caption);
  infos_area.print_left(caption_line, y, x);


  if (vert_compress) {
    if ((long)y+n_empty_line< infos_area.h) {
      vert_compress = false;
      refresh_infos();
    }
  }
  else
    if ((long)y>= (long)infos_area.h) {
      vert_compress = true;
      refresh_infos();
      comment_area.erase();
      query_area.erase();
    }

  refresh();
}

input_kind_t
editter_t::wait_for_char(char& result)
{
  while (true) {
    // The function directly returns from within this loop.
    // This loop is here in case `getch()` times out and we have to refresh the
    // window.  Using a recursive call in that case would cause a segmentation
    // fault due to stack overflow in the case where an `edit` window was left
    // hanging long enough.
    refresh_message();
    timeout(50);
    int c=getch();
    switch (c) {
      case ERR: // This code means getch() `timed` out
        break;
      case KEY_UP :
        return UP;
      case KEY_DOWN :
        return DOWN;
      case KEY_LEFT :
        return LEFT ;
      case KEY_RIGHT :
        return RIGHT;
      case '\t':
        return TAB;
      case KEY_NUMPAD_ENTER:
      case KEY_ENTER:
      case '\n':
      case '\r':
        return NEXT;
      case KEY_BACKSPACE : // Backspace on Linux
      case 127 :           // Backspace on MacOS
        return BACKSPACE;
      case KEY_RESIZE:
        require_refresh = true;
        clear_transition_data();
        clear_state_data();
        timeout(50);
        while (KEY_RESIZE == (c = getch()));
        if (c != ERR)
          ungetch(c);
        timeout(-1);
        return OTHER;
      case KEY_DC : //Suppr
        //         case KEY_CANCEL :
        //         case KEY_UNDO :
        return DELETE;
      case 27: //ESC key 
      {
        // Verifying that ESC key is hopefully alone.
        int d;
        timeout(0);
        if ( (d=getch()) != ERR ) {
          std::vector<std::string> escaped_sequence = {"[27,"};
          do{
            escaped_sequence.push_back(std::to_string(d)+",");
          } while ( (d=getch()) != ERR );
          escaped_sequence.back().back() = ']';
          sentence_t tokens
            ("Probable unrecognised escape sequence: ");
          for (std::string const& s: escaped_sequence)
            tokens << s;
          print_message(tokens);
          break; // Waiting again for char
        }
        else 
          return BACK;
      }
      case 24: // char ^X, ctrl+x on MacOs/Linux
        return BACK;
      case KEY_NUMPAD_0:
      case KEY_NUMPAD_1:
      case KEY_NUMPAD_2:
      case KEY_NUMPAD_3:
      case KEY_NUMPAD_4:
      case KEY_NUMPAD_5:
      case KEY_NUMPAD_6:
      case KEY_NUMPAD_7:
      case KEY_NUMPAD_8:
      case KEY_NUMPAD_9:
        result = c-KEY_NUMPAD_0+'0';
        return NORMAL;
      default:
        if (c >= 32 && c <= 126) {
          result = c;
          return NORMAL;
        }
        else
          print_message( sentence_t(message::unknown_keycode)
                         /= std::to_string(c) );
    }
  }
}

answer_kind_t
editter_t::wait_for_string(std::string& result, size_t width)
{
  //     bool no_input = true;
  if (width == 0)
    width = query_area.w;
  int x, y;
  getyx(stdscr, y, x);
  char c;
  input_kind_t rm;
  bool repeat = true;
  int i = result.size();
  int j = i;
  while (repeat) {
    move(y, x);
    query_area.overwrite_str(result, STANDOUT);
    if (query_area.holdscur())
      query_area.overwrite_ch(' ', STANDOUT);
    move(y+(x+j)/width, (x+j)%width);
    curs_set(2);
    bool modification = false;
    while(!modification && repeat) {
      if (query_area.holdscur()) {
        query_area.overwrite_ch(inch(), UNDERLINE);
        query_area.move_left();
      }
      refresh();
      rm = wait_for_char(c);
      switch (rm) {
        case NORMAL:
          result.insert(j, 1, c);
          modification = true;
          i++;
          j++;
          break;
        case LEFT:
          if (j> 0) {
            if (query_area.holdscur()) {
              query_area.overwrite_ch(inch() & ~UNDERLINE);
              query_area.move_left();
            } 
            query_area.move_left();
            j--;
          }
          break;
        case RIGHT:
          if (j<i) {
            if (query_area.holdscur())
              query_area.overwrite_ch(inch() & ~UNDERLINE);
            else
              query_area.move_right();
            j++;
          }
          break;
        case BACKSPACE:
          if (j>0)
            j--;
        // fall through
        case DELETE:
          if ((i > 0) && (j < i)) {
            query_area.overwrite_ch(' ', STANDOUT);
            result.erase(j, 1);
            modification=true;
            i--;
          }
          break;
        case TAB:
        case NEXT:
          repeat = false;
          break;
        case BACK:
          curs_set(0);
          query_area.erase();
          return ABORT;
        case UP:
        case DOWN:
        case OTHER:
          ;
      }
    }
  }
  curs_set(0);
  query_area.erase();
  if (result == "")
    return DEFAULT;
  else
    return INPUT;
}


void
editter_t::action_main_meta()
{
  enter_state(MAIN_META);
  refresh_infos();
  refresh_transitions();
  refresh_states();
  char c;
  switch(wait_for_char(c)) {
    case NORMAL:
      switch(c) {
      case 'q' : action_quit(); break;
      case 's' : action_save(); break;
      case 'w' : action_display(); break;
      case 'p' : action_meta_promote(); break;
      case 'n' : action_meta_name(); break;
      case 'c' : action_meta_caption(); break;
      case 'e' :
        if (!is_transducer) {
          action_allow_epsilon();
          break;
        }
      // fall through
      case 'l' :
        if (!is_transducer) {
          action_meta_addletter();
          break;
        }
      // fall through
      default:
        print_message(sentence_t(message::unknown_action) /= c);
      }
      break;
    case LEFT:
      return;
    case TAB:
    case RIGHT:
      leave_state();
      action_main_state();
      return;
    case UP:
    case DOWN:
    case BACKSPACE:
    case DELETE:
    case BACK:
    case NEXT:
    case OTHER: ; // other char are ignored
  }
}

void
editter_t::action_main_trans()
{
  enter_state(MAIN_TRANS);
  refresh_infos();
  refresh_transitions();
  refresh_states();
  char c;
  switch(wait_for_char(c)) {
    case NORMAL: {
      switch(c) {
        case 'q' : action_quit(); break;
        case 's' : action_save(); break;
        case 'w' : action_display(); break;
        case 'a' : action_trans_add(); break;
        case 'd' : action_trans_delete(); break;
        case 'e' : action_trans_edit(); break;
        default:
          print_message(sentence_t(message::unknown_action) /= c);
      }
      break;
    }
    case BACKSPACE:
    case DELETE:
      action_trans_delete();
      break;
    case UP:
      trans_area.action_up();
      leave_state();
      action_main_trans();
      return;
    case TAB:
      leave_state();
      action_main_meta();
      return;
    case LEFT:
      leave_state();
      action_main_state();
      return;
    case DOWN:
      trans_area.action_down();
      leave_state();
      action_main_trans();
      return;
    case RIGHT:
      return;
    case BACK:
    case NEXT:
    case OTHER: ; // other char are ignored
  }
}

void
editter_t::action_main_state()
{
  enter_state(MAIN_STATE);
  refresh_infos();
  refresh_states();
  refresh_transitions();
  char c;
  switch(wait_for_char(c)) {
    case NORMAL:
      switch(c) {
        case 'q' : action_quit(); return;
        case 's' : action_save(); return;
        case 'w' : action_display(); return;
        case 'a' : action_state_add(); return;
        case 'd' : break;
        case 'n' : action_state_rename(); return;
        case 'i' : action_state_initial(); return;
        case 'f' : action_state_final(); return;
        case 't' : action_state_transfrom(); return;
        default: print_message(sentence_t(message::unknown_action) /= c);
      }
      if (c != 'd')
        break;
    // fall through
    // if key 'd' was pressed
    case BACKSPACE:
    case DELETE:
      action_state_delete();
      return;
    case UP:
      state_area.action_up();
      refresh_states();
      break;
    case TAB:
    case RIGHT:
      leave_state();
      action_main_trans();
      return;
    case DOWN:
      state_area.action_down();
      refresh_states();
      break;
    case LEFT:
      leave_state();
      action_main_meta();
      return;
    case BACK:
    case NEXT:
    case OTHER: ; // other char are ignored
  }
}


void
editter_t::action_allow_epsilon()
{
  if (is_transducer || has_epsilon)
    return;

  if ( !is_int_automaton) {
    for (std::string const& str : alphabets[0])
      if (!str.compare(eps_repr)) {
        print_message("Char '" + eps_repr
                      + "' in alphabet: cannot allow eps. transitions.");
        return;
      }
  }

  bool b = true;
  if (ask_confirmation(b,
                       true,
                       std::string("Allowing epsilon transitions requires ")
                       + "a full copy of the automaton.  Confirm"
                       ))
    if (b) {
      automaton_t new_aut = allow_eps_transition(aut, {IN_PLACE=false});
      dyn::internal::rename_from_origin_inplace(new_aut, aut);
      new_aut->set_desc(aut->get_desc());
      new_aut->set_name(aut->get_name());
      aut = new_aut;
      has_epsilon = true;
      populate_alphabets();
      tip_handler.update_alternatives(aut);
      notify_modified();
      action_refresh(true);
    }
}


void
editter_t::action_meta_name ()
{
  std::string old = aut->get_name();
  std::string result;
  static sentence_t const question("New name ?");
  if (ask_aut_name(result, old, question))
    if (result != old)  {
      aut->set_name(result);
      is_saved = false;
      refresh_infos();
    }
}


void
editter_t::action_meta_caption ()
{
  std::string old = aut->get_desc();
  std::string result = old;
  if (ask_long_string(result, "Edit caption:")) {
    aut->set_desc(result);
    refresh_infos();
  }
  action_refresh();
}


void
editter_t::action_trans_delete()
{
  static sentence_t const comment("Deleting this transition cannot be undone.");
  if (!transitions.empty()) {
    bool b = true;
    if (ask_confirmation(b, true, comment) &&b) {
      aut->del_transition(current_transition());
      notify_modified();
      clear_transition_data();
    }
  }
  else
    print_message("No transition to delete.");
}


void
editter_t::apply(transition_pack_t const& tr)
{
  // If transition exists, we delete it.
  if (aut->has_transition(tr.id))
    aut->del_transition(tr.id);
  // Then we create the one that replaces it.
  if(is_transducer) {
    std::vector<std::string> labs;
    for (std::string const& label : tr.labels)
      labs.push_back( label == eps_repr ? "" : label );
    tdc.add_transition(
        aut->get_state_by_name(tr.src),
        aut->get_state_by_name(tr.dst),
        labs,
        ctx->get_weight(tr.weight)     );
  }
  else {
    if ((tr.labels[0] == eps_repr) && aut->is_eps_allowed())
      aut->add_eps_transition(aut->get_state_by_name(tr.src),
                              aut->get_state_by_name(tr.dst),
                              ctx->get_weight(tr.weight)     );
    else
      aut->add_transition(aut->get_state_by_name(tr.src),
                          aut->get_state_by_name(tr.dst),
                          ctx->get_label(tr.labels[0]),
                          ctx->get_weight(tr.weight)     );
  }
  notify_modified();
  clear_transition_data();
}


void
editter_t::action_trans_add()
{
  if (states.size() == 0) {
    print_message("Cannot add transition: no state in the automaton.");
    return;
  }
  transition_pack_t pack(aut);
  if(edit_tr_pack(pack))
    apply(pack);
}


void
editter_t::action_trans_edit()
{
  if (transitions.size() == 0) {
    print_message("No transition to edit.");
    return;
  }
  transition_pack_t pack(aut, current_transition(), eps_repr);
  if(edit_tr_pack(pack))
    apply(pack);
}


void
editter_t::action_state_transfrom()
{
  if (states.size() == 0) {
    print_message("Cannot add transition: no state in the automaton.");
    return;
  }
  transition_pack_t pack(aut);
  pack.src = aut->get_state_name(current_state());
  if(edit_tr_pack(pack, is_over_B?2:1))
    apply(pack);
}


bool
editter_t::edit_tr_pack(transition_pack_t& tr, int off)
{
  enter_state(TRANSEDIT);
  comment_area.erase();
  std::string question = (tr.id == -1) ? "New trans.: ": "Edit trans.: ";
  char c;
  bool repeat = true;
  int i = off;
  int maxi = tr.labels.size()+4;

  sentence_t format;
  format << token_t(question, BOLD) << " " << tr_format << "    " << "%s";
  while (repeat) {
    sentence_t to_print = format;
    int j = i;
    to_print /= token_t( (tr.src == transition_pack_t::DEFAULT ?"%.":tr.src),
                         j--==0);
    if (!is_over_B)
      to_print /= token_t(tr.weight, j==0);
    j--;
    for (std::string const& label: tr.labels)
      to_print /= token_t(
                      label==transition_pack_t::DEFAULT?"%.":label,
                      j--==0
                  );
    to_print /= token_t(
                    tr.dst == transition_pack_t::DEFAULT?"%.":tr.dst,
                    j--==0
                );
    to_print /= token_t(
                    " OK ",
                    j--==0
                );
    size_t y = 0, x =0;
    comment_area.print_left(to_print, y, x);
    refresh();
    std::string result;
    switch(wait_for_char(c)) {
    case BACK:
      leave_state();
      comment_area.erase();
      return false;
    case NORMAL:
      result += c;
    // fall through
    case NEXT: {
      bool success = false;
      if (i == 0) {
        success = ask_state(result, 
                            tr.src == transition_pack_t::DEFAULT?"":tr.src, 
                            sentence_t("Source state?"));
        if(success)
          tr.src = result;
      }
      else if (i == maxi-2) {
          success = ask_state(result,
                              tr.dst == transition_pack_t::DEFAULT?"":tr.dst, 
                              sentence_t("Destination state?"));
          if (success)
            tr.dst = result;
      }
      else if (i == 1) {
            success = ask_weight(result, tr.weight, 
                                 sentence_t("Transition weight?"));
            if (success)
              tr.weight = result;
      }
      else if (i == maxi-1) { // select "ok"
        if (tr.src == transition_pack_t::DEFAULT)
          print_message("Please enter a source state.");
        else if (tr.dst == transition_pack_t::DEFAULT)
            print_message("Please enter a destination state.");
        else {
          bool b = false;
          int tape_n = 0;
          for (std::string const& label : tr.labels) {
            b |= (label == transition_pack_t::DEFAULT);
            if (b)
              print_message(
                  sentence_t("Please enter label for tape %s.") /=  i
              );
            tape_n++;
          }
          if (!b) {
            if (ctx->weight_to_string(ctx->weight_zero()) ==  tr.weight) {
              bool b2 = ask_confirmation(b, true, sentence_t("Given weight is the neutral element for addition."));
              b &= b2;
              b = !b;
              i = 1;
            }
            if (!b) {
              comment_area.erase();
              leave_state();
              return true;
            }
          }
        }
      }
      else { //label
        int k = i-2;
        std::string question2 = 
          is_transducer ? "New label for tape "+std::to_string(k)+"?"
                        : "New label?";
        success = ask_label(result,
                            tr.labels[k] == transition_pack_t::DEFAULT
                              ? "" : tr.labels[k],
                            k, 
                            question2);
        if (success)
          tr.labels[k] = result;
      }
      query_area.erase();
      if (!success)
        break;
    }
    // Fall through
    // to RIGHT if success
    case RIGHT:
      if (i<maxi-1)
        i++;
      if (i == 1 && is_over_B)
        i++;
      break;
    case TAB:
      if (i<maxi-1) {
        i++;
        if (i == 1 && is_over_B)
          i++;
      }
      else
        i=0;
      break;
    case LEFT:
      if (i>0)
        i--;
      if (i == 1 && is_over_B)
        i--;
      break;
    /* ignored input */
    case UP:
    case DOWN:
    case BACKSPACE:
    case DELETE:
    case OTHER:
      break;
    }
  }
  comment_area.erase();
  leave_state();
  return false;
}


std::string
editter_t::fresh_state_name()
{
  static unsigned id = 0;
  std::string name;
  do {
    name = "s"+std::to_string(id++);
  } while (aut->has_state(aut->get_state_by_name(name)));
  return name;
}


void
editter_t::deal_with_underscore_and_epsilon()
{ 
  static sentence_t message("Given string, %s, is already a label.");
  runtime_validator_t<std::vector<std::vector<std::string>>> validator(
    [] (std::vector<std::vector<std::string>> const& alphabets, 
        std::string const& str) 
      {
        for (std::vector<std::string> const& alph : alphabets )
          for (std::string const& s : alph)
            if (s == str)
              return false;
        return true;
      },
    this->alphabets,
    message
  );
  if (!validator.validate(eps_repr)) {
    sentence_t comment("Representation of epsilon, %s, is also a label"
                                    " in this automaton." );
    static sentence_t const question("Enter new epsilon representation.");
    std::string result = "";
    while (true) {
      if (ask_data(result, 
                   "", 
                   comment /= this->eps_repr ,
                   question, 
                   validator))
      {
        eps_repr = result;
        comment_area.erase();
        return;
      }
      print_message("Cannot cancel this action.");
    }
  }
  return;
}


bool
editter_t::ensure_suitable_state_names() 
{
  bool some_name_changed = false;
  for(state_t s : states) {
    std::string name = aut->get_state_name(s);
    if ( s != aut->get_state_by_name(name) || name == "" ) {
      name = this->fresh_state_name();
      aut->set_state_name(s, name);
      some_name_changed = true;
    }
  }
  if (some_name_changed) {
    print_message("Some state names were changed to ensure unicity.");
    notify_modified();
    clear_state_data();
    clear_transition_data();
  }
  return some_name_changed;
}


void
editter_t::action_state_add()
{
  std::string name;
  aut->add_state(this->fresh_state_name());
  notify_modified();
  refresh_states(true);
}


void
editter_t::action_state_rename()
{
  if (states.size() == 0) {
    print_message("No state to rename");
    return;
  }
  static sentence_t const question("New state name?"); 
  std::string old = aut->get_state_name(current_state());
  std::string result;
  if (ask_state_name(result, old, question))
    if (result != old)  {
      aut->set_state_name(current_state(), result);
      notify_modified();
      clear_transition_data();
    }
  clear_transition_data();
  clear_state_data();
}


bool
editter_t::ask_long_string(std::string& result, std::string const& question)
{
  enter_state(ASK);
  curs_set(2);
  int x= long_string_area.c,
      y= long_string_area.r;
  int w = long_string_area.w;
  char c;
  input_kind_t rm;
  bool repeat = true;
  int i = result.size();
  int j = i;
  while (repeat) {
    attron(A_STANDOUT);
    long_string_area.erase();
    attroff(A_STANDOUT);
    move(y, x);
    print(true, question);
    {
      sentence_t  t;
      t << token_t{result, STANDOUT};
      size_t xoff = 0;
      size_t yoff = 1;
      long_string_area.print_left(
          t, yoff, xoff
      );
    }
    move(y+1+(j/w), x+(j%w));
    refresh();
    rm = wait_for_char(c);
    switch (rm) {
    case NORMAL:
      addch(c);
      result.insert(j, 1, c);
      i++;
      j++;
      break;
    case LEFT:
      if (j> 0)
        j--;
      break;
    case RIGHT:
      if (j<i)
        j++;
      break;
    case BACKSPACE:
      if (j>0)
        j--;
    // fall through
    case DELETE:
      if ((i > 0) && (j < i)) {
        result.erase(j, 1);
        i--;
      }
      break;
    case TAB:
    case NEXT:
      repeat = false;
      leave_state();
      break;
    case BACK:
      curs_set(0);
      query_area.erase();
      leave_state();
      return false;
    case UP:
    case DOWN:
    case OTHER:
      ;
    }
  }
  curs_set(0);
  long_string_area.erase();
  return true;
}


void
editter_t::action_state_delete()
{
  if (states.size() == 0) {
    print_message("No state to delete.");
    return;
  }
  dyn::state_t s = current_state();
  size_t n = aut->in(s).size() + aut->out(s).size();

  if (n>= 1) {
    std::stringstream ss;
    ss << "Deleting state " << current_state_name() << " will also delete "
       << n << " transitions.";
    sentence_t comment(ss.str());
    bool b;
    if (ask_confirmation(b, true, comment))
      if (b) {
        aut->del_state(current_state());
        notify_modified();
        clear_transition_data();
      }
  }
  else {
    aut->del_state(current_state());
    notify_modified();
  }
  comment_area.erase();
  query_area.erase();
  refresh_states(true);
  refresh_transitions(true);
}


void
editter_t::action_state_initial()
{
  if (states.size() == 0)
    return;
  dyn::state_t s = current_state();
  if (weightset_name == "B") {
    if (aut->is_initial(s))
      aut->unset_initial(s);
    else
      aut->set_initial(s);
    notify_modified();
  }
  else {
    static sentence_t const question ("New initial weight?");
    std::string result;
    std::string old_weight = ctx->weight_to_string(aut->get_initial_weight(s));
    std::string proposition = 
      aut->is_final(s) ? old_weight
                       : ctx->weight_to_string(ctx->weight_one());
    if (ask_weight(result, proposition, question))
      if (result != old_weight) {
        aut->set_initial(s, ctx->get_weight(result));
        notify_modified();
      }
  }
  refresh_states(true);
}


bool
editter_t::ask_weight (std::string& result, std::string const& default_value, 
                       sentence_t const& question)
{
  static sentence_t message("Invalid weight: %s");
  static runtime_validator_t<dyn::automaton_t> validator
    ([](automaton_t const& aut, std::string const& str) 
        {
          try { aut->get_context()->get_weight(str); return true;}
          catch(...) {}
          return false;
        },
     aut,
     message);
  return ask_data(result, default_value, question, validator);
}


bool
editter_t::ask_label (std::string& result, std::string const& default_value,
                      unsigned i, sentence_t const& question)
{
  sentence_t message;
  if (this->is_transducer)
    message = sentence_t("Invalid label for tape "+std::to_string(i)+": %s");
  else
    message = sentence_t("Invalid label: %s");
  runtime_validator_t<std::vector<std::string>> validator(
    [](std::vector<std::string>const& alph, std::string const& str) 
      { for(std::string const& l : alph)
          if (str == l)
            return true;
        return false ;},
      alphabets.at(i),
      message
  );
  return ask_data(result, default_value, question, validator);
}


bool
editter_t::ask_alphabet (std::string& result, sentence_t const& question)
{
  static sentence_t message("Letter %s appears on a transition, cannot remove it from alphabet.");
  static runtime_validator_t<dyn::automaton_t> validator
    ( [](dyn::automaton_t const& aut, std::string const& str)
        {
          std::set<char> required_labels;
          for(dyn::transition_t tr : aut->transitions())
            required_labels.insert(tr);
          for (char c : required_labels)
            if (str.find(c) == std::string::npos)
              return false;
          return true;
        },
        aut,
        message
    );
  return ask_data(result, "", question, validator);
}


bool
editter_t::ask_aut_name (std::string& result, std::string const& default_value,
                         sentence_t const& question)
                         
{
  static sentence_t message("Only char in [a-z], [A-Z], [0-9] or '-' are "
                            "authorised.");
  static simple_validator_t validator (
   [] (std::string const& str) {
      for (char c : str) {
        if (! (  (c == '-')
               ||( c >= '0' &&  c <= '9' )
               ||( c >= 'a' &&  c <= 'z' )
               ||( c >= 'A' &&  c <= 'Z' )) )
          return false;
      }
      return true;
    },
    message
  );
  return ask_data(result, default_value, question, validator);
}


bool
editter_t::ask_state_name (std::string& result, 
                           std::string const& default_value,
                           sentence_t const& question)
{
  static sentence_t message("Another state is already named %s");
  static runtime_validator_t<editter_t> validator(
    [] (editter_t const& ed, std::string const& str) 
      { return ( !ed.aut->has_state(ed.aut->get_state_by_name(str))
                 || str == ed.current_state_name() ); },
    (*this),
    message
  );
  return ask_data(result, default_value, question, validator);
}


bool
editter_t::ask_filename (std::string& result, 
                         std::string const& default_value,
                         sentence_t const& question)
{
  static sentence_t message("Cannot wrile file: %s");
  static simple_validator_t validator(
    [] (std::string const& str) {
        int res = access(str.c_str(), W_OK);
        return (res == 0 || errno == ENOENT);
      },
    message
  );
  return ask_data(result, default_value, question, validator);
}


bool
editter_t::ask_state(std::string& result, 
                     std::string const& default_value,
                     sentence_t const& question)
{
  static sentence_t message("No state named %s");
  static runtime_validator_t<dyn::automaton_t> validator (
    [] (dyn::automaton_t const& aut, std::string const& str)
      { return aut->has_state(aut->get_state_by_name(str)); },
    this->aut,
    message
  );
  return ask_data(result, default_value, question, validator);
}


bool
editter_t::ask_confirmation (bool& b, 
                             bool default_value,
                             sentence_t const& comment)
{
  static std::unordered_map<std::string,bool> const acceptable_answers 
    = {{"yes",true}, {"y",true},  {"Yes",true}, {"Y",true},  {"Y/n",true}, 
       {"no",false}, {"n",false}, {"No",false}, {"N",false}, {"N/y",false}};
  static sentence_t const message("Invalid boolean answer: %s");
  static sentence_t const question("Confirm?");
  static simple_validator_t validator(
    [] (std::string const& str) {
        return (acceptable_answers.find(str) != acceptable_answers.end());
      },
    message
  );
  std::string result = "";
  if (ask_data(result, default_value?"Y/n":"N/y", 
               comment, question, validator)) 
  {
    b= acceptable_answers.find(result)->second;
    return true;
  }
  return false;
}


void
editter_t::enter_state(editstate_t state)
{
  editstate_stack.push(state);
  refresh_tips();
}


void
editter_t::leave_state()
{
  editstate_stack.pop();
  refresh_tips();
}


bool
editter_t::ask_data( std::string& result, 
                     std::string const& default_value,
                     sentence_t const& comment,
                     sentence_t const& question, 
                     abstract_validator_t& validator    )
{
  enter_state(ASK);
  if (comment.size() != 0) {
    comment_area.erase();
    comment_area.print_left(comment);
  }
  bool stop = false;
  answer_kind_t answer;
  while (!stop) {
//     std::stringstream ss;
//     ss << question <<"?";
//     if (default_value != "")
//       ss << "[" << default_value << "]";
//     ss << " ";
    query_area.erase(STANDOUT);
    attron(STANDOUT);
    query_area.start();
    query_area.overwrite(question);
    if (default_value != "")
      query_area.overwrite(token_t(" ["+default_value+"]"));
    query_area.overwrite_ch(' ');
    attroff(STANDOUT);
    switch(answer=wait_for_string(result)) {
      case ABORT: {
        stop = true;
        break;
      }
      case DEFAULT:
        result = default_value;
        if (default_value != "") {
          stop = true;
          break;
        }
      // fall through
      default: {
        if (validator.validate(result)) {
          stop = true;
          break;
        }
        else
          print_message(validator.error_message());
      }
    }
  }
  if (comment.size() != 0)
    comment_area.erase();
  leave_state();
  return (answer != ABORT);

}


// bool
// editter_t::ask_data (std::string& result,
//                      std::string const& question,
//                      std::string const& default_value,
//                      string_validator_t& validator,
//                      std::string const& error_msg )
// { 
//   decorated_validator_t v(validator, error_msg);
//   return ask_data(result, question, default_value, { &v });
// }


void
editter_t::action_state_final()
{
  if (states.size() == 0)
    return;
  dyn::state_t s = current_state();
  if (weightset_name == "B") {
    notify_modified();
    if (aut->is_final(s))
      aut->unset_final(s);
    else
      aut->set_final(s);
  }
  else {
    static sentence_t const question("New final weight?");
    std::string result;
    std::string old_weight = ctx->weight_to_string(aut->get_final_weight(s));
    std::string proposition =
      aut->is_final(s) ? old_weight
                       : ctx->weight_to_string(ctx->weight_one());
    if (ask_weight(result, proposition, question)) {
      if (result != old_weight) {
        aut->set_final(s, ctx->get_weight(result));
        notify_modified();
      }
    }
  }
  refresh_states(true);
}


void
editter_t::action_save()
{
  std::string result;
  static sentence_t const question("Save automaton as?");
  if (ask_filename(result, filename, question)) {
    filename = result;
    dyn::save(aut, filename, {IO_FORMAT = output_format});
    is_saved=true;
    ever_saved=true;
  }
  refresh_infos(true);
}


void 
editter_t::notify_modified()
{
  is_saved = false;
  refresh_infos(true);
}


void
editter_t::action_meta_promote()
{
  class weightset_validator_t : public abstract_validator_t {
    std::string const& ws;
    sentence_t _error_message;
   public:
    bool validate(std::string const& str) override {
      try {
        if ( (str != ws) && is_promotable(ws,str))
          return true;
        else {
          _error_message = sentence_t("Weightset "+ws+" cannot be promoted into %s.") /= str;
        return false;
        }
      }
      catch (std::invalid_argument& e) {
        _error_message = sentence_t("String "+str+" does not name a weightset.");
        return false;
      }
    }
    sentence_t const& error_message() const override { return _error_message; }
    weightset_validator_t(std::string const& s) : ws(s) {}
  };
  std::string ws= weightset_name;
  weightset_validator_t validator(ws);
  std::string result = "";

  dyn::context::weightset_description wd
    = dyn::context::weightset(weightset_name);
  std::vector<std::string> const& proms
    = dyn::context::weightset_promotion_public_static_names(wd);
  if (proms.empty()) {
    print_message( sentence_t("No super-weightset available for %s.")
                   /=weightset_name                                );
    return;
  }
  sentence_t comment ("Available:");
  for (auto it = proms.begin(); it != proms.end(); it++) {
    if (it+1 != proms.end())
      comment << (*it+',');
    else
      comment << *it;
  }
  static sentence_t question("New weightset?");
  if ( ask_data(result, "", comment, question, validator)) {
    automaton_t new_aut = promote_automaton(aut, result);
    dyn::internal::rename_from_origin_inplace(new_aut, aut);
    new_aut->set_desc(aut->get_desc());
    new_aut->set_name(aut->get_name());
    aut = new_aut;
    if (is_transducer)
      tdc = aut;
    weightset_name = result;
    ctx = aut->get_context();
    is_over_B = (weightset_name=="B");
    compute_formats();
    tip_handler.update_alternatives(aut);
    clear_transition_data();
    notify_modified();
    action_refresh(true);
  }
  comment_area.erase();
}


void
editter_t::action_meta_addletter()
{
  if (is_transducer) {
    print_message("Cannot add letters to transducers yet.");
    return;
  }
  static sentence_t const question ("New letter?");
  /* For the case where labels are integers */
  static sentence_t const int_message("The new letter should be an integer.");
  static simple_validator_t int_validator (
    [](std::string const& str) {
        return str.find_first_not_of( "0123456789" ) == std::string::npos;
      },
    int_message
  );
  /* For the case where labels are chars */
  static sentence_t const char_size_message
    ("In char alphabets, letters should be of size 1");
  static simple_validator_t char_size_validator (
    [] (std::string const& str) { return str.size() == 1; },
    char_size_message
  );
  static sentence_t const char_eps_message
    ("Given string, %s, is reserved for the representation of epsilon.");
  static runtime_validator_t<std::string> char_eps_validator(
    [] (std::string const& eps_repr, std::string const& str) 
      { return (eps_repr != str); },
    this->eps_repr,
    char_eps_message
  );
  static combined_validator_t char_validator
    ({&char_size_validator, &char_eps_validator});
  if (is_int_automaton) {
    std::string result = "";
    if ( ask_data(result, "", question, int_validator) ) {
      aut->get_context()->add_letter(atoi(result.c_str()));
      populate_alphabets();
      refresh_infos(true);
    }
  }
  else {
    std::string result = "";
    if ( ask_data(result, "", question, char_validator ) ) {
      aut->get_context()->add_letter(result[0]);
      populate_alphabets();
      refresh_infos(true);
    }
  }
}


void
editter_t::action_quit()
{
  static sentence_t const question("Save automaton before exiting?");
  if (!is_saved) {
    bool b;
    if (ask_confirmation(b, true, question)) {
      if (b) {
        action_save();
        action_quit();// This call is to take into account that action_save
        // may be cancelled by the user.
        return;
      }
    }
    else // if cancel, do not quit
      return;
  }
  continue_edit = false;
}


void
editter_t::action_display()
{
  pdfdisplay(aut);
}


void
editter_t::print_message(std::string const& str)
{
  print_message(sentence_t(str, NO_STYLE, " "));
}


void
editter_t::print_message(sentence_t tokens)
{
  tokens.font_style = STANDOUT;
  message_queue.push(tokens);
}


editstate_t
editter_t::editstate()
{
  return editstate_stack.empty()?MAIN_STATE:editstate_stack.top();
}


dyn::state_t
editter_t::current_state()
const
{
  return states[state_area.index];
}


std::string
editter_t::current_state_name() 
const
{
  return aut->get_state_name(current_state());
}


transition_t
editter_t::current_transition()
const
{
  return transitions[trans_area.index];
}


void
editter_t::notify(void* notifier)
{
  if (notifier == &message_queue)
    refresh_message();
}


} // end of namespace awali::cora
} // end of namespaces awali

#endif
