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

#ifndef CALLBACK_OSTREAM_HH
#define CALLBACK_OSTREAM_HH


namespace awali { namespace py {

typedef void (*callback_function_t)(std::string);

class callback_ostream_t : public std::ostream 
{
  class callback_buffer_t : public std::streambuf {
  private:
    callback_function_t _callback_function;
    std::string _message;
  public: 
    callback_buffer_t(callback_function_t f) : _callback_function(f) {}
  protected: 
    int overflow(int ch) override {
//       std::cout << "overflow: " << (char) ch << std::endl;
      if (ch ==  '\n') {
//         std::cout << "what the fuck" << std::endl;
        _callback_function(_message);
        _message="";
      }
      else
        _message += ch;
      return ch;
    }
    std::streamsize xsputn( const char_type* s, std::streamsize count ) {
//       std::cout << count << std::endl;
     return  std::streambuf::xsputn(s,count);
    }
  };
public:
  callback_ostream_t(callback_function_t f)
  : std::ostream(new callback_buffer_t(f)) {}

  ~callback_ostream_t() { delete rdbuf(); }
};



void setup_callback_stream(callback_function_t warning, 
                           callback_function_t error) 
{
  dyn::set_warning_stream(*(new callback_ostream_t(warning)));
  dyn::set_error_stream(*(new callback_ostream_t(error)));
}


}}

#endif
