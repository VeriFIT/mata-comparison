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

#include <stdio.h>
#include <fstream>

#include <awali/dyn/loading/locations.hh>

#include <awali/dyn/algos/sys.hh>
#include <awali/dyn/modules/output.hh>
#include <awali/dyn/modules/context.hh>
#include <awali/dyn/modules/ratexp.hh>
#include <awali/common/no_such_file_exception.hh>
#include <awali/common/json_ast.hh>


namespace awali { namespace dyn {

//   void display(automaton_t aut, options_t opts) {
//     std::string filename= "/tmp/"+opts[NAME]+".gv";
//     std::ofstream o(filename);
//     put(aut, o, opts+(IO_FORMAT=DOT));
//     o.close();
//     system(("dotty "+filename).c_str());
//   }

  void 
  pdfdisplay
  ( automaton_t aut, options_t opts)
  {
    std::string name;
    if (aut->get_name() != "")
      name = aut->get_name();
    else
      name = "tmp";
    if (loading::get_dot_binary() == "") {
        *warning_stream << "Display is disabled (requires dot binary)" 
                        << std::endl;
      return;
    }
    std::string filename= "/tmp/"+name+".pdf";
    std::ofstream o(filename);
    put(aut, o, opts+(IO_FORMAT=PDF));
    o.close();
    std::string command;
// __APPLE__ ,  __linux__ , __unix__ , __WIN32
#ifdef __APPLE__
    command = "open ";
#else
    command = "xdg-open ";
#endif
    system((command+filename).c_str());
  }


  automaton_t parse_automaton(std::istream& i, options_t opts) {
    switch(opts[IO_FORMAT]) {
      case FADO:
        return internal::fado(i);
      case GRAIL:
        return internal::grail(i);
      case JSON:
        return internal::parse_automaton(i);
      case FSM_JSON_V0:
        return internal::deprecated_parse_automaton(i);
      default:
        throw std::runtime_error("Only FADO, GRAIL, JSON and DEPRECATED are possible input formats.");
    }
  }

  
  automaton_t 
  internal::load(const std::string& filename, bool& found, io_format_t format) 
  {
    if (is_true_json(format)) {
      return parse_automaton(load_json_ast(found,filename, true));
    } 
    else if (format == FSM_JSON_V0) {
      std::map<std::string,loading::file_loc_t> examples
      = loading::examples({"automata","ratexps"}, true);

      auto it = examples.find(filename);
      found = (it != examples.end());
      if (found) {
        std::ifstream fic(it->second.full());
        if (fic.fail ())
          throw no_such_file_exception("can not load example automaton " + filename);
        return dyn::parse_automaton(fic, {IO_FORMAT=format});
      }
    }
    std::ifstream fic(filename);
    if (fic.fail ())
      throw no_such_file_exception("can not load file " + filename);
    return dyn::parse_automaton(fic, {IO_FORMAT=format});
  }


  automaton_t load(const std::string& s, options_t opts) {  
    bool b;
    return internal::load(s,b,opts[IO_FORMAT]);
  }

  ratexp_t load_exp(const std::string& s) {
    bool b;
    return internal::parse_ratexp(internal::load_json_ast(b,s,true));
  }

  
  std::ostream& put(automaton_t aut, std::ostream& o, options_t opts)
  {
    switch(opts[IO_FORMAT]) {
      case DOT: return internal::dot(aut,o,opts);
      case FADO: return internal::fado(aut,o);
      case GRAIL: return internal::grail(aut,o);
      case JSON: return internal::json(aut,o);
      case PDF: return internal::pdf(aut,o,opts);
      case TEXT: 
        throw std::runtime_error("TEXT format is not supported for automata");
      case SVG: return internal::svg(aut,o,opts);
      default:
        throw std::runtime_error("put:: Unreachable statement");
    }
  }
  
  std::ostream& put(ratexp_t ratexp, std::ostream& o, options_t opts)
  {
    switch(opts[IO_FORMAT]) {
      case JSON: {
        json_ast_t ast = to_json_ast(ratexp);
        return awali::put(ast,o);
      }
      case TEXT: return ratexp->print(o);
      case DOT:
      case FADO:
      case GRAIL:
      case SVG:
      case PDF:
        throw std::runtime_error(
          "Only JSON and TEXT format are supported for ratexps.");
      default:
        throw std::runtime_error("put:: Unreachable statement");
    }
  }

  void save(const automaton_t aut, const std::string& filename, 
  options_t opts) {
     std::ofstream o(filename);
     put(aut,o,opts);
     o.close();
  }

  std::ostream *error_stream = &std::cerr;
  std::ostream *warning_stream = &std::cerr;
  
  void set_error_stream(std::ostream & o) {
    error_stream = &o;    
  }

  void set_warning_stream(std::ostream & o) {
    warning_stream = &o;    
  }



  std::ostream& operator<<(std::ostream& o, automaton_t aut) {
    return put(aut,o);
  }

  std::ostream& operator<<(std::ostream& o, transducer_t tdc) {
    return o << (automaton_t) tdc;
  }

  internal::formatted_ostream operator<<(std::ostream& o, io_format_t fmt) {
    return internal::formatted_ostream(fmt, o);
  }

  internal::formatted_istream operator>>(std::istream& i, io_format_t fmt) {
    return internal::formatted_istream(fmt, i);
  }

  std::ostream& internal::formatted_ostream::operator<<(ratexp_t ratexp) {
    return put(ratexp,this->o, {IO_FORMAT=this->format});
  }

  std::ostream& internal::formatted_ostream::operator<<(automaton_t aut) {
    return put(aut,this->o, {IO_FORMAT=this->format});
  }

  std::istream& internal::formatted_istream::operator>>(automaton_t& aut) {
    aut = dyn::parse_automaton(this->i,{IO_FORMAT=format});
    return (this->i);
  }


  std::istream& operator>>(std::istream& i, automaton_t& aut) {
    aut = parse_automaton(i);
    return i;
  }

  namespace internal {



  json_ast_t
  load_json_ast(bool& found, const std::string& s, bool recurse) 
  {
    std::map<std::string,loading::file_loc_t> examples
    = loading::examples({"automata","ratexps"}, recurse);

    std::string filename (s);

    auto it = examples.find(s);
    found = (it != examples.end());
    if (found)
      filename = it->second.dir + "/" + it->second.name+"." + it->second.ext;
    
    return json_ast::from_file(filename);
  } 

  
 json_ast_t
 load_json_ast(std::string const& file, bool recurse) {
    bool found;
    return load_json_ast(found, file, recurse);
  }

  aut_or_exp_t::~aut_or_exp_t() { 
    if (is_aut)
      aut.~automaton_t();
    else
      exp.~ratexp_t();
  }

  aut_or_exp_t::aut_or_exp_t(aut_or_exp_t const& other)
  : is_aut(other.is_aut) 
  {
    if (is_aut)
      aut = other.aut;
    else
      exp = other.exp;
  }

  bool
  aut_or_exp_t::split_case(automaton_t& a, ratexp_t& e) {
    if (is_aut)
      a = aut;
    else
      e = exp;
    return is_aut;
  }


  aut_or_exp_t load_aut_or_exp(std::string const& filename, bool recurse) 
  {
    return parse_aut_or_exp(load_json_ast(filename, recurse));
  }


  aut_or_exp_t parse_aut_or_exp(json_ast_t p) {
    if (!p->has_child("kind"))
      throw std::runtime_error("JSON file does not define a field \"kind\".");
    
    std::string kind = p->at("kind")->to_string();
    if (kind=="Rational Expression")
      return parse_ratexp(p);
    else if (kind == "Automaton") 
      return parse_automaton(p);
    else
      throw std::runtime_error("JSON file associate field \"kind\" to \""
        + kind
        + "\",expecting \"Automaton\" or \"Rational Expression\".");
  }
  } // end of ns awali::dyn::internal
  
  automaton_t parse_automaton(json_ast_t p) {
    return internal::parse_aut_or_exp(p).aut;
  }
  
  ratexp_t parse_ratexp(json_ast_t p) {
    return internal::parse_aut_or_exp(p).exp;
  }


}}//end of ns awali::dyn
