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


#include <awali/dyn/core/context_description/weightsets/abstract_weightset.hh>
#include <awali/dyn/core/context_description/weightsets/bounded_weightset.hh>
#include <awali/dyn/core/context_description/weightsets/cyclic_weightset.hh>
#include <awali/dyn/core/context_description/weightsets/basic_weightset.hh>

namespace awali {
namespace dyn {
namespace context {

int abstract_weightset::counter=0;

std::vector<std::string> const&
abstract_weightset::possible_promotions()
const
{
  static std::vector<std::string> empty_vector;
  return empty_vector;
}


/* =============================================================================
   basic_weightset 
============================================================================= */
weightset_description
basic_weightset::fromstring(std::string const&k)
const
{
  weightset_description ws;
  if(k==public_name) {
    ws = make_weightset_description();
    ws->type_ = index;
  }
  return ws;
}


bool
basic_weightset::is_promotable_to(weightset_description ws)
const
{
  if(public_name=="B")
    return true;
  if(ws->type_<0)
    return false;
  std::string str=instances()[ws->type_]->static_public_name();
  for(const auto& s : promotion)
    if(s==str)
      return true;
  return false;
}


std::vector<std::string> const&
basic_weightset::possible_promotions()
const
{
  static std::vector<std::string> b_vector
    = all_weightset_public_static_names();
  static bool once = true;
  if (once) {
    once = false;
    size_t i;
    for (i = 0; i< b_vector.size(); i++) {
      if(!b_vector[i].compare("B"))
        break;
    }
    b_vector.erase(b_vector.begin()+i);
  }
  if (this->static_public_name() == "B")
    return b_vector;
  else
    return promotion;
}


/* =============================================================================
   cyclic_weightset 
============================================================================= */
weightset_description
cyclic_weightset::parse_weightset(json::object_t const* p)
  const
{
  weightset_description ws;
  if(p->at("semiring")->to_string() == "Cyclic") {
    ws = make_weightset_description();
    ws->type_ = index;
    ws->characteristic = p->at("characteristic")->to_int();
  }
  return ws;
}


std::string const&
cyclic_weightset::tostring(weightset_description ws, bool dynamic)
const
{
  static std::string res;
  std::ostringstream o;
  o << "zz" << ws->characteristic ;
  res=o.str();
  return res;
}


std::string cyclic_weightset::static_pub_name = "Z/<int>Z";


std::string const& 
cyclic_weightset::static_public_name() 
const 
{
  return static_pub_name;
}

std::string cyclic_weightset::static_pub_description 
  = "Cyclic semiring Z/<int>Z";

std::string const& cyclic_weightset::static_desc() const {
  return static_pub_description;
}


weightset_description cyclic_weightset::fromstring(std::string const&k) const {
  weightset_description ws;
  if(k[0] == 'Z' && k[1] == '/') {
    std::istringstream i(k.substr(2));
    int n;
    i >> n;
    char c;
    i >> c;
    if (c=='Z') {
      ws = make_weightset_description();
      ws->type_ = index;
      ws->characteristic= n;
    }
  }
  return ws;
}

bool
cyclic_weightset::is_promotable_to(weightset_description ws) const {
  return false;
}

//
//   weightset_description
//   cyclic_weightset::parse_weightset(std::istream& i)
//   const
//   {
//     weightset_description ws;
//     if(sttc::parsestring(i)=="Cyclic") {
//       ws = make_weightset_description();
//       ws->type_ = index;
//       std::string key = sttc::parsestring(i);
//       if (key == "Characteristic") {
//         sttc::check(i, ':');
//         ws->characteristic=std::stoi(sttc::parsestring(i));
//       } else
//         throw std::runtime_error("json: expected \"Characteristic\", got \""+key+"\"");
//     }
//     return ws;
//   }

/* =============================================================================
   bounded_weightset 
============================================================================= */

weightset_description
bounded_weightset::parse_weightset(json::object_t const* obj)
const
{
  weightset_description ws;
  if(obj->at("semiring")->to_string()=="Bounded") {
    ws = make_weightset_description();
    ws->type_ = index;
    ws->characteristic = obj->at("characteristic")->to_int();
  }
  return ws;
}

std::string const&
bounded_weightset::tostring(weightset_description ws, bool dynamic)
const
{
  static std::string res;
  std::ostringstream o;
  o << "nn" << ws->characteristic ;
  res=o.str();
  return res;
}


std::string bounded_weightset::static_pub_name = "N<int>";


std::string const&
bounded_weightset::static_public_name()
const
{
  return static_pub_name;
}

std::string bounded_weightset::static_pub_description =
  "Quotient of N by the congruence generated by <int> = <int>+1";


std::string const&
bounded_weightset::static_desc()
const
{
  return static_pub_description;
}


weightset_description
bounded_weightset::fromstring(std::string const&k)
const
{
  weightset_description ws;
  if(k[0] == 'N' && k[1] != '\0') {
    std::istringstream i(k.substr(1));
    int n;
    i >> n;
    if (i.eof()) {
      ws = make_weightset_description();
      ws->type_ = index;
      ws->characteristic= n;
    }
  }
  return ws;
}


bool
bounded_weightset::is_promotable_to(weightset_description ws) const {
  return false;
}


} // end of namespace awali::dyn::context
} // end of namespace awali::dyn
} // end of namespace awali



