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

#ifndef CORA_EDIT_NOTIFIABLE_HH
#define CORA_EDIT_NOTIFIABLE_HH


namespace awali {
namespace cora {
namespace editter {



  struct notifiable_t
  { 
    public: 
      virtual void notify(void* notifier) = 0;
      virtual ~notifiable_t() = default;
  };


} // end of namespace awali::cora::editter
} // end of namespace awali::cora
} // end of namespace awali
#endif
