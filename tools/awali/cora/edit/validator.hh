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

#ifndef CORA_EDIT_VALIDATOR_HH
#define CORA_EDIT_VALIDATOR_HH

#include <string>
#include <vector>

#include<awali/dyn/core/automaton.hh>
#include<cora/edit/sentence.hh>

namespace awali { 
namespace cora {
namespace editter {


/** Interface for validators of string used to test user inputs. */
class abstract_validator_t
{
public:
  /** Returns `true` if `str` is acceptable.
   * If returning `false` the function {@link error_message} is expected to */
  virtual bool validate(std::string const& str) = 0;

  /** Returns the error message explaining the last failed validation.
   * Behaviour is undefined if no validation ever failed.
   */
  virtual sentence_t const& error_message() const = 0;
};


/* Simplest possible instantiation of {@link abstract_validator_t} where the
 * validation function is independent of the edit instance.
 *
 * This class wraps a string-to-bool function and a prototype for error messages
 * if the evaluation fails.
 */
class simple_validator_t : public abstract_validator_t
{
private:
  /** Stores the message given by the user. This could contain one occurrence
   * of %s.
   */
  const sentence_t _proto_error_message;

  /** After a failed validation, contains a copy of 
   * {@link _proto_error_message} where %s is replaced the string that failed
   * validation.
   */
  sentence_t _error_message;

  /** The wrapped validation function. */
  std::function<bool(std::string const&)> _function;
public:

  /** Calls {@link _function} on `str`, and if it returns false, populates
   * {@link _error_message} with a copy of {@link _proto_error_message} where
   * the first occurrence of %s` is replaced by `str`.
   */
  virtual bool validate(std::string const& str) override;

  /** Returns the error message related to the last failed validation. */
  virtual sentence_t const& error_message() const override;

  /** Constructs a `simple_validator_t` that will call `is_valid` to test
   * validation and prepare a message based on `prototype` if it is not.
   */
  simple_validator_t( std::function<bool(std::string const&)> is_valid, 
                      sentence_t prototype);
};


/** Implementation of interface {@link abstract_validator_t} for classes that
 * requires extra data from the edit instance.
 * Template parameter `D` is the type of that data, and is usually {@link
 * dyn::automaton_t} or {@link dyn::transducer_t}.
 * It is also sometimes std::string (e.g., for testing equality to epsilon
 * representation)
 *
 * This classes derives from {@link simple_validator_t} because it uses the 
 * same scheme for handling error_messages.
 */
template<typename D>
class runtime_validator_t : public simple_validator_t
{
private:
  /** References to the extra data used to validate*/
  D const& _data;

  /** Wrapped `(D,string) -> bool` validation function. */
  std::function<bool(D const&, std::string const&)> _function;


public:
  /** Constructs a validator.
   *  @param validation function to wrap.
   *  @param data Reference to the runtime data used to validate strings.
   *  @param prototype Prototype for error messages when validation fails.
   */
  runtime_validator_t<D>(
      std::function<bool(D const&, std::string const&)> function, 
      D const& data,
      sentence_t prototype )
  : simple_validator_t(
        [this](std::string const & str) { return _function(_data,str);},
        prototype
    ),
    _data(data),
    _function(function)
  {}
};


/** Validator that calls several other validators in a row. */
class combined_validator_t 
: public abstract_validator_t
{
private:
  /** Wrapped validators */
  std::vector<abstract_validator_t*> _validators;

  /** After a failed validation, stores the indec of the incriminated validator
    */
  size_t _error_is_in = 0;
public:

  virtual bool validate(std::string const&) override;
  virtual sentence_t const& error_message() const override;
  combined_validator_t(std::initializer_list<abstract_validator_t*>);
};



} // end of namespace awali::cora::editter
} // end of namespace awali::cora
} // end of namespace awali



#endif
