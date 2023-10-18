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

#ifndef AWALI_JSON_UTILS_HH
#define AWALI_JSON_UTILS_HH

#include<iostream>
#include<vector>
#include<limits>
#include<iomanip>
#include<sstream>
#include<awali/common/parse_exception.hh>

#define __SUPPRESS_UNUSED_WARNING__(a) (void)a;


namespace awali {
  namespace internal {
    
    /** prints spaces
     * 
     *  prints \a n spaces in the stream \a o.
     *
     * @param o output stream
     * @param n number of spaces
     * return the output stream
     */
    static inline
    std::ostream& tabs(std::ostream& o, int n) {
      __SUPPRESS_UNUSED_WARNING__(tabs)
        for(int i=0; i<n; i++)
          o << "   ";
      return o;
    }
    
    /** formates a json file
     *
     * This function insert indentations and line breaks in order to pretty print
     * a json file.
     *
     * The rules are the following:
     * - the existing line breaks are preserved, without changing the indentation;
     * - there is always a line break before a left curly brace, the indentation is then incremented;
     * - if an object contains inner objects, the right curly brace is written on a new line with the same indentation as the left curly brace. In any case, the indentation is decremented;
     *
     * @param o the output stream
     * @param i the input stream sending a syntacticly correct json file
     */
    static inline
    std::ostream& js_formate(std::ostream& o, std::istream& i) {
      __SUPPRESS_UNUSED_WARNING__(js_formate)
        int depth=0;
      char c;
      bool inopen=false;
      bool instring=false;
      while(! i.eof()) {
        i.get(c);
        switch(c) {
        case '\t': break;
        case '{':
          if(!instring) {
            inopen=true;
            if(depth>0)
              o << std::endl;
            tabs(o,depth++);
          }
          o << c;
          break;
        case '}':
          if(!instring) {
            --depth;
            if(depth<0)
              return o;
            if(!inopen){
              o << std::endl;
              tabs(o,depth);
            }
            inopen=false;
          }
          o << c;
          break;
        case '\n' :
          o << std::endl << ' ';
          tabs(o,depth-1);
          break;
        case '"' :
          instring=!instring;
        default :
          o << c;
        }
      }
      return o;
    }

    /** checks the next character
     *  
     * @param i the input stream
     * @param e the expected character
     * @param oth other expected characters (previously tested with peek)
     * return true if the next non blank character in \a i is \a e
     */
    static inline
    void check(std::istream& i, char e, std::string oth="") {
      __SUPPRESS_UNUSED_WARNING__(check)
      char c;
      do {
        i>>c;
      }while(c==' ' || c== '\n' || c== '\t');
      //std::cerr << c << '&' << std::endl;
      if(c!=e){
        std::ostringstream os;
        os << "parser expects ";
        for(auto o : oth)
          os << o << " or ";
        os << e << " gets " << c;
        throw parse_exception(i, os.str());
      }
    }

    /** peeks the next character
     *  
     * @param i the input stream
     * return the value of the next non blank character in \a i
     */
    static inline
    char peek(std::istream& i) {
      __SUPPRESS_UNUSED_WARNING__(peek)
        char c;
      while(1) {
        c=i.peek();
        if(c!=' ' && c!= '\n' && c!= '\t')
          return c;
        i.get(c);
      }
    }


    static inline
    std::string parsestring(std::istream& i) {
      __SUPPRESS_UNUSED_WARNING__(parsestring)
        char c;
      i >> c;
      if(c!='"')
        throw parse_exception(i,"parser \"");
      std::ostringstream o;
      while(true) {
        i.get(c);
        if(c=='"')
          break;
        if(c=='\\') {
          i.get(c);
          switch(c) {
            case '\\':
            case '"':
              std::cerr << "found " << c;
              o << c;
              break;
            case 'b': o << '\b'; std::cerr << "found \\b"; break;
            case 'n': o << '\n'; break;
            case 'f': o << '\f'; break;
            case 'r': o << '\r'; break;
            case 't': o << '\t'; break;
            case 'u': {
              char c1,c2,c3,c4;
              i >> c1;
              i >> c2;
              i >> c3;
              i >> c4;
              if(c1=='0' && c2 =='0' && (c3 == '0' || c3 == '1')) {
                c = (c3 == '1')?16:0;
                if ('0'<= c4 && c4 <= '9')
                  c +=  (c4-'0');
                else if ('a'<= c4 && c4 <= 'f')
                  c += 10 + c4 -'a';
                else if ('A'<= c4 && c4 <= 'F')
                  c += 10 + c4 -'A';
                else
                  throw parse_exception(i, "Only unicode escaped character "
                    "below 1F are supported, i.e., of the form \\u00YZ, where "
                    "Y = 0 or 1.");
                std::cerr << "found " << std::hex << (int) c;
                o << c;
              }
              else
                throw parse_exception(i, "Only unicode escaped character below "
                  "1F are supported, i.e., of the form \\u00YZ, where Y = 0 or "
                  "1.");

            }
            break;
            default:
              o << '\\' << c;
          }
        }
        else
          o << c;
      }
      return o.str();
    }


    static inline
    char parsecst(std::istream& i) {
      __SUPPRESS_UNUSED_WARNING__(parsecst)
        std::string s;
      int l;
      switch(i.peek()) {
      case 'n' : case 't': l=4; break;
      case 'f' : l=5; break;
      default:
        throw parse_exception(i,"parser cst");
      }
      for(int n=0; n<l; ++n) {
        char c;
        i.get(c) ;
        s.append(1,c);
      }
      if(s!="true" && s!="none"  && s!="null" && s!= "false")
        throw parse_exception(i,"parser cst");
      return s[0];
    }

    static inline
    int parseint(std::istream& i) {
      __SUPPRESS_UNUSED_WARNING__(parseint)
        int x;
      i >> x;
      return x;
    }

    static inline
    std::string get_first_attr(std::istream& i) {
      __SUPPRESS_UNUSED_WARNING__(get_first_attr)
        check(i, '{');
      std::string attr = parsestring(i);
      check(i, ':');
      return attr;
    }
  
    static inline
    void parseignore(std::istream& i) {
      __SUPPRESS_UNUSED_WARNING__(parseignore)
        switch(peek(i)) {
        case '"':
          parsestring(i);
          if(peek(i)!=':')
            return;
          check(i,':');
          parseignore(i);
          return;
        case '{':
          check(i,'{');
          while(peek(i)!='}')
            parseignore(i);
          check(i,'}');
          return;
        case '[' :
          check(i,'[');
          while(1) {
            parseignore(i);
            if(peek(i)!=',')
              break;
            check(i,',');
          }
          check(i,']');
          return;
        case 't': case 'n' : case 'f':
          parsecst(i);
          return;
        default :
          throw parse_exception(i,"Parse ignore");
        }
    }    


    static std::vector<std::string> init_converter () {
      std::vector<std::string> res;
      for (int i = std::numeric_limits<char>::min(); 
           i <= (int) std::numeric_limits<char>::max();
           i++)
        res.push_back("");
      for (int i = std::numeric_limits<char>::min(); 
           i <= std::numeric_limits<char>::max();
           i++)
      {
        char c = (char) i;
        std::stringstream o;
        switch (c) {
          case '\b':  o << "\\b"; break;
          case '\f':  o << "\\f"; break;
          case '\n':  o << "\\n"; break;
          case '\r':  o << "\\r"; break;
          case '\t':  o << "\\t"; break;
          case '"':   o << "\\\""; break;
          case '\\':  o << "\\\\"; break;
          default:
            if ('\x00' <= c && c <= '\x1f') {
                o << "\\u"
                  << std::hex << std::setw(4) << std::setfill('0') << (int)c;
            }
            else
                o << c;
         }
         res[(unsigned char) c] = o.str();
      }
      return res;
    }

    static std::vector<std::string> const converter =  init_converter();
} // end of namespace awali::internal 

namespace json {

    inline std::string const& escape(char c) { 
      return internal::converter[(unsigned char) c];
    }
    

    inline std::string unescape(std::string const& str) { 
      std::stringstream ss(str);
      std::string res= internal::parsestring(ss);
      if (ss.eof())
        return res;
      throw parse_exception(ss, "Given string contains more than a json "
                                "string."); 
    }


    inline
    std::ostream& escape_and_print(std::ostream& o,
                                     std::string const & str)
   {
      __SUPPRESS_UNUSED_WARNING__(escape_and_print)
      o << '"';
      for (char c : str)
        o << escape(c);
      return (o << '"'); 
    }
    
    inline std::string escape (std::string const& str) {
      __SUPPRESS_UNUSED_WARNING__((std::string(*)(std::string const&))escape)
      std::stringstream ss;
      escape_and_print(ss,str);
      return ss.str();
    } 


}// end of namespace awali::json
}// end of namespace awali

#undef __SUPPRESS_UNUSED_WARNING__
#endif                 //AWALI_JSON_UTILS_HH

