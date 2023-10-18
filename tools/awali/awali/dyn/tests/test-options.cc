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

#include <cassert>
#include <vector>

#include <awali/dyn.hh>
#include <cassert>


using namespace awali::dyn;
using namespace awali;
using namespace awali::dyn::internal;

int main()
{
  using pair_t = option_value_pair_t;
  options_t default_options;
  options_t changed_options;

  {
    pair_t pair = (EXP_TO_AUT_ALGO = GLUSHKOV);
    changed_options += pair;
    assert(default_options[EXP_TO_AUT_ALGO]
           != changed_options[EXP_TO_AUT_ALGO]);
  }
  {
    pair_t pair = (LAYOUT = "VERTICAL");
    changed_options += pair;
    assert(default_options[LAYOUT] != changed_options[LAYOUT]);
    // Check that previous were not changed
    assert(default_options[EXP_TO_AUT_ALGO]
           != changed_options[EXP_TO_AUT_ALGO]);
  }
  {
    pair_t pair = (IN_PLACE = true);
    changed_options += pair;
    assert(default_options[IN_PLACE] != changed_options[IN_PLACE]);
    // Check that previous were not changed
    assert(default_options[LAYOUT] != changed_options[LAYOUT]);
    assert(default_options[EXP_TO_AUT_ALGO]
           != changed_options[EXP_TO_AUT_ALGO]);
  }
  {
    pair_t pair = (KEEP_HISTORY = "false");
    changed_options += pair;
    assert(default_options[KEEP_HISTORY] != changed_options[KEEP_HISTORY]);
    // Check that previous were not changed
    assert(default_options[IN_PLACE] != changed_options[IN_PLACE]);
    assert(default_options[LAYOUT] != changed_options[LAYOUT]);
    assert(default_options[EXP_TO_AUT_ALGO]
           != changed_options[EXP_TO_AUT_ALGO]);
  }
  {
    pair_t pair{"IO_FORMAT", "GRAIL"};
    changed_options += pair;
    assert(default_options[IO_FORMAT] != changed_options[IO_FORMAT]);
    // Check that previous were not changed
    assert(default_options[KEEP_HISTORY] != changed_options[KEEP_HISTORY]);
    assert(default_options[IN_PLACE] != changed_options[IN_PLACE]);
    assert(default_options[LAYOUT] != changed_options[LAYOUT]);
    assert(default_options[EXP_TO_AUT_ALGO]
           != changed_options[EXP_TO_AUT_ALGO]);
  }
  {
    pair_t pair{"minim-algo", "brzozowski"};
    changed_options += pair;
    assert(default_options[MINIM_ALGO] != changed_options[MINIM_ALGO]);
    // Check that previous were not changed
    assert(default_options[IO_FORMAT] != changed_options[IO_FORMAT]);
    assert(default_options[KEEP_HISTORY] != changed_options[KEEP_HISTORY]);
    assert(default_options[IN_PLACE] != changed_options[IN_PLACE]);
    assert(default_options[LAYOUT] != changed_options[LAYOUT]);
    assert(default_options[EXP_TO_AUT_ALGO]
           != changed_options[EXP_TO_AUT_ALGO]);
  }
  {
    std::cout << "==========" << std::endl
              << "Expected to fail 1/3" << std::endl;
    try {
      pair_t pair{"n'importe-quoi", "quoi"};
      assert(false);
    } catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
  {
    std::cout << "==========" << std::endl
              << "Expected to fail 2/3" << std::endl;
    try {
      pair_t pair{"EXP_TO_AUT_ALGO", "n'importe-quoi"};
      assert(false);
    } catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
  {
    std::cout << "==========" << std::endl
              << "Expected to fail 3/3" << std::endl;
    try {
      pair_t pair = (EXP_TO_AUT_ALGO = "n'importe-quoi");
      assert(false);
    } catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}