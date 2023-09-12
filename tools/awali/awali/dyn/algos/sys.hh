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

#ifndef DYN_ALGOS_SYS_HH
#define DYN_ALGOS_SYS_HH

#include<stdio.h>

#include <awali/common/json_ast.hh>

#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/core/ratexp.hh>
#include <awali/dyn/core/transducer.hh>
#include <awali/dyn/options/options.hh>

namespace awali {
namespace dyn {
 
//  /** Computes geometry of \p aut with program dot and displays it with dotty.
//   * @param aut Automaton to display
//   * @param opts A set of options; {@link KEEP_HISTORY}, 
//   * {@link LAYOUT} are meaningful.
//   */
//  void display(automaton_t aut, options_t opts = {});
 

  /** Computes geometry of \p aut with program dot and displays with a
   * pdf-viewer.
   * @param aut Automaton to display
   * @param opts A set of options; {@link KEEP_HISTORY}, 
   * {@link LAYOUT} are meaningful.
   */
  void pdfdisplay(automaton_t aut, options_t opts = {});


  /** Parse an automaton from
   * @param in   Input stream.
   * @param opts A set of options; {@link IO_FORMAT} is meaningful.
   * @return The parsed automaton.
   */
  automaton_t parse_automaton(std::istream& in, options_t opts = {});


  /** Write \p aut to output stream \p o.
   * @param aut
   * @param o 
   * @param opts A set of options; {@link IO_FORMAT} is meaningful; 
   * {@link KEEP_HISTORY} and {@link LAYOUT} are meaningful if given 
   * {@link IO_FORMAT} uses dot to compute geometry.
   * @return The stream \p o
   */
  std::ostream& put(automaton_t aut, std::ostream& o, options_t opts = {});
  
  std::ostream& put(ratexp_t aut, std::ostream& o, options_t opts = {});


  /** Writes \p aut to file pointed by \p path
   *
   * @param aut
   * @param path
   * @param opts A set of options; {@link IO_FORMAT} is meaningful; 
   * {@link KEEP_HISTORY} and {@link LAYOUT} are meaningful if given 
   * {@link IO_FORMAT} uses dot to compute geometry.
   */
  void save(const automaton_t aut, const std::string& path, options_t opts ={});


  /** Loads an automaton from file pointed by \p path.
   * @param path
   * @param opts A set of options; {@link IO_FORMAT} is meaningful.
   * @return A new automaton
   */
  automaton_t load(const std::string& path, options_t opts={});

  /** Loads a \ref ratexp_t from file @pname{filepath} */
  ratexp_t load_exp(const std::string& filepath);
  
  namespace internal {
    // This function returns by reference into b whether the automaton is an
    // example automaton.
    automaton_t load(const std::string& filename, 
      bool& is_example,  
      io_format_t format = JSON);
  }

  /** A pointer to the error_stream used at dynamical layer. */
  extern std::ostream *error_stream ;
  
  /** A pointer to the error_stream used at dynamical layer. */
  extern std::ostream *warning_stream;
  
  /** Setter for the {@link error_stream} of the dynamical layer. */
  void set_error_stream(std::ostream & o);

  /** Setter for the {@link warning_stream} of the dynamical layer. */
  void set_warning_stream(std::ostream & o);

  
  namespace internal {
    struct formatted_ostream {
      private :
        io_format_t format;
        std::ostream& o;
      public :
        formatted_ostream(io_format_t fmt, std::ostream& o)
        : format(fmt), o(o) {}

        std::ostream& operator<<(ratexp_t aut);
        std::ostream& operator<<(automaton_t aut);
        std::ostream& operator<<(transducer_t aut);
    };
    
    struct formatted_istream {
      private :
        io_format_t format;
        std::istream& i;
      public :
        formatted_istream(io_format_t fmt, std::istream& i)
        : format(fmt), i(i) {}

        std::istream& operator>>(automaton_t& aut);
    };
  }

  std::ostream& operator<<(std::ostream& o, automaton_t aut);
  std::ostream& operator<<(std::ostream& o, transducer_t aut);
  internal::formatted_ostream operator<<(std::ostream& o, 
    io_format_t const fmt);
  std::istream& operator>>(std::istream& i, automaton_t& aut);
  internal::formatted_istream operator>>(std::istream& i, 
    io_format_t const fmt);


  namespace internal {
    /** Loads a json file as an AST (possibly an example).
     * @param found  indicate whether the file corresponds to an example file.
     * @param filename the name of the files
     * @param recurse if true the subdirectories of the example directories are explored
     * */
    json_ast_t load_json_ast(bool& found, std::string const& filename,
                                     bool recurse = false);

    /** Loads a json file as an AST (possibly an example). */
    json_ast_t load_json_ast(std::string const& filename, 
                                     bool recurse= false);
    
    
    struct aut_or_exp_t {
      bool is_aut = true;
      union {
        automaton_t aut;
        ratexp_t exp;
      };
      aut_or_exp_t() : is_aut(true), aut() {}
      ~aut_or_exp_t();
      aut_or_exp_t(aut_or_exp_t const& other);
      aut_or_exp_t(ratexp_t e) : is_aut(false), exp(e) {}
      aut_or_exp_t(automaton_t a) : is_aut(true), aut(a) {}
  

      /** Returns field #is_aut and assign either @pname{a} or @pname{e} to the
       * content of this.
       *
       * @a Assigned if this contained an \ref automaton_t.
       * @e Assigned if this contained a \ref ratexp_t.
       * @return `true` if @pname{a} was assigned, `false`  otherwise
       */
      bool split_case(automaton_t& a, ratexp_t& e);
    };


    aut_or_exp_t load_aut_or_exp(std::string const& name, bool recurse = false);
    aut_or_exp_t parse_aut_or_exp(json_ast_t ast);

  }
  automaton_t parse_automaton(json_ast_t ast);
  ratexp_t parse_ratexp(json_ast_t ast);

}}//end of ns awali::dyn



#endif
