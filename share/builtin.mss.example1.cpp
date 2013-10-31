//=========
// Author:  Shane Neph
// Date:    2006
// Project: all maximal scoring subsequences
//=========

/*
MSS

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.


If the use of the MSS software results in outcomes which will be published,
please specify the version of MSS software you used and cite the following
reference:

Ruzzo, W. L., and Tompa, M. 1999. A Linear Time Algorithm for Finding All
Maximal Scoring Subsequences. Seventh International Conference on Intelligent
Systems for Molecular Biology. 234-241.

*/


#include "../include/MSS.hpp"
#include <algorithm>
#include <iostream>
#include <utility>

namespace std {
  template <typename T>
  ostream& operator<<(ostream& os, const pair<T*, T*>& p) {
    copy(p.first, p.second, ostream_iterator<T>(os, "\t"));
    return(os);
  }
} // namespace std

int main() {
  using namespace mss;
  typedef std::pair<int*, int*> PType;

  int arr[] = { 1, -2, 3, -2, 4, -1, -2, 1, -1, 3 };
  const int sz = sizeof(arr)/sizeof(int);
  AlgMSS(arr, arr+sz, std::ostream_iterator<PType>(std::cout, "\n"), 0);
  return(0);
}
