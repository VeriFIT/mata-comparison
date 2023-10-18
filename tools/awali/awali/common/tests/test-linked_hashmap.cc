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

#include <string>
#include <iostream>
#include <vector>
#include <awali/common/linked_map.hxx>

class false_equaller_t {
public:
  template<typename V>
  bool operator()(V const&, V const&) {
    return true;
  }
};

struct some_type_t {
  std::string s;
  long i;

  bool operator== (some_type_t const& other) const {
    return (other.s == s && other.i == i);
  }
  
  /* required for sorting */
  bool operator< (some_type_t const& other) const {
    return (s < other.s || (other.s == s &&  i < other.i));
  }
};

namespace std {
template<>
struct hash<some_type_t> {
  std::hash<std::string> hs;
  std::hash<long> hl;
  long unsigned operator()(some_type_t const& x) const {
    return hs(x.s)+hl(x.i);
  }
};
}


std::ostream& operator<<(std::ostream& out, some_type_t t) {
  return out << "(" << t.s << "," << t.i<<")";
}

namespace awali {



/* The key<T>(i) is expected to be monotonous with respect to T::operator<(T) */

template<typename T>
T key(unsigned i) {
  return new T();
}

template<typename T>
T value(unsigned i) {
  return new T();
}

template<typename T>
std::string to_string(T val) {
  std::stringstream ss;
  ss << val;
  return ss.str();
}

template<>
std::string key<std::string>(unsigned i) {
  std::stringstream ss;
  ss << "key-" << i;
  return ss.str();
}

template<>
std::string value(unsigned i) {
  std::stringstream ss;
  ss << "value-" << i;
  return ss.str();
}


template<>
some_type_t key<some_type_t>(unsigned i) {return {"key",i};}

template<>
some_type_t value<some_type_t>(unsigned i) {return {"value",i};}

template<>
int key<int>(unsigned i) {return i;}

template<>
int value<int>(unsigned i) {return -i;}


template<>
int* value<int*>(unsigned i) {
  static std::vector<int> vect;
  if (vect.size() < i)
    for (unsigned j = vect.size(); j<=i; j++)
      vect.push_back(value<int>(j));
  return &(vect[i]);
}

template<>
int* key<int*>(unsigned i) {
  static std::vector<int> vect;
  if (vect.size() < i)
    for (unsigned j = vect.size(); j<=i; j++)
      vect.push_back(key<int>(j));
  return &(vect[i]);
}


template<typename key_t, typename value_t> 
class tester_t {

  typedef awali::linked_map_t<key_t,value_t> map_t;
  typedef std::pair<key_t,value_t> pair_t;


  key_t key(unsigned i) {
    return awali::key<key_t>(i);
  }

  value_t value(unsigned i) {
    return awali::value<value_t>(i);
  }

  map_t make_example_map() 
  {
    return { {key(0),value(0)},
             {key(1),value(1)},
             {key(2),value(2)},
             {key(3),value(3)},
             {key(4),value(4)},
             {key(5),value(5)},
             {key(6),value(6)},
             {key(7),value(7)},
             {key(8),value(8)},
             {key(9),value(9)}  };
  }
  
  map_t make_scrambled_map() 
  {
    return { {key(0),value(0)},
             {key(2),value(2)},
             {key(4),value(4)},
             {key(6),value(6)},
             {key(8),value(8)},
             {key(1),value(1)},
             {key(3),value(3)},
             {key(5),value(5)},
             {key(7),value(7)},
             {key(9),value(9)}  };
  }

  std::vector<pair_t> values;
  unsigned front = 0;
  unsigned back = 9;
  unsigned size = 10;

  void test_size() 
  {
    std::cout << "  " << __func__ << std::endl;
    map_t map = make_example_map();
    if (map.size() != size)
      throw std::runtime_error("test_size: map has size "
        +std::to_string(map.size())+"expected to be of size "+to_string(size)
        +".");
  }


  void test_count() 
  {
    std::cout << "  " << __func__ << std::endl;
    map_t map = make_example_map();
    for (unsigned i = 0; i< size; i++)
      if (map.count(key(i)) != 1)
        throw std::runtime_error(
          "test:count: map declares it does not have a binding for: \"" 
          + to_string(key(i)) + "\".");
    for (unsigned i = size; i< 2*size; i++)
      if (map.count(key(i)) != 0)
        throw std::runtime_error(
          "test:count: map declares having a binding: \""
          + to_string(key(i)) + "\".");
  }


  void test_find() 
  {
    std::cout << "  " << __func__ << std::endl;
    map_t map = make_example_map();
    for (unsigned i = front; i< size; i++) {
      auto it = map.find(key(i));
      if (it == map.end())
        throw std::runtime_error("test_find:: Did not find key \"" 
        + to_string(key(i))+"\".");
      if (!(it->second == value(i)))
        throw std::runtime_error("test_find:: value found \"" 
        + to_string(it->second)+"\" != \""+to_string(value(i))+"\".");
    }

    for (unsigned i = size; i< 2*size; i++)
      if (!(map.find(key(i)) == map.end()))
        throw std::runtime_error("test_find:: Found key \"" 
        + to_string(key(i)) + "\".");
    

    map_t const& map2 = map;
    for (unsigned i = front; i< size; i++) {
      auto it = map2.find(key(i));
      if (it == map2.end())
        throw std::runtime_error("test_find:: Did not find key \""
        + to_string(key(i))+"\" (const).");
      if (!(it->second == value(i)))
        throw std::runtime_error("test_find:: value found \""
        +to_string(it->second)+"\" != \""+to_string(value(i))+"\" (const).");
    }

    for (unsigned i = size; i< 2*size; i++)
      if (!(map2.find(key(i)) == map2.end()))
        throw std::runtime_error("test_find:: Found key \""+to_string(key(i))
        +"\".");
  }

  std::vector<pair_t> 
  test_iterator() 
  {
    std::cout << "  " << __func__ << std::endl;
    map_t map = make_example_map();
    std::vector<pair_t> res;
    for (auto pair : map)
      res.push_back(pair);
    if (res.size() != size)
      throw std::runtime_error("test-iterator::count");
    for (unsigned i = front; i<size; i++) {
      if (!(res[i].first == key(i)))
        throw std::runtime_error("test-iterator::key at " + to_string(i) 
        + ": \"" + to_string(res[i].first) + "\" != \"" + to_string(key(i)) 
        + "\"");
      if (!(res[i].second == value(i)))
        throw std::runtime_error("test-iterator::value at "+to_string(i)+": \""
        + to_string(res[i].second)+"\" != \""+to_string(value(i))+"\".");
    }
    return res;
  }


  void
  test_constructor()
  {
    std::cout << "  " << __func__ << std::endl;
    std::vector<pair_t> vect = {{key(2),value(2)}, {key(5),value(5)},
                                {key(2),value(3)}};// one value will overwrite 
                                                   // the other 

    map_t map1(vect);
    if (map1.size() != 2)
      throw std::runtime_error("test-constructor::generic: vector");
    
    std::vector<pair_t> const& vect2 = vect;
    map_t map1b(vect2);
    if (map1b.size() != 2)
      throw std::runtime_error("test-constructor::generic: vector const&");

    map_t map2(std::move(vect));
    if (map2.size() != 2)
      throw std::runtime_error("test-constructor::generic: vector&&");
    

    std::list<pair_t> list = {{key(2),value(2)}, {key(5),value(5)},
                              {key(2),value(3)}}; // one value will overwrite
                                                  // the other 
    map_t map3(list);
    if (map3.size() != 2)
      throw std::runtime_error("test-constructor::generic: list const&");
    map_t map4(std::move(list));
    if (map4.size() != 2)
      throw std::runtime_error("test-constructor::generic: list&&");
  }


  void
  test_insert() {
    std::cout << "  " << __func__ << std::endl;
    map_t map1 = make_example_map();
    auto it = map1.find(key(5));
    map1.insert(it, key(14), value(14));
    map1.insert(it, key(8), value(18), false);
    map1.insert(it, key(9), value(19), true);

    map_t map2;
    std::list<std::pair<unsigned,unsigned>> list 
      = { {0,0}, {1,1}, {2,2}, {3,3}, {4,4}, {14,14}, {9,19}, {5,5} , {6,6}, 
          {7,7}, {8,8} };
    for (std::pair<unsigned,unsigned> i : list)      
      map2.push_back(key(i.first), value(i.second));
    if (map1 != map2) {
     for (auto& pair : map1)
        std::cout << pair.first << ": " << pair.second << std::endl;
     std::cout << "==" << std::endl;
     for (auto& pair : map2)
        std::cout << pair.first << ": " << pair.second << std::endl;
     throw std::runtime_error("test_insert:: failed."); 
    }
  }

  void
  test_sort() {
    std::cout << "  " << __func__ << std::endl;
    map_t map1 = make_example_map();
    map_t map2 = make_scrambled_map();
    map2.sort();
    if (map1 != map2) {
     for (auto& pair : map1)
        std::cout << pair.first << ": " << pair.second << std::endl;
     std::cout << "==" << std::endl;
     for (auto& pair : map2)
        std::cout << pair.first << ": " << pair.second << std::endl;
     throw std::runtime_error("test_sort:: failed."); 
    }
  }


  void 
  test_back() {
    std::cout << "  " << __func__ << std::endl;
    map_t map = make_example_map();
    if (!(map.back().first == key(back)))
        throw std::runtime_error("test_back::key at back is "
          + to_string(map.back().first) + ". Expecting \"" 
          + to_string(key(back)));
    if (!(map.back().second == value(back)))
        throw std::runtime_error("test_back::key at back is "
          + to_string(map.back().first) + ". Expecting \"" 
          + to_string(value(back)));

    map_t const& map_ref = map;
    if (!(map_ref.back().first == key(back)))
        throw std::runtime_error("test_back::key at back (const) is "
          + to_string(map_ref.back().first) + ". Expecting \"" 
          + to_string(key(back)));
    if (!(map_ref.back().second == value(back)))
        throw std::runtime_error("test_back::key at back (const) is "
          + to_string(map_ref.back().first) + ". Expecting \"" 
          + to_string(value(back)));
  }



  void 
  test_push_back() 
  {
    std::cout << "  " << __func__ << std::endl;
    map_t map = make_example_map();
    for (unsigned i = size; i<2*size; i++) {
      map.push_back(key(i),value(i));
      if (!(map.back().first == key(i)))
        throw std::runtime_error("test_push_back::key at back is \""
          + to_string(map.back().first) + "\". Expecting \"" 
          + to_string(key(i))+"\".");
      if (!(map.back().second == value(i)))
        throw std::runtime_error("test_push_back::key at back is \""
          + to_string(map.back().second) + "\". Expecting \"" 
          + to_string(value(i))+"\".");
    }
  }



  void 
  test_front() 
  {
    std::cout << "  " << __func__ << std::endl;
    map_t map = make_example_map();
    if (!(map.front().first == key(front)))
        throw std::runtime_error("test_front::key at front is "
          + to_string(map.front().first) + ". Expecting \""
          + to_string(key(front)));
    if (!(map.front().second == value(front)))
        throw std::runtime_error("test_front::key at front is "
          + to_string(map.front().first) + ". Expecting \""
          + to_string(value(front)));

    map_t const& map_ref = map;
    if (!(map_ref.front().first == key(front)))
        throw std::runtime_error("test_front::key at front (const) is "
          + to_string(map_ref.front().first) + ". Expecting \"" 
          + to_string(key(front)));
    if (!(map_ref.front().second == value(front)))
        throw std::runtime_error("test_front::key at front (const) is "
          + to_string(map_ref.front().first) + ". Expecting \"" 
          + to_string(value(front)));
  }


  void
  test_at() 
  {
    std::cout << "  " << __func__ << std::endl;
    map_t map = make_example_map();
    for (unsigned i=front; i <size; i++) {
      value_t& s = map.at(key(i));
      if ( !(s ==  value(i)))
          throw std::runtime_error(
            "test_at:: at("+to_string(i)+") before change is "
            + to_string(s) + ". Expecting \""+to_string(value(i)));
      s = value(i+size);
    }
    for (unsigned i=front; i <size; i++) {
      value_t& s = map.at(key(i));
      if (!(s ==  value(i+size)))
          throw std::runtime_error(
            "test_at:: at("+std::to_string(i)+") after change is "
            + to_string(s) + ". Expecting \""+ to_string(value(i+size)));
    }

  }


  void
  test_brackets()
  {
    std::cout << "  " << __func__ << std::endl;
    map_t map = make_example_map();
    for (unsigned i=size; i <2*size; i++) {
      map[key(i)] = value(i);
    }
    if (map.size() != 2*size)
      throw std::runtime_error("test_brackets:: some assignation failed.");

    for (unsigned i=front; i <size; i++) {
      value_t& s = map[key(i)];
      if (!(s ==  value(i)))
          throw std::runtime_error("test_brackets:: map["+to_string(i)+"] is "
            + to_string(s) + ". Expecting \""+ to_string(value(i)));
    }
    
      
  }



  void 
  test_push_front() 
  {
    std::cout << "  " << __func__ << std::endl;
    map_t map = make_example_map();
    for (unsigned i = 11; i<21; i++) {
      map.push_front(key(i),value(i));
      if (!(map.front().first == key(i)))
        throw std::runtime_error("test_push_front::key at front is "
          + to_string(map.front().first) + ". Expecting \"" 
          + to_string(key(i))+"\".");
      if (!(map.front().second == value(i)))
        throw std::runtime_error("test_push_front::value at front is "
          + to_string(map.front().second) + ". Expecting \"" 
          + to_string(value(i))+"\".");
    }
  }


  void
  test_erase_key()
  {
    std::cout << "  " << __func__ << std::endl;
    map_t map = make_example_map();
    for (unsigned i = front; i<size; i+=2)
      map.erase(key(i));
    unsigned i = 1, j= 0;
    for (auto pair : map) {
      if (!(pair.first == key(i)))
        throw std::runtime_error("test_erase_key::key at " + to_string(j)
          + " is  \""+to_string(pair.first)+"\". Expecting \"" 
          + to_string(key(i))+"\".");
      if (!(pair.second == value(i)))
        throw std::runtime_error("test_erase_key::value at " + to_string(j)
          + " is  \""+to_string(pair.second)+"\". Expecting \"" 
          + to_string(value(i))+"\".");
      i += 2;
      j++;
    }
  }


  void
  test_erase_it()
  {
    std::cout << "  " << __func__ << std::endl;
    map_t map = make_example_map();
    typename map_t::iterator it = map.begin();
    for (unsigned i = front; i<size; i+=2) {
      it = map.erase(it);
      it++;
    }
    unsigned i = 1, j= 0;
    for (auto pair : map) {
      if (!(pair.first == key(i)))
        throw std::runtime_error("test_erase_key::key at " + to_string(j)
          + " is  \""+to_string(pair.first)+"\". Expecting \"" 
          + to_string(key(i))+"\".");
      if (!(pair.second == value(i)))
        throw std::runtime_error("test_erase_key::value at " + to_string(j)
          + " is  \""+to_string(pair.second)+"\". Expecting \"" 
          + to_string(value(i))+"\".");
      i += 2;
      j++;
    }

  }



  void 
  test_equality() 
  {
    std::cout << "  " << __func__ << std::endl;
    map_t map1 = make_example_map();
    map_t map2 = make_example_map();
    map_t map3 = make_scrambled_map();
    
    if (map1 != map2)
      throw std::runtime_error("test_equality:: map1 != map2");
    if (!map3.equals_as_map(map2))
      throw std::runtime_error("test_equality:: !map3.equals_as_map(map2)");
    if (map3 == map2)
      throw std::runtime_error("test_equality:: map3 == map2");

    map2.push_back(key(size),value(size));
    if (map1 == map2) 
      throw std::runtime_error("test_equality:: map1 == map2 (new value)");
    if (map3.equals_as_map(map2)) 
      throw std::runtime_error("test_equality:: map3.equals_as_map(map2) (new value)");

    map2.erase(key(size));
    map2[key((front+back)/2)] = value(size);
    if (map1 == map2) 
      throw std::runtime_error("test_equality:: map == map (diff binding)");
    if (map3.equals_as_map(map2)) 
      throw std::runtime_error("test_equality:: map3.equals_as_map(map2) (diff binding)");


    if (!(map3.template equals_as_list< false_equaller_t,
                                        false_equaller_t >(map1))) {
      throw std::runtime_error("test_equality:: false_equality");
    }

  }


public:
  void
  run() {
    map_t map = make_example_map();
    test_size();
    test_count();
    values = test_iterator();

    test_front(); /* getters */
    test_back();
    test_find();

    test_push_front(); /* adders */
    test_push_back();
    test_insert();

    test_at(); /* getter+modifier */
    test_brackets();

    test_erase_key();
    test_erase_it();

    test_equality();
    test_sort();


    test_constructor();
  }

};// end of tester_t


}// end of namespace awali

int 
main (int argc, char** argv) 
{
  std::cout << std::endl << "std::string -> int" << std::endl;
  awali::tester_t<std::string,int> tester1;
  tester1.run();
  
  std::cout << std::endl << "int -> some_type_t" << std::endl;
  awali::tester_t<int,some_type_t> tester2;
  tester2.run();

  std::cout << std::endl << "some_type_t -> std::string" << std::endl;
  awali::tester_t<some_type_t,std::string> tester3;
  tester3.run();
  
  std::cout << std::endl << "int* -> int*" << std::endl;
  awali::tester_t<int*,int*> tester4;
  tester4.run();
}





