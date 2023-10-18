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

#ifndef COMMON_JSON_VISITOR_HH
#define COMMON_JSON_VISITOR_HH

#include <cstddef>

#include <awali/common/json/node.hh>
#include <awali/common/json_ast.hh>

namespace awali {
namespace json {

enum class visit_t {
  IGNORE_UNVISITED = -1,
  CONTINUE = 0,
  IGNORE_UNVISITED_CHILDREN = 1,
  IGNORE_UNVISITED_CHILDREN_AND_SIBLINGS = 2
};

/** Abstract class that helps implement a visitor of 
 * {@link json_ast_t json tree} or {@link node_t subtree}.
 *  Usually some of the following method are overriden:
 *  - enter
 *  - leave
 *  - leaf
 *  - before_child
 *  - after_child
 *  - between_children
 *  The case covered by these function sometimes "overlap", in order to suit
 *  the user needs. You do not have to override all to override all methods.
 *
 *  The `visit` functions may be overriden at your own risk.
 *
 *  The function {@link visitor_t::stop()} can be called from anywhere to stop
 *  the visit.
 *
 * Here is a short description of the order in which functions are called: 
 *  - enter(node_t)
 *  - enter(explicit node type)
 *  - if n is a leaf `leaf(explicit data type)`
 *  - otherwise n is an inner node:
 *    > before_child on 1st child (general, then specialized)   
 *    > ..recursive calls
 *    > afterChildren with 1st child
 *    > between_children (general, then specialized)
 *    > before_children with 2nd child (general, then specialized)
 *    > .. treatment of other children is similar
 *    > after_child on penultimate child (general, then specialized)
 *    > between_children (general, then specialized)
 *    > before_child with last child (general, then specialized)
 *    > ..visit last child
 *    > after_child with last child (general, then specialized)
 *  - leave (explicit node type)
 *  - leave (node_t)
 */
class visitor_t {
private:
  visit_t _stop;
  json::path_t _path_from_root;
  template<typename L> inline void visit_leaf(L* node) 
  { enter(node); if (!is_stopped()) leaf(node->value); leave(node);}

public:
  inline void stop(visit_t b = visit_t::IGNORE_UNVISITED) {_stop=b;}
  inline void step_stop() {
    switch(_stop) {
      case visit_t::IGNORE_UNVISITED_CHILDREN:
        _stop = visit_t::CONTINUE;
        break;
      case visit_t::IGNORE_UNVISITED_CHILDREN_AND_SIBLINGS:
        _stop = visit_t::IGNORE_UNVISITED_CHILDREN;
        break;
      default:{}
    }
  }
  inline bool is_stopped() {return visit_t::CONTINUE != _stop;}
  inline visit_t stop_status() {return _stop;}
  inline json::path_t const& path() {return _path_from_root; }
  inline unsigned depth() {return _path_from_root.size(); }

protected:
  visitor_t() : _stop(visit_t::CONTINUE), _path_from_root() {}

  /* Part of these functions are supposed to be overriden. */

  /** Very first function called when visiting a node. 
   * Called for every node kind.
   */
  virtual void enter(node_t const*) {}


  /** Function called when starting visiting a {@link bool_t}, just after
   * the kind-independent {@link enter (node_t const*)}, and just before 
   * {@link leaf(bool)}.
   */
  virtual void enter(bool_t const*) {}
  

  /** Function called when starting visiting an {@link object_t}, just
   * after the kind-independent {@link enter (node_t const*)}.
   */
  virtual void enter(object_t const*) {}
  
  /** Function called when starting visiting an {@link array_t}, just
   * after the kind-independent {@link enter (node_t const*)}.
   */
  virtual void enter(array_t const*) {}

  /** Function called when starting visiting a {@link bool_t}, just
   * after the kind-independent {@link enter (node_t const*)}, and just before 
   * {@link leaf(int)}.
   */
  virtual void enter(int_t const*) {}
  
  /** Function called when starting visiting a {@link string_t}, just
   * after the kind-independent {@link enter (node_t const*)}, and just before 
   * {@link leaf(std::string const&)}.
   */
  virtual void enter(string_t const*) {}
  
  /** Function called when starting visiting a {@link float_t}, just
   * after the kind-independent {@link enter (node_t const*)}, and just before 
   * {@link leaf(double)}.
   */
  virtual void enter(float_t const*) {}

  /** Function called when starting visiting a {@link null_t}, just
   * after the kind-independent {@link enter (node_t const*)}, and just before 
   * {@link leaf(std::nullptr_t)}.
   */
  virtual void enter(null_t const*) {}


  /** Function called when visiting a {@link int_t}, after 
   * {@link enter (int_t const*)} but before {@link leave(int_t const*)}.
   */
  virtual void leaf(int) {}

  /** Function called when visiting a {@link string_t}, after 
   * {@link enter (string_t const*)} but before {@link leave(string_t const*)}.
   */
  virtual void leaf(std::string const&) {}

  /** Function called when visiting a {@link float_t}, after 
   * {@link enter (float_t const*)} but before {@link leave(float_t const*)}.
   */
  virtual void leaf(double) {}

  /** Function called when visiting a {@link bool_t}, after 
   * {@link enter (bool_t const*)} but before {@link leave(bool_t const*)}.
   */
  virtual void leaf(bool) {}
  
  /** Function called when visiting a {@link null_t}, after 
   * {@link enter (null_t const*)} but before {@link leave(null_t const*)}.
   */
  virtual void leaf(std::nullptr_t) {}


  /** Function called just before and just after visiting a child node. */
  virtual void before_child(object_t const*, std::string const&,
                            node_t const*) {}

  virtual void after_child(object_t const*, std::string const&,
                           node_t const*) {}
  

  virtual void before_child(array_t const*, unsigned,
                            node_t const*) {}

  virtual void after_child(array_t const*, unsigned,
                           node_t const*) {}

  /** Function called just before the specialized version with the same name */
  virtual void before_child(node_t const*, uint_or_string_t const&,
                           node_t const*) {}

  /** Function called just after the specialized version with the same name */
  virtual void after_child(node_t const*, uint_or_string_t const&,
                           node_t const*) {}

  /* These functions are called in between */

  /** Function called between visiting two children, before the function with
   * the same name and specialized argument type.
   *  If @pname{node} has only one children, this will never be called.
   * */
  virtual void between_children(node_t const*) {}

  /** Function called between visiting two children, after the function with
   * the same name and argument type {@link node_t}.
   *  If @pname{node} has only one children, this function is not called.
   * */
  virtual void between_children(object_t const*) {}
  
  /** Function called between visiting two children, after the function with
   * the same name and argument type {@link node_t}.
   *  If @pname{node} has only one children, this function is not called.
   * */
  virtual void between_children(array_t const*) {}

  /** This function is called for every node type. */
  virtual void leave(node_t const*) {}

  virtual void leave(bool_t const*) {}
  virtual void leave(object_t const*) {}
  virtual void leave(array_t const*) {}
  virtual void leave(int_t const*) {}
  virtual void leave(string_t const*) {}
  virtual void leave(float_t const*) {}
  virtual void leave(null_t const*) {}

  /* These functions are usually not overriden. */
  inline virtual void visit(bool_t const* node) {visit_leaf(node);}
  inline virtual void visit(int_t const* node) {visit_leaf(node);}
  inline virtual void visit(string_t const* node) {visit_leaf(node);}
  inline virtual void visit(float_t const* node) {visit_leaf(node);}
  inline virtual void visit(null_t const* node) 
  {enter(node); if (!is_stopped()) leaf(nullptr); leave(node);}

  virtual void visit(object_t const* node);
  virtual void visit(array_t const* node);
  virtual void visit(node_t const* node);

public:
  inline virtual void run(json_ast_t tree) { run (&(*tree)); }
  inline virtual void run(node_t const* node) { visit(node); }

  virtual ~visitor_t() = default;
};


} // end of namespace json
} // end of namespace awali
#endif
