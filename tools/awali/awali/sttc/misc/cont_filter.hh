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

#ifndef AWALI_MISC_CONT_FILTER_HH
# define AWALI_MISC_CONT_FILTER_HH

#include<iterator>
#include<functional>


namespace awali {
  namespace sttc {
    namespace internal {

      template <typename Container>
      struct it_indice_filter {
        using value_t = typename Container::value_type;
        using pred_t = std::function<bool(const value_t&)>;
        using value_type = unsigned;
        using reference = unsigned&;
        using difference_type = int;
        using pointer = unsigned*;
        using  iterator_category = std::forward_iterator_tag ;
        it_indice_filter(Container cont, unsigned current, unsigned end, pred_t pred) :
          current(current), length(end), pred(pred), cont(cont) {}

        it_indice_filter(unsigned end) :
          current(end), length(end) {}

        unsigned current;
        unsigned length;
        pred_t pred;
        Container cont;

        unsigned operator*() {
          return current;
        }

        it_indice_filter& operator++() {
          do {
            ++current;
          } while(current!=length && !pred(cont[current]));
          return *this;
        }

        bool operator!=(const it_indice_filter& it) const {
          return current != it.current;
        }
        bool operator==(const it_indice_filter& it) const {
          return current == it.current;
        }
      };


      template <typename Container>
      struct indice_filter {
        using value_t = typename Container::value_type;
        using pred_t = std::function<bool(const value_t&)>;
        using const_iterator = it_indice_filter<Container>;

        indice_filter(const Container& cont, pred_t pred) :
          cont(cont), pred(pred), shift(0) {}
        indice_filter(const Container& cont, pred_t pred, unsigned shift) :
          cont(cont), pred(pred), shift(shift) {}
        const Container& cont;
        pred_t pred;
        unsigned shift;

        const_iterator begin() const {
          unsigned b=shift;
          unsigned e= cont.size();
          while(b!=e && !pred(cont[b])) ++b;
          return const_iterator{cont, b, e, pred};
        }

        const_iterator end() const {
          return const_iterator{(unsigned)cont.size()};
        }
        bool empty() const {
          return begin()==end();
        }

        unsigned size() const {
          unsigned s=0;
          for( const_iterator b=begin(), e= end(); b!=e ; ++b) ++s;
          return s;
        }
      };


      template <typename Container>
      struct it_filter {
        using value_type = typename Container::value_type;
        using pred_t = std::function<bool(const value_type&)>;
        using reference = value_type&;
        using difference_type = int;
        using pointer = value_type*;
        using  iterator_category = std::forward_iterator_tag ;
        using it_t = typename Container::const_iterator;

        it_filter(it_t current, it_t end, pred_t pred) :
          current(current), end_(end), pred(pred) {}

        it_filter(it_t end) :
          current(end), end_(end) {}

        it_t current;
        it_t end_;
        pred_t pred;

        const value_type& operator*() const {
          return *current;
        }

        it_filter& operator++() {
          do {
            ++current;
          } while(current!=end_ && !pred(*current));
          return *this;
        }

        bool operator!=(const it_filter& it) const {
          return current != it.current;
        }
        bool operator==(const it_filter& it) const {
          return current == it.current;
        }
      };


      template <typename Container>
      struct cont_filter {
        using value_type = typename Container::value_type;
        using pred_t = std::function<bool(const value_type&)>;
        using const_iterator = it_filter<Container>;
        using it_t = typename Container::const_iterator;

        cont_filter(const Container& cont, pred_t pred) :
          cont(cont), pred(pred) {}
        const Container& cont;
        pred_t pred;

        const_iterator begin() const {
          it_t b=cont.begin(), e= cont.end();
          while(b!=e && !pred(*b)) ++b;
          return const_iterator{b, e, pred};
        }

        const_iterator end() const {
          return const_iterator{cont.end()};
        }

        bool empty() const {
          return begin()==end();
        }

        unsigned size() const {
          unsigned s=0;
          for( const_iterator b=begin(), e= end(); b!=e ; ++b) ++s;
          return s;
        }

      };

      template <typename Iterator>
      struct it_cont {
        using value_type = typename Iterator::value_type;

        it_cont(Iterator begin, Iterator end) :
          begin_(begin), end_(end) {}
        Iterator begin_;
        Iterator end_;


        Iterator begin() const {
          return begin_;
        }

        Iterator end() const {
          return end_;
        }

        bool empty() const {
          return begin_==end_;
        }

        unsigned size() const {
          unsigned s=0;
          for( Iterator b=begin_, e= end_; b!=e ; ++b) ++s;
          return s;
        }

      };
    }
  }
}//end of ns awali::stc

#endif // !AWALI_MISC_CONT_FILTER_HH
