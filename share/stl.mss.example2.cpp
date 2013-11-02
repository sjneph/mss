/*

FILE: stl.mss.example2.cpp
AUTHOR: Shane Neph
CREATE DATE: 2006


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
#include <fstream>
#include <iterator>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>


/*
  =============================================================================
   Define some user-defined type that is implicitly convertible to type double.

   Below, this user defined type would probably be used when reading in some
   external file that contains biological information.
      Each row of data might be of the form:

      chromosome start_coordinate end_coordinate strand measurement
  =============================================================================
*/

struct MyBiologyData {

 /* ... implement interesting member functions, constructors, etc. */

 operator double() const { /* an implicit conversion function */
   return(measurement_);
 }

 friend std::istream& operator>>(std::istream& is, MyBiologyData& mbd) {
   /* no error-handling performed: for illustrative purposes only */
   is >> mbd.chromosome_;
   is >> mbd.coordinates_.first;
   is >> mbd.coordinates_.second;
   is >> mbd.strand_;
   is >> mbd.measurement_;
   return(is);
 }


private:
  std::string chromosome_;
  std::pair<unsigned int, unsigned int> coordinates_;
  char strand_;
  double measurement_;
};



//========
// main()
//========
int main(int argc, char** argv) {

  using namespace mss;
  using namespace std;

  // Simple error check
  if ( argc != 2 ) {
    cerr << "Expect: " << argv[0] << " <input-file>" << endl;
    return(-1);
  }


  // Open input file
  ifstream inputFile(argv[1]); // your input file
  if ( !inputFile ) {
    cerr << "Unable to find: " << argv[1] << endl;
    return(-1);
  }

  // Typedef MyBiologyData and the typedef Helper class
  typedef MyBiologyData T;
  typedef Help<T, list<T>, list> HType;


  // Copy 'inputFile' into input container: 'bioStuff'
  //  Uses overloaded operator>> in MyBiologyData
  istream_iterator<T> inputIter(inputFile), eos;
  HType::InputContType bioStuff(inputIter, eos);


  // Make sure we have some actual input scores
  if ( bioStuff.empty() ) {
    cerr << "No data found in: " << argv[1] << endl;
    return(-1);
  }


  // Declare output container variable
  HType::DisjointRangeList algOutput;
  double threshold = 0;


  // Run AlgMSS()
  AlgMSS(bioStuff.begin(), bioStuff.end(),
         back_inserter(algOutput),
         threshold);


  /* ...do something interesting with your results
       o maybe combine chromosome coordinates across grouped results
       o maybe erase() positions in bioStuff that aren't part of a
          maximal scoring sequence (we used list types :)
       o etc.
  */

  // Send results to standard output
  HType::OutputIterator i = algOutput.begin();
  while ( i != algOutput.end() ) {
    HType::ForwardIterator fi = i->first;
    while ( fi != i->second ) {
      cout << *fi++ << "\t";    
    } // while
    cout << "(total score = "
         << accumulate(i->first, i->second, 0.0) 
         << ")"
         << endl;
    ++i;
  } // while

  return(0);
}


/*
  ===========
  Discussion
  ===========
  o Realize that implicit conversions are often undesirable "features" that
     some smart people would tell you not to use.  Clearly, you could copy the
     measurements from bioStuff over into a separate container of doubles and
     apply AlgMSS() on that container instead.

     Personally, I believe controlled and moderate use of implicit conversion
     functions can be a powerful (and pretty) technique.  You could also
     provide an overloaded assignment operator and/or copy constructor to type
     double.  A conversion constructor perhaps?

     You could provide conversion functions to type int, char, bool or whatever
     else you wanted as well.  Operator overloading is a very nice feature.
     Perhaps you could make MyBiologyData a templated class with a conversion
     operator to the templated type, T: "operator T()".  Then, create a type:
     MyBiologyData<double> or MyBiologyData<unsigned int>, etc.

     The tie-in here with AlgMSS() is that it is a fairly generic algorithm
     when combined with other C++ idioms.  As long as you use some sort of
     arithmetic type (or type that is convertible to an arithmetic type, like
     MyBiologyData), you're in good shape.  Even the threshold parameter is
     a template --> you can provide a user-defined type for that as long as you
     use overloading to make it behave like a built-in arithmetic type.

  o There are currently 5 built-in iterator "concepts" in C++.  The template
     parameters for AlgMSS() describe the minimum required concept for your
     code to compile.  Consult your favorite STL reference for information on
     Input, Output, Forward, Bi-directional and Random-Access Iterators.

     The first two arguments to AlgMSS() are iterators from your input
     container.  As the template type for these implies, a Forward Iterator
     concept is required at a minimum.  STL containers such as vector, list
     and deque provide iterators that all exceed this requirement.

     What you cannot do is tie an input stream that only supports an Input
     Iterator concept for direct use with AlgMSS().  This would not make
     sense since the effect of calling AlgMSS() is a collection of iterator
     ranges that mark off sequences from your input.  An Input Iterator is
     a single-pass concept: once you move passed an element, it is gone.

     An example using most of the code above that would *not* compile:
      std::istream_iterator<T> inputIter(inputFile), eos;
      ... other stuff as code above, except for the bioStuff variable
      AlgMSS(inputIter, eos, back_inserter(algOutput), threshold);

     Unfortunately, such an attempt is likely to lead to a nearly
     undecipherable compiler error message...a common side effect of using
     the STL.  There are tools|techniques to combat this somewhat, but none
     are 100% satisfactory to me and they would only add complexity.  So,
     the best thing to do is use an STL container class (or an array) to get
     iterators for use with AlgMSS()'s ForwardIterator template type.  All
     example code files do this properly.

     The OutputIterator template type of AlgMSS() only requires an output
     iterator concept, as the name implies (ie; something that you can
     dereference and write to).  Any STL container, array, or even a
     stream to an output file|terminal may be used.
     See builtin.mss.example1.cpp for an example of the latter.
*/
