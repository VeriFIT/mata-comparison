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

#ifndef AWALI_MISC_ZIP_HH
# define AWALI_MISC_ZIP_HH

#include <awali/sttc/misc/raise.hh> // pass
#include <awali/common/tuple.hh>

namespace awali {
  namespace sttc {
    namespace internal {

      template <typename... Sequences>
      struct zip_sequences
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

        zip_sequences(const sequences_t& sequences)
          : sequences_(sequences)
        {}

        zip_sequences(Sequences... sequences)
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
        struct zip_iterator
        {
          /// Underlying iterators.
          using iterators_type = IteratorsType;

          zip_iterator(const iterators_type& is, const iterators_type& ends)
            : is_{is}
            , ends_{ends}
          {}

          template <typename OtherValue, typename OtherIterators>
          zip_iterator(zip_iterator<OtherValue, OtherIterators> const& that)
            : is_{that.is_}
            , ends_{that.ends_}
          {}

          /// The current position.
          iterators_type is_;
          /// The ends.
          iterators_type ends_;

          /// Advance to next position.
          zip_iterator& operator++()
          {
            if (!next_())
              done_();
            return *this;
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
          bool next_()
          {
            return next_(indices_t{});
          }

          template <std::size_t... I>
          bool next_(seq<I...>)
          {
            bool res = true;
            using swallow = int[];
            (void) swallow
              {
                res
                  && (++std::get<I>(is_) == std::get<I>(ends_)
                      ? res = false
                      : true)
                  ...
                  };
            return res;
          }

          template <typename OtherValue, typename OtherIterators>
          bool equal(const zip_iterator<OtherValue, OtherIterators>& that) const
          {
            return equal_(that, indices_t{});
          }

          template <typename OtherValue, typename OtherIterators,
                    std::size_t... I>
          bool equal_(const zip_iterator<OtherValue, OtherIterators>& that,
                      seq<I...>) const
          {
            for (auto n: {(std::get<I>(is_) == std::get<I>(that.is_))...})
              if (!n)
                return false;
            return true;
          }

          /// Tuple of values.
          template <std::size_t... I>
          value_type dereference_(seq<I...>) const
          {
            return value_type{(*std::get<I>(is_))...};
          }
        };

        /// Mutable iterator.
        using iterator = zip_iterator<value_type, iterators_t>;

        /// Const iterator.
        using const_iterator = zip_iterator<const value_type, const_iterators_t>;

        const_iterator cbegin() const
        {
          return cbegin_(indices_t{});
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
          return begin_(indices_t{});
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
      zip_sequences<Sequences...>
      zip(Sequences&&... seqs)
      {
        return {std::forward<Sequences>(seqs)...};
      }

      template <typename... Sequences>
      zip_sequences<Sequences...>
      zip_tuple(const std::tuple<Sequences...>& seqs)
      {
        return {seqs};
      }
    }
  }
}//end of ns awali::stc

#endif // !AWALI_MISC_ZIP_HH
