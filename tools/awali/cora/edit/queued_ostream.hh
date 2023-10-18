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

#ifndef CORA_EDIT_QUEUEDOSTREAM_HH
#define CORA_EDIT_QUEUEDOSTREAM_HH

#include<chrono> 
#include<iostream>
#include<queue>
#include<sstream>
#include<thread>

#include<cora/edit/notifiable.hh>
#include<cora/edit/sentence.hh>
#include<cora/edit/token.hh>

namespace awali {
namespace cora {
namespace editter {



  class queued_ostream_t : public std::ostream 
  { 
  private:
    notifiable_t& client;
    std::queue<sentence_t> message_queue;
    bool front_called_since_last_pop = false;
    std::chrono::high_resolution_clock::time_point front_call_time;
    std::chrono::milliseconds message_timeout{1500};
    
    class custombuffer_t : public std::streambuf {
      std::stringstream ss;
      queued_ostream_t& holding_ostream;
      public: custombuffer_t(queued_ostream_t& o) : holding_ostream(o) {}
      protected: int overflow(int ch) override;
    };

  public:
    
    enum class action_t {
      NOTHING=0,
      ERASE=1,
      PRINT=2,
      REPLACE=3
    };

    queued_ostream_t (notifiable_t& c) 
    : std::ostream(new custombuffer_t(*this)), client(c) 
    {}

    ~queued_ostream_t();


    sentence_t const& front();
    void pop(); 
    void push(const sentence_t& tokens);
    size_t size() const;
    action_t expected_action() const;
    
  };

  queued_ostream_t& operator<< (queued_ostream_t& o, const sentence_t& tokens);

} // end of namespace awali::cora::editter
} // end of namespace awali::cora
} // end of namespace awali
#endif
