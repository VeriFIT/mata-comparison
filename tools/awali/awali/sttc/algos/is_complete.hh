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

#ifndef AWALI_ALGOS_IS_COMPLETE_HH
# define AWALI_ALGOS_IS_COMPLETE_HH

# include <queue>
# include <set>

namespace awali { namespace sttc {

  /// Whether \a aut is complete.
  /// \pre \a aut is LTL
  template <typename Aut>
  bool is_complete(const Aut& aut)
  {
    static_assert(labelset_t_of<Aut>::is_free(),
                  "requires free labelset");

    if (aut->num_initials() == 0)
      return false;

    using label_set_t = std::set<typename labelset_t_of<Aut>::letter_t>;

    const auto& letters = aut->labelset()->genset();
    for (auto state : aut->states())
    {
      label_set_t missing_letters = {std::begin(letters), std::end(letters)};

      for (auto tr : aut->all_out(state))
        missing_letters.erase(aut->label_of(tr));

      if (!missing_letters.empty())
        return false;
    }

    return true;
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_IS_COMPLETE_HH
