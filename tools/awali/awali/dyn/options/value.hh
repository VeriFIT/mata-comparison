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

#ifndef AWAlI_DYN_OPTIONS_VALUE_HH
#define AWAlI_DYN_OPTIONS_VALUE_HH


namespace awali { namespace dyn { namespace internal {

  struct opt_untyped_value
  {
    opt_untyped_value() {}
    virtual opt_untyped_value* clone() const = 0;
    virtual ~opt_untyped_value() {}
  };

  template <typename T>
  struct opt_typed_value : opt_untyped_value
  {
    T value;

    opt_typed_value(const T& val) : value(val) {}

    opt_typed_value* clone() const override
    {
      return new opt_typed_value(value);
    }
  };


  struct opt_any_t
  {
  private:
    opt_untyped_value* value;

  public:
    template <typename T>
    opt_any_t(const T& val) : value(new opt_typed_value<T>(val))
    {}

    opt_any_t(char const* s) : value(new opt_typed_value<std::string>(s)) {}

    opt_any_t(opt_any_t const& a) : value(a.value->clone()) {}

    opt_any_t& operator=(opt_any_t const& t)
    {
      if (&t != this) {
        delete value;
        value = t.value->clone();
      }
      return (*this);
    }

    ~opt_any_t() { delete (value); }


    template <typename T>
    explicit operator T() const
    {
      const opt_typed_value<T>& test
          = dynamic_cast<const opt_typed_value<T>&>(*(this->value));
      return test.value;
    }
  };

  struct option_value_pair_t
  {
    size_t id;
    internal::opt_any_t value;
    option_value_pair_t(size_t i, internal::opt_any_t v) : id(i), value(v) {}
    option_value_pair_t(std::string const&, std::string const&);
  };

}}} // namespace awali::dyn::internal
#endif
