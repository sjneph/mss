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
#include <cmath>
#include <ctime>
#include <deque>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <utility>
#include <vector>

//=================================================================
// main(): generate a bunch of random integers in random order and
//         run AlgMSS() on them.
//
// Use the AlgMSS() with STL but without mss::Help<T,U,V>
//=================================================================
int main() {
  using namespace std;
  using namespace mss;

  // Declare input container type
  typedef vector<int> InputType;
  const int MYMAX = 1234;
  InputType myInput;

  // Populate it with random #'s
  unsigned int rnd = (unsigned)time(NULL);
  std::cerr << "Random seed: " << rnd << std::endl;
  srand(rnd);
  for ( int i = 0; i < 50; ++i )
    myInput.push_back(rand() % MYMAX);

  // Find the median, shuffle input container's contents
  InputType::iterator mid = myInput.begin();
  advance(mid, static_cast<int>(myInput.size()/2));
  nth_element(myInput.begin(), mid, myInput.end());
  int myThreshold = *mid;
  std::cerr << "Threshold: " << *mid << std::endl;
  random_shuffle(myInput.begin(), myInput.end());

  // Create output types
  typedef pair<InputType::iterator, InputType::iterator> PairType;
  typedef vector<PairType> OutputType;
  OutputType myOutput;

  // Call algorithm
  AlgMSS(myInput.begin(), myInput.end(), back_inserter(myOutput), myThreshold);

  // Sweep through results and send to standard output
  OutputType::iterator i = myOutput.begin();
  while ( i != myOutput.end() ) {
    PairType p = *i++;
    copy(p.first, p.second, ostream_iterator<int>(cout, "\t"));
    cout << endl;
  } // while

  return(0);
}


/*
  ------------
  Discussion:
  ------------
  o Bad Compilers:
    Some (very old) compilers may be unable to compile in AlgMSS() due to
     the templated Help<> class in MSS.hpp.  Help<> uses a template-template
     parameter.

     You have three real choices:
     - Get a reasonably modern C++ compiler.
     - Make the last template paramer of Help<> a non-template-template
       parameter (like parameter 2 - the input container type).  This will
       require more changes to Help<>'s definition.
     - Don't use Help<> and remove all of it from MSS.hpp.  You can still use
       the STL as above, or use built-in arrays like in other example files.

  o threshold argument to AlgMSS():
    This is often zero.  When non-zero, the value is subtracted from each
     input score prior to use by AlgMSS(), but the actual input scores
     themselves are always left unchanged.

    If you have all positive scores, for example, the threshold chosen might be
     the median of those scores.  You may want to look at the nth_element()
     function that accompanies the <algorithm> header file.  That (average)
     linear time algorithm is excellent, but does change the order of values.
     Perhaps pass a copy of your data to it?
*/
