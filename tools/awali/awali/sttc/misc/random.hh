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

#ifndef AWALI_MISC_RANDOM_HH
# define AWALI_MISC_RANDOM_HH

# include <random>
# include <iterator>

#include <awali/sttc/misc/attributes.hh>

namespace awali { namespace sttc {
    namespace internal {

  // Based on https://gist.github.com/cbsmith/5538174.
  template <typename RandomGenerator = std::default_random_engine>
  struct random_selector
  {
    random_selector(const RandomGenerator& g = RandomGenerator())
      : gen_(g)
    {}

    /// A randomly selected iterator in [start, end).
    template <typename Iter>
    Iter select(Iter start, Iter end)
    {
      std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
      std::advance(start, dis(gen_));
      return start;
    }

    /// A randomly selected iterator in [start, end).
    template <typename Iter>
    Iter operator()(Iter start, Iter end)
    {
      return select(start, end);
    }

    /// A randomly selected member of \a c.
    template <typename Container>
    auto operator()(const Container& c) -> decltype(*std::begin(c))
    {
      return *select(std::begin(c), std::end(c));
    }

    /// A randomly selected member of \a c.  Remove it from c.
    template <typename Container>
    auto pop(Container& c) -> typename Container::value_type
    {
      auto i = select(std::begin(c), std::end(c));
      auto res = *i;
      c.erase(i);
      return res;
    }

  private:
    RandomGenerator gen_;
  };


  template <typename RandomGenerator = std::default_random_engine>
  struct random_selector<RandomGenerator>
  make_random_selector(const RandomGenerator& g)
    ATTRIBUTE_PURE;

  template <typename RandomGenerator>
  struct random_selector<RandomGenerator>
  make_random_selector(const RandomGenerator& g)
  {
    return g;
  }

    }}}//end of ns awali::stc

#endif // !AWALI_MISC_RANDOM_HH
