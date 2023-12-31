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

#ifndef AWALI_MISC_CROSS_HH
# define AWALI_MISC_CROSS_HH

#include <awali/sttc/misc/raise.hh> // pass
#include <awali/common/tuple.hh>
# include <iterator>

namespace awali {
  namespace sttc {
    namespace internal {
      template <typename... Sequences>
      struct cross_sequences
      {
        /// Type of the tuple of all the maps.
        using sequences_t = std::tuple<Sequences...>;

        /// Type of index sequences.
        template <std::size_t... I>
        using seq = awali::internal::index_sequence<I...>;

        /// Number of sequences.
        static constexpr size_t size = sizeof...(Sequences);

        /// Index sequence for our sequences.
        using indices_t = awali::internal::make_index_sequence<sizeof...(Sequences)>;

        /// The type of the underlying sequences, without reference.
        template <typename Seq>
        using seq_t = typename std::remove_reference<Seq>::type;

        /// The type of the members.
        using value_type
        = std::tuple<typename seq_t<Sequences>::value_type...>;

        cross_sequences(const sequences_t& sequences)
          : sequences_(sequences)
        {}

        cross_sequences(Sequences... sequences)
          : sequences_(sequences...)
        {}

        /// Tuple of const_iterators.
        using const_iterators_t
        = std::tuple<typename seq_t<Sequences>::const_iterator...>;

        /// Tuple of iterators.
        using iterators_t
        = std::tuple<typename seq_t<Sequences>::iterator...>;

        /// Composite iterator.
        template <typename ValueType,
                  typename IteratorsType>
        struct cross_iterator
        {
          /// Underlying iterators.
          using iterators_type = IteratorsType;

          cross_iterator(const iterators_type& is, const iterators_type& ends)
            : is_{is}
            , begins_{is}
            , ends_{ends}
          {}

          template <typename OtherValue, typename OtherIterators>
          cross_iterator(cross_iterator<OtherValue, OtherIterators> const& that)
            : is_{that.is_}
            , begins_{that.begins_}
            , ends_{that.ends_}
          {}

          /// The current position.
          iterators_type is_;
          /// The begins.
          iterators_type begins_;
          /// The ends.
          iterators_type ends_;

          /// Advance to next position.
          cross_iterator& operator++()
          {
            if (next_() == -1)
              done_();
            return *this;
          }

          bool operator!=(const cross_iterator& that) const
          {
            return not_equal_(that, indices_t{});
          }

          value_type operator*() const
          {
            return dereference_(indices_t{});
          }

        private:

          /// We have reached the end, move all the cursors to this end.
          void done_()
          {
            is_ = ends_;
          }

          /// Move to the next position.  Return the index of the lastest
          /// iterator that could move, -1 if we reached the end.
          int next_()
          {
            auto res = next_(indices_t{});
            // Reset all the iterators that are before the first one that could
            // advance.
            if (res != -1)
              reset_up_to_(res);
            return res;
          }

          template <std::size_t... I>
          int next_(seq<I...>)
          {
            int res = -1;
            using swallow = int[];
            (void) swallow
              {
                (res == -1
                 && std::get<size-1-I>(is_) != std::get<size-1-I>(ends_)
                 && std::next(std::get<size-1-I>(is_)) != std::get<size-1-I>(ends_))
                  ? ++std::get<size-1-I>(is_), res = size-1-I
                  : 0
                  ...
                  };
            return res;
          }

          /// Move beginning of ranges to their end, and align.
          void reset_up_to_(int n)
          {
            reset_up_to_(n, indices_t{});
          }

          template <std::size_t... I>
          void reset_up_to_(size_t n, seq<I...>)
          {
            using swallow = int[];
            (void) swallow
              {
                (n < I
                 && ((std::get<I>(is_) = std::get<I>(begins_)), true))...
                  };
          }

          template <std::size_t... I>
          bool not_equal_(const cross_iterator& that, seq<I...>) const
          {
            for (auto n: {(std::get<I>(is_) != std::get<I>(that.is_))...})
              if (n)
                return true;
            return false;
          }

          /// Tuple of values.
          template <std::size_t... I>
          value_type dereference_(seq<I...>) const
          {
            return value_type{(*std::get<I>(is_))...};
          }
        };

        /// Mutable iterator.
        using iterator = cross_iterator<value_type, iterators_t>;

        /// Const iterator.
        using const_iterator = cross_iterator<const value_type, const_iterators_t>;

        const_iterator cbegin() const
        {
          auto res = cbegin_(indices_t{});
          return res;
        }

        const_iterator cend() const
        {
          return cend_(indices_t{});
        }

        const_iterator begin() const
        {
          return cbegin();
        }

        const_iterator end() const
        {
          return cend();
        }

        iterator begin()
        {
          auto res = begin_(indices_t{});
          return res;
        }

        iterator end()
        {
          return end_(indices_t{});
        }

      private:
        template <std::size_t... I>
        const_iterator cbegin_(seq<I...>) const
        {
          return {const_iterators_t{std::get<I>(sequences_).cbegin()...},
              const_iterators_t{std::get<I>(sequences_).cend()...}};
        }

        template <std::size_t... I>
        const_iterator cend_(seq<I...>) const
        {
          return {const_iterators_t{std::get<I>(sequences_).cend()...},
              const_iterators_t{std::get<I>(sequences_).cend()...}};
        }

        template <std::size_t... I>
        iterator begin_(seq<I...>)
        {
          return {iterators_t{std::get<I>(sequences_).begin()...},
              iterators_t{std::get<I>(sequences_).end()...}};
        }

        template <std::size_t... I>
        iterator end_(seq<I...>)
        {
          return {iterators_t{std::get<I>(sequences_).end()...},
              iterators_t{std::get<I>(sequences_).end()...}};
        }

        /// The sequences we iterate upon.
        sequences_t sequences_;
      };

      template <typename... Sequences>
      cross_sequences<Sequences...>
      cross(Sequences&&... seqs)
      {
        return {std::forward<Sequences>(seqs)...};
      }

      template <typename... Sequences>
      cross_sequences<Sequences...>
      cross_tuple(const std::tuple<Sequences...>& seqs)
      {
        return {seqs};
      }
    }
  }
}//end of ns awali::stc

#endif // !AWALI_MISC_CROSS_HH
