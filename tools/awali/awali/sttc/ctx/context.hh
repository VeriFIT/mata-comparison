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

#ifndef AWALI_CTX_CONTEXT_HH
# define AWALI_CTX_CONTEXT_HH

# include <cassert>
# include <memory>
# include <string>

#include <awali/common/json/node.cc>
#include <awali/common/version.hh>
#include <awali/sttc/core/kind.hh>
#include <awali/sttc/core/rat/fwd.hh>
#include <awali/sttc/ctx/fwd.hh>
#include <awali/sttc/misc/stream.hh>

namespace awali { namespace sttc {

    /** @brief carries the algebraic settings of automata
     *
     * @tparam LabelSet the type that manages labels
     * @tparam WeightSet the type that manages weights
     */
  template <typename LabelSet, typename WeightSet>
  class context
  {
  public:
    using labelset_t = LabelSet;
    using weightset_t = WeightSet;
    using labelset_ptr = std::shared_ptr<const labelset_t>;
    using weightset_ptr = std::shared_ptr<const weightset_t>;

    using kind_t = typename labelset_t::kind_t;
    enum
      {
        is_lal = std::is_same<kind_t, labels_are_letters>::value,
        is_lan = std::is_same<kind_t, labels_are_nullable>::value,
        is_lao = std::is_same<kind_t, labels_are_one>::value,
        is_lar = std::is_same<kind_t, labels_are_ratexps>::value,
        is_lat = std::is_same<kind_t, labels_are_tuples>::value,
        is_law = std::is_same<kind_t, labels_are_words>::value
      };
    /// Type of transition labels, and type of RatExp atoms.
    using label_t = typename labelset_t::value_t;
    /// Type of weights.
    using weight_t = typename weightset_t::value_t;
    /// Type of RatExp ratexps objects.
    using node_t = rat::node<label_t, weight_t>;
    using ratexp_t = std::shared_ptr<const node_t>;
    /// Type of RatExp visitor.
    using const_visitor = sttc::rat::const_visitor<label_t, weight_t>;

    context(const context& that)
      : context(that.ls_, that.ws_)
    {}

    /// \param ls  the labelset
    /// \param ws  the weightset
    context(const labelset_ptr& ls, const weightset_ptr& ws)
      : ls_{ls}
      , ws_{ws}
    {}

    /// Build a context.
    /// \param ls  the labelset
    /// \param ws  the weightset
    context(const labelset_t& ls, const weightset_t& ws = {})
      : context(std::make_shared<const labelset_t>(ls),
                std::make_shared<const weightset_t>(ws))
    {}

    /// Build a context.
    /// \param gs  the generators
    /// \param ws  the weightset
    //
    // Use SFINAE to avoid requiring labelset_t to define letter_t.
    // labels_are_tuples does not define it for instance.
    //
    // It would be simpler to just use "= {}", but the C++ standard
    // does not support it (and this is properly considered a
    // defect: see http://cplusplus.github.io/LWG/lwg-active.html#2193).
    //
    // Gcc accepts though.
    // http://gcc.gnu.org/bugzilla/show_bug.cgi?id=56922
    // But clang rejects it.
    // http://llvm.org/bugs/show_bug.cgi?id=15724
    template <typename LabelSet2 = labelset_t>
    context(const std::initializer_list<typename LabelSet2::letter_t>& gs,
            const weightset_t& ws = {})
      : context{labelset_t{gs}, ws}
    {}

    /// Build a context whose labelset constructor takes no argument.
    template <typename LabelSet2 = labelset_t>
    context()
      : context{typename std::enable_if<is_lao, labelset_t>::type{},
                weightset_t{}}
    {}

  
    /* The fact that this operator is default is deprecated. */
    context& operator=(const context& other) = default;
    /*
    context& operator=(context&& that)
    {
      if (this != &that)
        {
          std::swap(ls_, that.ls_);
          std::swap(ws_, that.ws_);
        }
      return *this;
    }
    */
    
    /// The name of this context, built from its parameters.
    /// E.g., "lal_char_b", "law_char_zmin".
    static std::string sname()
    {
      return (labelset_t::sname()
              + "_" + weightset_t::sname());
    }

    std::string vname(bool full = true) const
    {
      return (labelset()->vname(full)
              + "_" + weightset()->vname(full));
    }

    /// Build from the description in \a is.
    static context make(std::istream& is)
    {
      auto ls = labelset_t::make(is);
      eat(is, '_');
      auto ws = weightset_t::make(is);
      return {ls, ws};
    }

    const labelset_ptr& labelset() const
    {
      return ls_;
    }

    const weightset_ptr& weightset() const
    {
      return ws_;
    }

    std::ostream&
    print_set(std::ostream& o, const std::string& format = "text") const
    {
      labelset()->print_set(o, format);
      if (format == "latex")
        o << "\\rightarrow";
      else
        o << '_';
      return weightset()->print_set(o, format);
    }

    
  
    template<unsigned version = version::fsm_json>
    json::node_t* 
    to_json() const {
      version::check_fsmjson<version>();
      json::object_t* obj = new json::object_t();
      obj->push_back("labels",  ls_->template to_json<version>());
      obj->push_back("weights", ws_->template to_json<version>());
      return obj;
    }

    static constexpr bool
    has_one()
    {
      return labelset_t::has_one();
    }

  private:
    labelset_ptr ls_;
    weightset_ptr ws_;
  };

  /*-----------------.
  | join_t, meet_t.  |
  `-----------------*/
  /** Computation of the join of some value sets.
   * 
   * The join of two value sets is the most restrictive value set
   * which is an extension of both ones.
   * 
   * For instance, the join of the weightset z and the weightset
   * ratexpset<context_t<lal_char,b>> is the weightset ratexpset<context_t<lal_char,z>>
   *
   * @tparam ValueSets a list of value sets
   */
  template <typename... ValueSets>
  using join_t = decltype(join(std::declval<ValueSets>()...));

  /** Computation of the meet of some value sets.
   * 
   * The meet of two value sets is the lees restrictive value set
   * whose both ones are extensions.
   * 
   * @tparam ValueSets a list of value sets
   */
  template <typename... ValueSets>
  using meet_t = decltype(meet(std::declval<ValueSets>()...));


  /*-------------------------.
  | Variadic join and meet.  |
  `-------------------------*/

  /// The join of a single valueset.
  /// Useful for variadic operator on a single argument.
  template <typename ValueSet>
  auto
  join(const ValueSet& vs)
    -> ValueSet
  {
    return vs;
  }

  template <typename ValueSet1, typename ValueSet2,
            typename ValueSet3, typename... VSs>
  auto
  join(const ValueSet1& vs1, const ValueSet2& vs2, const ValueSet3& vs3,
       const VSs&... vs)
    -> decltype(join(join(vs1, vs2), vs3, vs...))
  {
    return join(join(vs1, vs2), vs3, vs...);
  }

  /// The meet of a single valueset.
  /// Useful for variadic operator on a single argument.
  template <typename ValueSet>
  auto
  meet(const ValueSet& vs)
    -> ValueSet
  {
    return vs;
  }

  template <typename ValueSet1, typename ValueSet2, typename ValueSet3,
            typename... VSs>
  auto
  meet(const ValueSet1& vs1, const ValueSet2& vs2, const ValueSet3& vs3,
       const VSs&... vs)
    -> decltype(meet(meet(vs1, vs2), vs3, vs...))
  {
    return meet(meet(vs1, vs2), vs3, vs...);
  }


  /*-------------------------.
  | join(context, context).  |
  `-------------------------*/

  /// The join of two contexts.
  template <typename LhsLabelSet, typename LhsWeightSet,
            typename RhsLabelSet, typename RhsWeightSet>
  auto
  join(const context<LhsLabelSet, LhsWeightSet>& a,
       const context<RhsLabelSet, RhsWeightSet>& b)
    -> context<join_t<LhsLabelSet, RhsLabelSet>,
               join_t<LhsWeightSet, RhsWeightSet>>
  {
    auto ls = join(*a.labelset(), *b.labelset());
    auto ws = join(*a.weightset(), *b.weightset());
    return {ls, ws};
  }


  /*-------------------------.
  | meet(context, context).  |
  `-------------------------*/

  /// The meet of two contexts.
  template <typename LhsLabelSet, typename LhsWeightSet,
            typename RhsLabelSet, typename RhsWeightSet>
  auto
  meet(const context<LhsLabelSet, LhsWeightSet>& a,
       const context<RhsLabelSet, RhsWeightSet>& b)
    -> context<meet_t<LhsLabelSet, RhsLabelSet>,
               join_t<LhsWeightSet, RhsWeightSet>>
  {
    auto ls = meet(*a.labelset(), *b.labelset());
    auto ws = join(*a.weightset(), *b.weightset());
    return {ls, ws};
  }

    
}}//end of ns awali::stc

#endif // !AWALI_CTX_CONTEXT_HH
