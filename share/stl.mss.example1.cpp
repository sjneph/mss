/*

FILE: stl.mss.example1.cpp
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
#include <deque>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <utility>
#include <vector>


/*
  Below is an example usage of the STL with AlgMSS().  A fairly detailed
    explanation is near the end of this file, following main().
*/


//========================================================================
// main(): Pass in 1 argument: a file name.
//         The file should be valid and should be full of + an - numbers.
//========================================================================

int main(int argc, char** argv) {

  // Simple error check
  if ( argc != 2 ) {
    std::cerr << "Expect: " << argv[0] << " <input-file>" << std::endl;
    return(-1);
  }


  // Choose any one typedef below for our TDHelper type.
  typedef double T;
  typedef mss::Help<T> TDHelper; // Uses default container types
  // typedef mss::Help<T, std::vector<T>, std::vector> TDHelper;
  // typedef mss::Help<T, std::list<T>, std::list> TDHelper;
  // typedef mss::Help<T, std::list<T>, std::vector> TDHelper;
  // typedef mss::Help< T, const std::deque<T> > TDHelper;
  //   --> note the use of const here for the 2nd template argument

  //  ... or any other combination of plain STL-like container types



  // Open input file
  std::ifstream inputFile(argv[1]); // your input file
  if ( !inputFile ) {
    std::cerr << "Unable to find: " << argv[1] << std::endl;
    return(-1);
  }


  // Copy 'inputFile' into input container: 'inputScores'
  std::istream_iterator<T> inputIter(inputFile), eos;
  TDHelper::InputContType inputScores(inputIter, eos);


  // Make sure we have some actual input scores
  if ( inputScores.empty() ) {
    std::cerr << "No data found in: " << argv[1] << std::endl;
    return(-1);
  }


  // Declare output container variable
  TDHelper::DisjointRangeList algOutput;
  T threshold = 0;


  // Run AlgMSS()
  mss::AlgMSS(inputScores.begin(),
              inputScores.end(),
              std::back_inserter(algOutput),
              threshold);


  // Send results to standard output
  TDHelper::OutputIterator i = algOutput.begin();
  while ( i != algOutput.end() ) {
    TDHelper::ForwardIterator fi = i->first, fj = i->second;
    while ( fi != fj )
      std::cout << *fi++ << "\t";
    std::cout << std::endl;
    ++i;
  } // while


  // Use algOutput as you wish...


  return(0);
}



/*
  ----------------
  Brief Overview:
  ----------------
  While AlgMSS() may be used with built in array types, it's also easy to use
   with STL container types.  A templated class located within the mss
   namespace, Help<>, is meant to help you declare/use input/output containers
   for use with AlgMSS().

  NOTE: It is not required that the templated Help<> class be used in order
        for AlgMSS() to be used with STL container types (nor built in arrays,
        nor user defined container types) - the optional class is provided only
        for convenience.


  ---------------------------
  Defaults and Alternatives:
  ---------------------------
  By default, mss::Help<T> defines:
   o a vector for the input container type
   o a list for the output container type

  Simply override the default container types based upon your needs.

  typedef mss::Help<double> MyHelper; // uses default container types
     OR
  typedef mss::Help<double, const std::list<double>, std::list> MyHelper;
     OR
  typedef mss::Help<double, std::deque<double>, std::vector> MyHelper;
     OR
  typedef mss::Help<double, std::list<double>, std::deque> MyHelper;
     OR, define the i/p container and use the default o/p container...
  typedef mss::Help< double, std::list<double> > MyHelper;
     OR, some other combination...


   NOTE: The second typedef example uses a *const* std::list<double> -->
         you are welcome to use a constant input container type.  The
         output container will then hold pairs of const_iterators instead
         of the usual pairs of iterators.  See below for more details.

   NOTE: For mss::Help<T,U,V>, the second template parameter (U) must be
         a container type.  It must also *be* a type: that is,
         std::list<double> names a type while std::list does not.

         The 3rd template parameter, V, is a template-template parameter.
         Therefore, you do not fully specify it: that is, std::list can
         be used but std::list<anything> cannot.

         Originally, both U and V were template-template parameters, but
         this appears to preclude the possibility for a const container
         type for U (note that V cannot be const since we write to it in
         AlgMSS()).  As it now stands, U may be a const container type or
         a non-const one.


  ------------------------------------------
  Declaring and using variables with Help<>
  ------------------------------------------
  With a proper typedef set up for MyHelper (a better typedef name is
    recommended), an input container variable can be declared as follows:

   MyHelper::InputContType myInputContainer;


  You may also declare an output container variable as follows:

   MyHelper::DisjointRangeList myOutputContainer;

    o DisjointRangeList is named as such due to the fact that the output
       container is a collection of iterator pairs that each marks off a range
       of data from the input container, following the call to AlgMSS().
       Each range [a,b) is disjoint from all others identified.  All
       identified ranges are ordered, and each marks off a maximal
       scoring subsequence.

    o DisjointRangeList elements have type std::pair<FI, FI>, where FI
       is type MyHelper::ForwardIterator - an iterator that will point
       into your input container.  If your InputContType is const, then
       ForwardIterator will actually be a const_iterator type.

    o Define an iterator pointing to elements of a DisjointRangeList as in:
      
       MyHelper::OutputIterator myOI = myOutputContainer.begin();

       myOI->first through myOI->second mark off a subsequence [a,b) that is
        maximal in the input container's data (myOI->first and myOI->second are
        each of type MyHelper::ForwardIterator).


  ----------------
  Some Basic Care
  ----------------
  Realize that STL containers have properties that deserve attention if you'll
  be doing more interesting things than running AlgMSS() and sending results
  to output.  For example, if you are using a std::vector<double> as your
  input container type, then inserting an element into or erasing an element
  from that container *after* the call to AlgMSS() may invalidate important
  iterators - since your output container holds pairs of such iterators, your
  output container's contents may be invalidated:
    o Using a const std::vector<double> could preclude any later changes.
    o Using a std::list<double> could be fine with a small amount of care.
    o Using a std::vector<double> with reserve() can preclude some problems.
    o There are even more possible techniques.

  This isn't meant to scare you away from the STL!  The STL is great!  Since
  a std::vector is a "dynamic array", elements in it are contiguous.  What
  happens to existing iterators if you erase an existing element or insert
  a new one into a contiguous container?  What if you simply add a new
  element to the end, but memory must be re-allocated to do so?  Some
  basic understanding of containers is needed to do anything sophosticated
  with the STL (or programming in general).

  If you will be modifying the input container after a call to AlgMSS(), then
  you just need to understand the effects, if any, imposed on the pairs of
  iterators, held by your output container, that mark off ranges in your input
  container.
*/
