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

#ifndef AWALI_JSON_NODE_HH
#define AWALI_JSON_NODE_HH

#include<awali/common/json/utils.hh>
#include<awali/common/linked_map.hxx>
#include<vector>
#include<unordered_map>
#include<cmath>
#include<memory>

namespace awali {
namespace json {


/* =============================================================================
   Forward declarations
============================================================================= */

class node_t;
class object_t;
class array_t;
class int_t;
class float_t;
class string_t;
class bool_t;
class null_t;
class parser_t;

/* =============================================================================
   uint_or_string  and  navigation path
============================================================================= */
class uint_or_string_t {
protected:
  long i;
  std::string str;
public:
  uint_or_string_t(unsigned integer) : i(integer), str("") {}

  uint_or_string_t(std::string s) : i(-1), str(std::move(s)) {}

  uint_or_string_t(uint_or_string_t const& uos)
  : i(uos.i), str(uos.str) {}

  uint_or_string_t(uint_or_string_t&& uos) 
  : i(uos.i), str(std::move(uos.str)) {}

  uint_or_string_t(char const* s) : i(-1), str(s?s:"") {}

  inline bool is_string() const {return i<0;}

  inline bool is_integer() const {return i>=0;}

  inline unsigned integer() const {return i;}

  inline std::string const& string() const {return str;}


  inline operator std::string&() {return str;}
  inline operator std::string() const {return str;}
  inline operator unsigned() const {return i;}

  inline uint_or_string_t& operator=(uint_or_string_t const& uos) 
  { if (this != &uos) { i = uos.i; str = uos.str;} return *this;}

  inline uint_or_string_t& operator=(uint_or_string_t&& uos) 
  { i = uos.i, str = std::move(uos.str); return *this;}
};

typedef std::vector<uint_or_string_t> path_t;



/* =============================================================================
   NODE KIND
============================================================================= */
enum node_kind_t {
  OBJECT   = 0,
  ARRAY    = 1,
  INTEGER  = 2,
  FLOATING = 3,
  STRING   = 4,
  BOOLEAN  = 5,
  _NULL    = 6
};

std::string const& string_of(node_kind_t kind);

std::ostream& operator<<(std::ostream& o, node_kind_t kind);

/* =============================================================================
   EXCEPTIONS
============================================================================= */

class exception : public std::exception {
public:
  std::string message;
  std::string caller;
  node_t const* node;
  node_t* attempted_parsed_tree; 
  path_t path_to_root;
  std::string what_msg;

  exception(std::string const& message, std::string const& caller,
            path_t path_to_root, int line, int col);

  exception(std::string const& message, std::string const& caller,
            node_t const* node = nullptr);

  exception(std::string message, node_t const* node = nullptr)
  : exception(message, "", node) {}

  virtual const char* what() const noexcept override 
  {return what_msg.c_str();}
};

class parse_exception: public exception {
public:
  parse_exception(std::string const& message, std::string const& caller,
            path_t path_to_root, int line, int col):
  exception(message, caller, path_to_root, line, col) {}
  parse_exception(std::string const& message, std::string const& caller):
  exception(message,caller){}
};


/** Exception used when trying to coerce a node to a given type */
class coercion_exception : public exception {
public:
  coercion_exception(std::string const& message, std::string const& caller,
            node_t const* node = nullptr)
  : exception(message, caller, node)  {}

  coercion_exception(std::string message, node_t const* node = nullptr)
  : coercion_exception(message, "", node) {}
};


class out_of_range : public exception {
public:
  out_of_range(std::string const& message, std::string const& caller,
               node_t const* node = nullptr)
  : exception(message, caller, node)
  {}

  out_of_range(std::string const& message, node_t const* node = nullptr)
  : out_of_range(message, "", node)
  {}
};



class kind_mismatch : public exception {
public:
  std::vector<node_kind_t> expected_types;
  kind_mismatch(std::string const& caller, node_t const* node,
                node_kind_t expected_type)
  : kind_mismatch(caller, node, {expected_type}){}

  kind_mismatch(std::string const& caller, node_t const* node,
                std::initializer_list<node_kind_t> types);


protected:
  static std::string message_builder(node_kind_t caller_kind,
                                     std::initializer_list<node_kind_t> types);

};





/* =============================================================================
   node_t
============================================================================= */

class node_t {
public:
  node_t const* parent;
  node_kind_t const kind;

//   static node_t* parse(std::istream& i);

  virtual ~node_t() {}

  /** Casts this node to {@link object_t} */
  virtual inline object_t const* object() const
  {throw kind_mismatch("node_t::object()", this, node_kind_t::OBJECT);}

  /** Casts this node to {@link array_t} */
  virtual inline array_t const* array() const
  {throw kind_mismatch("node_t::array()", this, node_kind_t::ARRAY);}

  /** Casts this node to {@link int_t} */
  virtual inline int_t const* integer() const
  {throw kind_mismatch("node_t::integer()", this, node_kind_t::INTEGER);}

  /** Casts this node to {@link string_t} */
  virtual inline string_t const* string() const
  {throw kind_mismatch("node_t::string()", this, node_kind_t::STRING);}

  /** Casts this not to {@link float_t} */
  virtual inline float_t const* floating() const
  {throw kind_mismatch("node_t::floating()", this, node_kind_t::FLOATING);}

  /** Casts this node to {@link bool_t} */
  virtual inline bool_t const* boolean() const
  {throw kind_mismatch("node_t::boolean()", this, node_kind_t::BOOLEAN);}

  /** Casts this node to {@link null_t} */
  virtual inline null_t const* null() const
  {throw kind_mismatch("node_t::null()", this, node_kind_t::_NULL);}

  /** Casts this node to {@link object_t} */
  virtual inline object_t* object()
  {throw kind_mismatch("node_t::object()", this, node_kind_t::OBJECT);}

  /** Casts this node to {@link array_t} */
  virtual inline array_t* array()
  {throw kind_mismatch("node_t::array()", this, node_kind_t::ARRAY);}

  /** Casts this node to {@link int_t} */
  virtual inline int_t* integer()
  {throw kind_mismatch("node_t::integer()", this, node_kind_t::INTEGER);}

  /** Casts this node to {@link string_t} */
  virtual inline string_t* string()
  {throw kind_mismatch("node_t::string()", this, node_kind_t::STRING);}

  /** Casts this not to {@link float_t} */
  virtual inline float_t* floating()
  {throw kind_mismatch("node_t::floating()", this, node_kind_t::FLOATING);}

  /** Casts this node to {@link bool_t} */
  virtual inline bool_t* boolean()
  {throw kind_mismatch("node_t::boolean()", this, node_kind_t::BOOLEAN);}

  /** Casts this node to {@link null_t} */
  virtual inline null_t* null()
  {throw kind_mismatch("node_t::null()", this, node_kind_t::_NULL);}

  virtual node_t* copy() const =0;

  node_t* at_path(path_t const& path, unsigned i = 0);
  node_t const* at_path(path_t const& path, unsigned i = 0) const;
  bool has_path(json::path_t const& path) const;

  inline virtual unsigned height() const {return 0;}

//  template<typename T2, typename... Args>
//  node_t* at(std::string const& str, T2 arg2, Args... args) {
//    return this->at(str)->at(arg2, args...);
// }

//  template<typename T2, typename... Args>
//  bool has_path(std::string const& str, T2 arg2, Args... args) const {
//    if (!this->has_child(str))
//      return false;
//    return this->at(str)->has_path(arg2, args...);
//  }

  //node_t* at(std::initializer_list<std::string> const&);
  //node_t const* at(std::initializer_list<std::string> const&) const;
  //bool has_child(std::initializer_list<std::string> const&l) const;

  virtual node_t* at(std::string const& key);
  virtual node_t const* at(std::string const& key) const;
  inline virtual bool has_child(std::string const&) const
  {return false;}


  //node_t const* at(std::initializer_list<unsigned> const&) const;
  virtual node_t* at(unsigned i);
  virtual node_t const* at(unsigned i) const;
  inline virtual bool has_child(unsigned) const
  {return false;}
  //inline virtual bool has_child(std::initializer_list<unsigned> const& l) const
  //{node_t const* n = this; for (auto x : l) if (n->has_child(x)) n = n->at(x); else return false; return true;}


  /** Coerces this {@link node_t} to `bool`.
   *  @throws type_missmatch if this {@link node_t} is not a
   *  {@link bool_t}, an {@link int_t}, a {float_t} or a
   *  {@link string_t}
   *  @throws coercion_exception if this {@link node_t} is
   *  an {@link int_t} and held value is not 0 or 1; or if it is a 
   *  {@link string_t} and held value is one of {"0", "1", "true", "false",
   *  "True", "False"}.
   * */
  virtual inline bool to_bool() const
  {throw kind_mismatch("node_t::to_bool()", this,
                      {node_kind_t::BOOLEAN,  node_kind_t::INTEGER,
                        node_kind_t::FLOATING, node_kind_t::STRING });}


  /** Coerces this {@link node_t} to `int`.
   *  @throws type_missmatch if this {@link node_t} is not a
   *  {@link bool_t}, an {@link int_t}, a {@link float_t},
   *  or a {@link string_t}.
   *  @throws coercion_exception if this {@link node_t} is a
   *  string_t the value of which is not the string-representation of an
   *  integer; or if this {@link node_t} is a {@link float_t} the held
   *  value of which is not an integer.
   */
  virtual inline int to_int() const
  {throw kind_mismatch("node_t::to_int()", this,
                       {node_kind_t::BOOLEAN,  node_kind_t::INTEGER,
                        node_kind_t::FLOATING, node_kind_t::STRING });}


  /** Coerces this {@link node_t} to an `std::string`.
   *  @throws type_missmatch if this {@link node_t} is not a
   *  {@link string_t}.
   */
  virtual std::string to_string() const
  {throw kind_mismatch("node_t::to_string()", this, node_kind_t::STRING);}


  /** Coerces this {@link node_t} to a `double`
   *  @throws type_missmatch if this {@link node_t} is not a
   *  {@link bool_t}, an {@link int_t}, a {@link float_t},
   *  or a {@link string_t}.
   *  @throws coercion_exception if this {@link node_t} is a
   *  {@link string_t} the value of which is not the string-representation
   *  of a double.
   */
  virtual double to_double() const
  {throw kind_mismatch("node_t::to_double()", this,
                       {node_kind_t::BOOLEAN,  node_kind_t::INTEGER,
                       node_kind_t::FLOATING, node_kind_t::STRING });}


  inline bool is(node_kind_t k) const {return (this-> kind == k);}
  inline bool is(std::vector<node_kind_t>const& v) const
  {for(auto k:v) if (this->is(k)) return true; return false;}

  inline bool is_leaf() const {return !this->is({OBJECT,ARRAY});}

  virtual inline unsigned arity() const {return 0u;}

  path_t path_to_root() const;

protected:
  node_t(node_kind_t k) : parent(nullptr), kind(k)  {}
};


/* =============================================================================
   OBJECT
============================================================================= */
class object_t final : public node_t {
  linked_map_t<std::string,node_t*> _fields;
public:
  linked_map_t<std::string,node_t*> const& fields;
  typedef linked_map_t<std::string,node_t*>::iterator iterator;
  typedef linked_map_t<std::string,node_t*>::const_iterator const_iterator;

  object_t() : node_t(node_kind_t::OBJECT), _fields(), fields(_fields) {}
  object_t(std::string key, node_t* child) : object_t()
  {this->push_back(std::move(key),child);}

  ~object_t();

  inline virtual bool has_child(std::string const& key) const override
  {return fields.find(key) != fields.end(); }

  virtual node_t* at(std::string const& key) override;

  virtual node_t const* at(std::string const& key) const override;

  object_t* push_back(std::string key, node_t* node);

  object_t* push_front(std::string key, node_t* node);
  
  object_t* insert_after(std::string const& ref_key, 
    std::string key, node_t* node);

  object_t* insert_before(std::string const& ref_key, 
    std::string key, node_t* node);

  inline void erase(std::string const& key) {
    auto it = _fields.find(key);
    if (it != _fields.end())
      delete it->second;
    _fields.erase(it);
  }

  inline virtual object_t* object() override {return this;}
  inline virtual object_t const* object() const override {return this;}

  virtual object_t* copy() const override ;

  inline virtual unsigned height() const override {
    unsigned d = 0;
    for (auto p : fields) {
      unsigned x = p.second->height() + 1;
      if (x > d)
        d = x;
    }
    return d;
  }

  inline iterator begin() {return _fields.begin();}
  inline iterator end() {return _fields.end();}
  inline const_iterator begin() const {return fields.begin();}
  inline const_iterator end() const {return fields.end();}

  inline virtual unsigned arity() const override {return fields.size();}
};


/* =============================================================================
   ARRAY
============================================================================= */
class array_t final : public node_t {
protected:
  std::vector<node_t*> _values;

public:
  std::vector<node_t*> const& values;

  typedef std::vector<node_t*>::iterator iterator;
  typedef std::vector<node_t*>::const_iterator const_iterator;

  array_t() : node_t(node_kind_t::ARRAY), _values(), values(_values) {}
  array_t(std::vector<node_t*> v) 
  : node_t(node_kind_t::ARRAY), _values(std::move(v)), values(_values) {}

  ~array_t();

  inline virtual array_t* array() override {return this;}
  inline virtual array_t const* array() const override {return this;}

  virtual array_t* copy() const override;


  virtual node_t* at(unsigned i) override;

  virtual node_t const* at(unsigned i) const override;

  inline iterator begin() {return _values.begin();}
  inline const_iterator begin() const {return values.begin();}

  inline iterator end() {return _values.end();}
  inline const_iterator end() const {return values.end();}

  array_t* push_back(node_t* p);

  inline unsigned arity() const override {return values.size();}

  inline unsigned empty() const {return arity() == 0;}


  inline virtual bool has_child(unsigned i) const override
  {return i < arity();}

  inline virtual unsigned height() const override {
    unsigned d = 0;
    for (auto p : values) {
      unsigned x = p->height()+1;
      if (x > d)
        d = x;
    }
    return d;
  }
};


/* =============================================================================
   INTEGER
============================================================================= */
class int_t final : public node_t {
public:
  int value;

  int_t(int v) : node_t(node_kind_t::INTEGER), value(v) {}

  inline virtual int_t* copy() const override {return new int_t(value);}

  inline virtual int_t* integer() override {return this;}
  inline virtual int_t const* integer() const override {return this;}

  virtual bool to_bool() const override;
  virtual inline int to_int() const override {return value;}
  virtual inline double to_double() const override {return value;}
};


/* =============================================================================
   FLOATING
============================================================================= */
class float_t final : public node_t {
public:
  double value;

  float_t(double v) : node_t(node_kind_t::FLOATING), value(v) {}

  inline virtual float_t* copy() const override
  {return new float_t(value);}

  inline virtual float_t* floating() override {return this;}
  inline virtual float_t const* floating() const override {return this;}

  virtual bool to_bool() const override;
  virtual int to_int() const override;
  virtual inline double to_double() const override {return value;}
};


/* =============================================================================
   STRING
============================================================================= */
class string_t final : public node_t {
public:
  std::string value;

  string_t(char c)
  : node_t(node_kind_t::STRING), value(1,c) {}

  string_t(std::string v)
  : node_t(node_kind_t::STRING), value(std::move(v)) {}

  inline virtual string_t* copy() const override
  {return new string_t(value);}

  inline virtual string_t* string() override {return this;}
  inline virtual string_t const* string() const override {return this;}

  virtual bool to_bool() const override;
  virtual int to_int() const override;
  virtual double to_double() const override;
  virtual inline std::string to_string() const override {return this->value;}
};


/* =============================================================================
   BOOLEAN
============================================================================= */
class bool_t final : public node_t {
public:
  bool value;

  bool_t(bool v) : node_t(node_kind_t::BOOLEAN), value(v) {}

  inline virtual bool_t* copy() const override
  {return new bool_t(value);}

  inline virtual bool_t* boolean() override {return this;};
  inline virtual bool_t const* boolean() const override {return this;};


  virtual inline bool to_bool() const override {return this->value;}
  virtual inline int to_int() const override {return this->value;}
  virtual inline double to_double() const override {return this->value;}

};


/* =============================================================================
   NULL
============================================================================= */
class null_t final : public node_t {
public:
  null_t() : node_t(node_kind_t::_NULL) {}

  inline virtual null_t* copy() const override {return new null_t();}

  inline virtual null_t* null() override {return this;}

  inline virtual null_t const* null() const override {return this;}
};



}// end of namespace awali::json



}// end of namespace awali

namespace std {
std::ostream& operator<<(std::ostream& o, awali::json::uint_or_string_t const& uos);
std::ostream& operator<<(std::ostream& o, awali::json::path_t const& path);
}

#endif
