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

/*
 o utilizes constant types
   --> see PType typedef below
   --> see arr[] declaration
 o uses (const) doubles
*/

int main() {
  using namespace mss;

  // input array to AlgMSS()
  const double arr[] = { 1.1, -2.2, 3.3, -2.4, 4.5,
                        -1.6, -2.7, 1.8, -1.9, 3.0 };
  const int INSIZE = sizeof(arr)/sizeof(double);

  // create & initialize output array for AlgMSS()
  typedef std::pair<const double*, const double*> PType;
  const double* NONE = 0;
  const PType UNUSED = std::make_pair(NONE, NONE);
  PType output[INSIZE/2 + 1];
  const int PSZ = sizeof(output)/sizeof(PType);
  for ( int i = 0; i < PSZ; ++i )
    output[i] = UNUSED;

  // call AlgMSS()
  AlgMSS(arr, arr+INSIZE, output, 0.0);

  // send results to stdout
  for ( int i = 0; i < PSZ; ++i ) {
    if ( output[i] == UNUSED )
      break;

    for ( const double* j = output[i].first; j != output[i].second; )
      std::cout << *j++ << "\t";
    std::cout << std::endl;
  } // for

  return(0);
}
