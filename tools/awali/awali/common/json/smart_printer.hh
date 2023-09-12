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

#ifndef COMMON_JSON_SMART_PRINTER_HH
#define COMMON_JSON_SMART_PRINTER_HH

#include <unordered_map>
#include <sstream>

#include <awali/common/json/visitor.hh>
#include <awali/common/json/parser.hh>

namespace awali {
namespace json {


class heighter_t : public visitor_t {
protected:
  std::unordered_map<node_t const*,unsigned> map;

  void enter(node_t const* node) override;

  void after_child(node_t const* node, uint_or_string_t const& uos,
                   node_t const* child) override;
public:
  inline unsigned operator[] (node_t const* node);

  heighter_t(json_ast_t ast) { run(ast); }
  heighter_t(node_t const* node) { run(node); }
  heighter_t() {}
};



class inline_sizer_t : public visitor_t {
protected:
  std::unordered_map<node_t const*,unsigned> map;
  std::unordered_map<node_t const*,unsigned> left_weight;
  heighter_t heighter;

  void leave(bool_t const* node) override;
  void leave(int_t const* node)  override;
  void leave(string_t const* node) override;
  void leave(float_t const* node) override;
  void leave(null_t const* node) override;
  void enter(object_t const* node) override;
  void enter(array_t const* node) override;
  void after_child(array_t const* node, unsigned i, node_t const* child) override;
  void after_child(object_t const* node, std::string const& key,
                   node_t const* child) override;
  void between_children(object_t const* node) override;
  void between_children(array_t const* node) override;

public:
  unsigned operator[] (node_t const* node);

  unsigned height_of(node_t const* node);

  inline_sizer_t(json_ast_t ast) { run(ast); }
  inline_sizer_t(node_t const* node) { run(node); }
  inline_sizer_t() {}
};



class smart_printer_t : public visitor_t {
protected:
  inline_sizer_t sizer;
  std::ostream& _out;
  int indent_amount ;
  node_t const* inliner;
  unsigned _max ;
  std::vector<unsigned> max_vect;
  unsigned max ();


  void indent(int i=0);

  void leaf(double d) override;
  void leaf(std::string const& str) override;
  void leaf(int i) override;
  void leaf(std::nullptr_t) override;
  void leaf(bool b) override;

  void enter(array_t const* node) override;
  void leave(array_t const* node) override;

  void enter(object_t const* node) override;
  void leave(object_t const* node) override;

  void between_children(object_t const* node) override;
  void between_children(array_t const* node) override;

  void after_child(node_t const* node, uint_or_string_t const& uos,
                   node_t const* child) override;

  void before_child(node_t const* node, uint_or_string_t const& uos,
                    node_t const* child) override;

  void before_child(array_t const* node, unsigned i,
                    node_t const* child) override;

  void before_child(object_t const* node, std::string const& key,
                    node_t const* child) override;

  void incr_indent();
  void decr_indent();

  bool is_last (node_t const* child);

  inline void leave(node_t const* node) override;

public:
  smart_printer_t (std::ostream& o, unsigned m = 80);

  void run(json_ast_t tree) override;
  void run(node_t const* tree) override;
};

/*
class yaml_sizer_t : public visitor_t {
protected:
  std::unordered_map<node_t const*,unsigned> map;

  inline void leave(bool_t const* node) override {map[node] = node->value?3:2;}
  inline void leave(int_t const* node)  override{
    std::stringstream ss; ss << node->value; map[node] = ss.str().length();}
  inline void leave(string_t const* node) override {
    map[node] = size_of(node->value);}
  inline void leave(float_t const* node) override {
    std::stringstream ss; ss << node->value; map[node] = ss.str().length();}
  inline void leave(null_t const* node) override { map[node] = 4;}
  inline void enter(object_t const* node) override { map[node] = 2;}
  inline void enter(array_t const* node) override { map[node] = 2;}
  inline void after_child(array_t const* node,
                          unsigned i, node_t const* child) override  {
    map[node] += map[child];
  }
  inline void after_child(object_t const* node,
    std::string const& key, node_t const* child) override
  {map[node] += key.length() + 2 + map[child];}

  inline void between_children(object_t const* node) override {map[node] += 3;}
  inline void between_children(array_t const* node) override {
    if (node->height() == 1)
      map[node] += 1;
    else
      map[node] += 2;
  }


public:
  static bool must_escape(std::string str) {
    static const std::vector<std::string> reserved
      = {"Yes", "No", "null"};
    static const std::string authorized
      =  "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789" ;
    if (str.find_first_not_of(authorized) != std::string::npos)
      return true;
    for (std::string const& target: reserved)
      if (str == target)
        return true;
    size_t idx = 0;
    try {
      std::stod(str, &idx);
    } catch (std::invalid_argument const&) {}
    if (idx == str.size())
      return true;
    return false;
  }

  static unsigned size_of(std::string const& str) {
    if (must_escape(str))
      return escape(str).length();
    else
      return str.length();
  }

  inline unsigned operator[] (node_t const* node) {return map.find(node)->second;}

};


class yaml_printer_t : public visitor_t {
protected:
  node_t const* inliner = nullptr;
  std::ostream& _out;
  std::string hyphen = " - ";
  unsigned indent_amount = hyphen.size();
  std::string comma = ",  ";
  bool print_inline = false;
  yaml_sizer_t sizer;
  unsigned max;

  inline void indent() {if (!print_inline) {_out << '\n' << std::string((path().size())*indent_amount, ' ');}}

  inline void leaf(std::string const& str) override {
    if (sizer.must_escape(str))
      parser_t::escape_and_put(_out,str);
    else
      _out << str;
  }

  inline void enter(array_t const* node) override {
    if (print_inline)
      _out << "[";
  }
  inline void leave(array_t const* node) override {
    if (print_inline)
      _out << "]";
  }

  inline void enter(object_t const* node) override {
    if (print_inline)
      _out << "{";
  }

  inline void leave(object_t const* node) override {
    if (print_inline)
      _out << "}";
  }


  inline void leaf(int i) override {_out << i;}

  inline void leaf(bool b) override {_out << (b?"Yes":"True");}

  inline void leaf(double x) override {_out << "test" << x;}

  inline void leaf(std::nullptr_t p) override {_out << "null";}

  inline void before_child(array_t const* node,
    unsigned i, node_t const* child) override
  {
    if (!print_inline) {
     _out << hyphen;
     if (depth()*indent_amount+sizer[child]+2<=max) {
      inliner = child;
      print_inline = true;
     }
   }
  }


  inline void between_children(array_t const* node) override
  { if (print_inline) {
      _out << ',';
      if (node->height() > 1)
        _out << ' ';
    }
    else
      indent();
  }

  inline void between_children(object_t const* node) override
  { if (print_inline) _out << comma; else indent();}

  inline void before_child(object_t const* node,
    std::string const& key, node_t const* child) override
  { _out << key<< ": ";
    if (!print_inline && !child->is_leaf()) {
      if (depth()*indent_amount+sizer[child]+key.length()+2<=max) {
        inliner = child;
        print_inline = true;
      }
      else {
        indent();
        _out << std::string(indent_amount, ' ');
      }
    }
  }

  inline void leave(node_t const* node) override
  {if (inliner == node) print_inline = false; }

public:
  inline void run(node_t const* tree) override {
    sizer.run(tree);
    visit(tree);
  }
  yaml_printer_t(std::ostream& out, unsigned m = 80) : _out(out), max(m) {}
};

//inline void inline_sizer_t::leave(node_t const* node) {
//    std::cerr << std::endl;
//    smart_printer_t printer(std::cerr, 0);
//    printer.run(node);
//    unsigned m = map.find(node)->second;
//    std::cerr << std::endl;
//    for (unsigned i = 1; i <= m; i++)
//      std::cerr << i%10;
//    std::cerr << std::endl;
//  }
*/

std::ostream& put(node_t const* node, std::ostream& out); 

} //end of namespace json
} //end of namespace awali
#endif
