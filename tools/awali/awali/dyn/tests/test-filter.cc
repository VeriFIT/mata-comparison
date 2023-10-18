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

#include<vector>
#include<iostream>
#include <awali/dyn.hh>
#include <awali/sttc/misc/cont_filter.hh>

int main() {
        std::vector<double> v;
        for(double i=1;i<=10;i++)
                v.emplace_back(5/i);
        //for( auto it=cv.begin(); it!=cv.end(); ++it)
        //      std::cout << *it << std::endl;

        for(auto i : awali::sttc::internal::cont_filter<std::vector<double>>(v, [](const double& d) -> bool {return (d<2);}))
                std::cout << i << std::endl;
}
