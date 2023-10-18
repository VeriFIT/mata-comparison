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

#include<awali/dyn/loading/parse_ctx.hh>


namespace awali { namespace  dyn {
  namespace loading {


    /* Transforms the string representing a weighset into the string of
    * the file (without ".hh") where it is defined.
    */
    std::string filename_of_ws(std::string w) {
      if (w[0] == 'z' && w[1] == 'z')
        return "zz";
      else
        if (w[0] == 'n' && w[1] == 'n')
          return "nn";
        else
          return w;
    }


    /* Transforms the string representing a weighset into the string of
    * its type.
    */
    std::string classname_of_ws(std::string w) {
        if (w[0] == 'z' && w[1] == 'z')
            return "zz<"+w.substr(2)+">";
        else
          if (w[0] == 'n' && w[1] == 'n')
            return "nn<"+w.substr(2)+">";
          else
            return w;
    }
    /*
     * C  -> L_W
     * L  -> lao | lal_char lal_int | law_char | lan<L> | lat<LL>
     * LL -> L | L,LL
     * W  -> b | z | q | r | c | f2 | zmin | zmax | pmax | zzN | nnK
     *     | ratexpset<C> | series<C> | product<WW>
     * WW -> W | W,WW
     *
     * where N is a positive number.
     */

    void rec_parse_labelset(const std::string& s, size_t& p,
                            std::set<std::string>& labelsets, std::string& ls) {
      if(s[p-1]=='>') {
        std::list<std::string> sublabelsets;
        do {
          rec_parse_labelset(s, --p, labelsets, ls);
          sublabelsets.emplace_front("sttc::ctx::"+ls);
        } while(s[p-1]==',');
        --p;
        size_t q=p;
        while(p!=0 && s[p-1]!='<' && s[p-1]!=',') --p;
        ls = s.substr(p, q-p)+"<";
        bool f=true;
        for(auto sls : sublabelsets) {
          if(f)
            f=false;
          else
            ls+=",";
          ls+=sls;
        }
        ls+=">";
        labelsets.emplace(s.substr(p, q-p));
      }
      else {
        size_t q=p;
        while(p!=0 && s[p-1]!='<' && s[p-1]!=',') --p;
        ls = s.substr(p, q-p);
        labelsets.emplace(ls);
      }
    }



    void rec_parse_context(const std::string& s, size_t& p,
                           std::set<std::string>& weightsets, std::set<std::string>& labelsets,
                           std::string& ws, std::string& ls) {
      //std::cerr << "S:"<<s<<std::endl;
      if(s[p-1]=='>') {
        rec_parse_context(s, --p, weightsets, labelsets, ws, ls);
        p--;
        std::string ctx="sttc::context<sttc::ctx::"+ls+','+"sttc::"+classname_of_ws(ws)+">";
        size_t q=p;
        while(s[p-1]!='_') --p;
        ws = s.substr(p, q-p)+'<'+ctx+'>';
        weightsets.emplace(s.substr(p, q-p));
      }
      else {
        size_t q=p;
        while(s[p-1]!='_') --p;
        ws = s.substr(p, q-p);
        weightsets.emplace(ws);
      }
      --p;
      rec_parse_labelset(s, p, labelsets, ls);
    }

  }
}}//end of ns awali::dyn

