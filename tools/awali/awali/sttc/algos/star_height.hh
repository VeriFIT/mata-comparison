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

#ifndef AWALI_ALGOS_STAR_HEIGHT_HH
# define AWALI_ALGOS_STAR_HEIGHT_HH

#include <awali/sttc/core/rat/visitor.hh>

namespace awali { namespace sttc {

  namespace internal
  {
    template <typename RatExpSet>
    class star_heighter
      : public RatExpSet::const_visitor
    {
    public:
      using ratexpset_t = RatExpSet;
      using super_type = typename ratexpset_t::const_visitor;
      using node_t = typename super_type::node_t;
      template <rat::type_t Type>
      using variadic_t = typename super_type::template variadic_t<Type>;

      /// Entry point: return the size of \a v.
      unsigned
      operator()(const node_t& v)
      {
        height_ = 0;
        v.accept(*this);
        return height_;
      }

      /// Entry point: return the size of \a v.
      unsigned
      operator()(const std::shared_ptr<const node_t>& v)
      {
        return operator()(*v);
      }

    private:

# define DEFINE(Type)                                           \
      using Type ## _t = typename super_type::Type ## _t;       \
      virtual void visit(const Type ## _t& v) override

      DEFINE(atom)         { (void) v; }
      DEFINE(complement)   { v.sub()->accept(*this); }
      DEFINE(conjunction)  { visit_variadic(v); }
      DEFINE(ldiv)         { visit_variadic(v); }
      DEFINE(lweight)      { v.sub()->accept(*this); }
      DEFINE(one)          { (void) v; }
      DEFINE(prod)         { visit_variadic(v); }
      DEFINE(rweight)      { v.sub()->accept(*this); }
      DEFINE(shuffle)      { visit_variadic(v); }
      DEFINE(star)         { ++height_; v.sub()->accept(*this); }
      DEFINE(maybe)        { v.sub()->accept(*this); }
      DEFINE(plus)         { ++height_; v.sub()->accept(*this); }
      DEFINE(sum)          { visit_variadic(v); }
      DEFINE(transposition){ v.sub()->accept(*this); }
      DEFINE(zero)         { (void) v; }

# undef DEFINE

      /// Traverse variadic node.
      template <rat::type_t Type>
      void
      visit_variadic(const variadic_t<Type>& n)
      {
        /* The height of an n-ary is the max of the n heights. */
        auto max = height_;
        auto initial = height_;
        for (auto child : n)
          {
            height_ = initial;
            child->accept(*this);
            if (max < height_)
              max = height_;
          }
        height_ = max;
      }

      unsigned height_;
    };
  } // namespace internal


  /// Star height of a ratexp.
  template <typename RatExpSet>
  inline
  unsigned
  star_height(const typename RatExpSet::ratexp_t& e)
  {
    internal::star_heighter<RatExpSet> s;
    return s(e);
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_STAR_HEIGHT_HH
