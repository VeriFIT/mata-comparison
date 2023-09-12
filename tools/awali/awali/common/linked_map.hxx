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

#ifndef AWALI_LINKED_MAP_HXX
#define AWALI_LINKED_MAP_HXX

#include<awali/common/is_iterable.hxx>

#include <unordered_map>
#include <list>
#include <stdexcept>
#include <sstream>

namespace awali {

/** This class takes a hasher for type `K` and makes a hasher for type `K
  * const*` that hash the pointed value and not the pointer.
  * The hash of a `nullptr` is 0.
  */
template <typename K, typename H = std::hash<K>>
class pointed_hash_t : protected H {
public:
  std::size_t operator()(K* key) const noexcept {
    return (key == nullptr) ? 0 : H::operator()(*key);
  }
};

/** This class takes a type `E` that computes equality for type `K` and allows
  * to compute equality for type `K const*` by calling deferencing both values
  * and calling `E` on them.
  * A `nullptr` is considered equal to itself and different to other pointers.
 */
template <typename K, typename E = std::equal_to<K>>
class pointed_equal_t : protected E {
public:
  bool operator()(K* key1, K* key2) const noexcept {
    return (key1 == nullptr || key2 == nullptr) ? (key1 == key2) 
                                                : E::operator()(*key1,*key2);
  }
};


/** Implemention of a linked hash-map.
 *
 * The main structure is in field {@link list}, a `std::list` of 
 * `std::pair<K const, V>` (const is there to avoid user to break the structure
 * invariant).
 * The iterators of a `{@link linked_map_t} (`iterator` and `const_iterator`)
 * are directly those of {@link list}.
 * The secondary structure is {@link map}, an `std::unordered_map` that maps
 * keys of type `K*` to iterators of the list above. The map uses the classes
 * {@link pointed_hash_t} and {@link pointed_equal_t} to hash its keys: they
 * wrap the possibility user-defined `H` and `E` in order to compute
 * equality and hash based on the pointed value (of type `K`) instead of the
 * pointer value (of type `K*`).
 * */
template <typename K, typename V, 
          typename H = std::hash<K>, typename E = std::equal_to<K>>
class linked_map_t {
public:
  typedef linked_map_t<K,V,H,E> self_type_t;
  typedef K key_t;
  typedef V value_t;
  typedef std::pair<key_t const,value_t> pair_t;
  typedef pair_t value_type;
  typedef std::pair<key_t,value_t> init_pair_t;
  typedef std::list<pair_t> list_t;
  typedef typename std::list<pair_t>::iterator iterator;
  typedef typename std::list<pair_t>::const_iterator const_iterator;
  typedef std::unordered_map< key_t const*, 
                              iterator,
                              pointed_hash_t<key_t const,H>,
                              pointed_equal_t<key_t const,E> > map_t;
  typedef typename map_t::size_type size_type;

protected:
  map_t map;
  list_t list;

  /** Recompute {@link map} from {@link list}. 
   * `hard` tells whether the {@link list} may contain key duplicates:
   * - if @pname{hard} is `true`: duplicates key are removed from {@link list}
   *   might have duplicates and only the last binding is kept.
   * - if @pname{hard} is `false`: {@link list} is not verified for key
   *   duplicates; if it has duplicated, this {@link linked_map_t} is in an
   *   inconsistent state.
   */
  void remap(bool hard) {
    map.clear();
    map.reserve(list.size());
    for(auto it = list.begin(); it != list.end(); it++) {
      if (hard) {
        auto x = map.find(&(it->first));
        if (x != map.end())
          erase(x->second);
      }
      map[&(it->first)]=it;
    }
  }

public:
  /** Returns if this {@link linked_map_t} is empty. */
  bool empty() const noexcept { return list.empty(); }

  /** Returns the number of bindings in this {@link linked_map_t}. */
  size_type size() const noexcept { return list.size(); }

  size_type max_size() const noexcept { return map.max_size(); }

  /** Clears this {@link linked_map_t}. */
  void clear() noexcept { map.clear(); list.clear(); }

  /** Accessor directly inherited from std::unordered_map. */
  size_type bucket_count() const noexcept {return map.bucket_count();}
  /** Accessor directly inherited from std::unordered_map. */
  size_type bucket_size(unsigned n) const noexcept {return map.bucket_size(n);}
  /** Accessor directly inherited from std::unordered_map. */
  size_type max_bucket_count() const noexcept {return map.max_bucket_count();}
  /** Accessor directly inherited from std::unordered_map. */
  float load_factor() const noexcept {return map.load_factor();}
  /** Accessor directly inherited from std::unordered_map. */
  float max_load_factor() const noexcept {return map.max_load_factor();}
  /** Accessor directly inherited from std::unordered_map. */
  void max_load_factor(float x) {map.max_load_factor(x);}
  /** Accessor directly inherited from std::unordered_map. */
  void rehash(size_type n) {map.rehash(n);}
  /** Accessor directly inherited from std::unordered_map. */
  void reserve(size_type n) {map.reserve(n);}

  iterator begin() noexcept {return list.begin();}
  iterator end() noexcept {return list.end();}

  const_iterator begin() const noexcept {return cbegin();}
  const_iterator end() const noexcept {return cend();}

  const_iterator cbegin() const noexcept {return list.cbegin();}
  const_iterator cend() const noexcept {return list.cend();}


  /** Returns the position corresponding to the binding of @pname{key}. 
   * Returns {@link end()} if key is unbound.
   */
  iterator
  find(key_t const& key)
  {
    auto res = map.find(&key);
    if (res == map.end())
      return list.end();
    else
      return res->second;
  }
  
  /** Returns the position corresponding to the binding of @pname{key}. 
   * Returns {@link end()} if key is unbound.
   */
  const_iterator
  find(key_t const& key)
  const
  {
    auto res = map.find(&key);
    if (res == map.end())
      return list.cend();
    else
      return res->second;
  }


  /** Returns the number of binding associated with @pname{key}, that is 0 or 1.
   */
  size_type count(key_t const& key) 
  const 
  {
    return map.find(&key) != map.end();
  }





  /** Inserts a new @pname{key}/@pname{value} binding before @pname{pos};
   *  deletes existing binding for @pname{key} prior to insertion if
   *  @pname{overwrite_existing} is `true`.
   * If key was unbound, insert binding before `pos`.
   * If key was bound and `overwrite_existing` is `true`, delete old binding
   * and inserts the new binding before `pos`.
   * If key was bound and `overwrite_existing` is `false`, do nothing.
   */
  iterator insert(const_iterator pos, key_t key, value_t value,
                  bool overwrite_existing = false)
  {
    auto it = map.find(&key);
    if (it != map.end()) {
      if (overwrite_existing) {
        list.erase(it->second);
        map.erase(it);
      }
      else
        return it->second;
    }
    auto res = list.insert(pos, pair_t(std::move(key),std::move(value)));
    map.emplace(&(res->first),res);
    return res;
  }


  /** Inserts a new @pname{key}/@pname{value} binding at the front of this
   *  {@link linked_map_t}; deletes existing binding for @pname{key} prior to
   *  insertion if @pname{overwrite_existing} is `true`.
   * Alias for `insert(begin(),key,value,overwrite_existing)`
   */
  void push_front(key_t key, value_t value,
                  bool overwrite_existing = false)
  {
    insert(begin(), std::move(key), std::move(value), overwrite_existing);
  }


  pair_t& front()  { return list.front(); }
  pair_t const& front() const  { return list.front(); }


  pair_t& back()  { return list.back(); }
  pair_t const& back() const  { return list.back(); }


  /** Inserts a new @pname{key}/@pname{value} binding at the back of this
   * {@link linked_map_t};
   *  deletes existing binding for @pname{key} prior to insertion if
   *  @pname{overwrite_existing} is `true`.
   * Alias for `insert(end(),key,value,overwrite_existing)`
   */
  void push_back(key_t key, value_t value, bool overwrite_existing = false)
  {
    insert(end(), std::move(key), std::move(value), overwrite_existing);
  }


  /* Erases the binding pointed at by given iterator.
   * Does nothing if @pname{pos} is equal to {@link end()}.
   * @return an iterator pointing to the next binding.
   */
  iterator
  erase(const_iterator pos)
  {
    if (pos == this->cend())
      return this->end();
    map.erase(&(pos->first));
    return list.erase(pos);
  }


  /** Deletes the value bound to @pname{key}.
   * @return the number of binding
   * deleted (1 if @pname{key} was bound, 0 if it was not).*/
  size_type
  erase(key_t const& key)
  {
    auto it = map.find(&key);
    if (it == map.end())
      return 0;
    const_iterator it2 = it->second; // Somehow this is required because 
    map.erase(it);                   // unordered_map tries to hash
    list.erase(it2);                 // its key before erasing from iterator.
    return 1;
  }


  /** Access value bound to @pname{key}. 
   *
   * @throws std::out_of_range if @pname{key} is unbound.*/
  value_t&
  at (key_t const& key)
  {
    auto it = map.find(&key);
    if (it != map.end())
      return it->second->second;
    std::stringstream ss;
    ss << "This linked_map does not have a binding with key" << key << ".";
    throw std::out_of_range(ss.str());
  }


  /** Access value bound to @pname{key}. 
   *
   * @throws std::out_of_range if @pname{key} is unbound.*/
  value_t const&
  at (key_t const& key)
  const
  {
    auto it = map.find(&key);
    if (it != map.end())
      return it->second->second;
    std::stringstream ss;
    ss << "This linked_map does not have a binding with key" << key << ".";
    throw std::out_of_range(ss.str());
  }



  /** Access, modify or set the value bound to @pname{key}.
   * If @pname{key} is unbound before the call, a new binding is inserted at
   * the end of this linked_map. It associates @pname{key} to the value
   * resulting from the default value_t constructor, and returns a reference to
   * it. 
   * Typical usage is `map[key] = value` where key is of type {@link key_t} and
   * value of type {@link value_t}.
   */
  value_t&
  operator[] (key_t key)
  {
    auto it = map.find(&key);
    if (it != map.end())
      return it->second->second;
    list.emplace_back(pair_t(std::move(key),value_t()));
    iterator x = --list.end();
    map[&(x->first)] = x;
    return x->second;
  }


  /** Sort this {@link linked_map_t} by key assuming `operator<` exists on
   * {@link key_t}. */
  void
  sort()
  {
    list.sort([](pair_t x,pair_t y){return (x.first < y.first);});
    remap(false);
  }


  /** Sort this {@link linked_map_t} by given comparison. */
  template <class Compare>
  void sort(Compare comp)
  {
    list.sort(comp);
    remap(false);
  }


  linked_map_t() {}

  linked_map_t(std::initializer_list<pair_t> l) : list(std::move(l)) 
  {remap(true);}
  
  linked_map_t(std::list<pair_t> l) : list(std::move(l)) 
  {remap(true);}

  linked_map_t(linked_map_t<key_t,value_t> const& other)
  : list(other.list), map() {remap(false);}

  linked_map_t(linked_map_t<key_t,value_t>&& other)
  : list(std::move(other.list)), map(std::move(other.map)) {}
                             //  ^^^^^^^^^^^^^^^^^^^^^^^^
                             //  This assumes that the iterators are still valid
                             //  after moving a std::list                   
  

  /** Universal constructor to build a {@link linked_map_t} from any kind of
   * iterable containing `std::pair<std::string,std::string>`'s.
   * If provided with an rvalue reference, it moves the elements out of it (so
   * the container does not change size, but its elements are emptied).
   * If provided with an lvalue reference, it copies each pair from it.
   * @param container Container to copy or move the binding pairs from
   * @param require_remap If set to `false`, the first elements in the pairs in
   * @pname{container} are assumed to be unique; if it is not the case, the
   * produced {@link linked_map_t} is inconsistent. If set to `true`, only the
   * last binding for a given key is kept.
   * extra checks.  */
  template<class O>
  linked_map_t(
    O&& container,  // /!\ This is a universal reference, not a rvalue reference
    typename std::enable_if<
      is_iterable_with<O,init_pair_t>::value,bool>::type require_remap = true)
  {
    bool b = std::is_lvalue_reference<O>::value; 
    for (auto& pair : container) {
      if (b)                              // This is clearly a case
        list.push_back(pair);             // where C17 `if constexpr` would
      else                                // be beneficial.
        list.push_back(std::move(pair));  //
    }
    remap(require_remap);
  }
  


  /** Returns `true` if this {@link linked_map_t} is different from
   * @pname{other}.
   *
   * Alias for `!( (*this) ==  other)`. */
  bool operator!=(self_type_t const& other) 
  {return !( (*this) ==  other);}


  /** Returns `true` if this {@link linked_map_t} is equal to
   * @pname{other}, when both are considered as lists of pairs.   
   * - Order of bindings is taken into account.
   * - Equality for keys is tested with class template parameter  @p E.
   * - Equality for values is tested with `operator==`.
   *
   * Same as {@link equals_as_list} with default values for template parameters.
   */
  bool operator==(self_type_t const& other) {
    return this->equals_as_list(other);
  }


  /** Returns `true` if this {@link linked_map_t} is equal to
   * @pname{other}, when both are considered as lists of pairs.
   * - Order of bindings is taken into account.
   * - Equality for keys is tested with template parameter @pname{KeyEquality}.
   * - Equality for values is tested with @pname{ValueEquality}.
   */
  template<typename KeyEquality = E, typename ValueEquality = std::equal_to<V>>
  bool equals_as_list(self_type_t const& other) const
  {
    KeyEquality ke;
    ValueEquality ve;
    const_iterator it1 = begin();
    const_iterator it2 = other.begin();
    while (it1 != end() && it2 != other.end()) {
      if (!ke(it1->first,it2->first))
        return false;
      if (!ve(it1->second,it2->second))
        return false;
      ++it1;
      ++it2;
    }
    return (it1 == end() && it2 == other.end());
  }

  /** Returns `true` if the bindings in this {@link linked_map_t} are exactly
   *  those in @pname{other}, when considered as mapsr.
   *
   * - Order of bindings is ignored.
   * - Equality for keys is tested with class template parameter @pname{E}: 
   *   each key bound in @pname{other} is looked up in this.
   * - Equality for values is tested by an object of type @pname{ValueEquality},
   *   built using the default constructor.
   */
  template<typename ValueEquality = std::equal_to<V>>
  bool equals_as_map(self_type_t const& other) const
  {
    ValueEquality ve;
    if (this->size() != other.size())
      return false;
    const_iterator end = this->end();
    for(pair_t const& pair: other) {
      const_iterator it = this->find(pair.first);
      if (it == end)
        return false;
      if (!ve(pair.second,it->second))
        return false;
    }
    return true;
  }
};



} // end of namespace awali

#endif
