
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

#ifndef CORA_EDIT_QUEUEDOSTREAM_CC
#define CORA_EDIT_QUEUEDOSTREAM_CC

#include<cora/edit/queued_ostream.hh>


namespace awali {
namespace cora {
namespace editter {

int
queued_ostream_t::custombuffer_t::overflow(int ch)
{
  if (ch ==  '\n') {
    holding_ostream.push(sentence_t(ss.str(),STANDOUT));
    ss.str("");
    refresh();
  }
  else
    ss << (char) ch;
  return ch;
}

queued_ostream_t::~queued_ostream_t() 
{
  delete(rdbuf());
}

queued_ostream_t& operator<< (queued_ostream_t& o, const sentence_t& tokens) 
{
  o.push(tokens);
  return o;
}


sentence_t const& 
queued_ostream_t::front() 
{ 
  if (!front_called_since_last_pop) {
    front_call_time = std::chrono::high_resolution_clock::now();
    front_called_since_last_pop = true;
  }
  return message_queue.front(); 
}


void 
queued_ostream_t::pop () 
{ 
  front_called_since_last_pop = false;
  return message_queue.pop(); 
}


void 
queued_ostream_t::push (const sentence_t& tokens) 
{ 
  message_queue.push(tokens); 
  client.notify(this);
}

size_t 
queued_ostream_t::size() 
const  
{ 
  return message_queue.size(); 
}


queued_ostream_t::action_t 
queued_ostream_t::expected_action() 
const 
{
  if (this->size() == 0)
    return action_t::NOTHING;
  std::chrono::milliseconds ellapsed 
    = std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::high_resolution_clock::now()
          - front_call_time
      );

  if (front_called_since_last_pop) {
    unsigned short res = 0;
    if (ellapsed > message_timeout) 
      res |= (unsigned short) action_t::ERASE;
    if (size() >= 2)
      res |= (unsigned short) action_t::PRINT;
    return (action_t)res;
  } 
  else
    return action_t::PRINT;
  


}

} // end of namespace awali::cora::editter
} // end of namespace awali::cora
} // end of namespace awali

#endif
