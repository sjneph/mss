/*

FILE: MSS.hpp
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


// Macro Guard
#ifndef MAXIMAL_SCORING_SUBSEQUENCE_H
#define MAXIMAL_SCORING_SUBSEQUENCE_H

// Files included
#include <iterator>
#include <list>
#include <utility>
#include <vector>


namespace mss {

//===============================
// Help: Optional typedef helper
//===============================
template <
         typename T, // Must be convertible to ArithmeticType of AlgMSS()
         typename InputContainerType = std::vector<T>, // May be const
         template < class Y,
                    class = std::allocator<Y> 
                  > class OutputContainerType = std::list
         >
class Help {

  template <typename U>
  struct Nested {
    typedef typename U::iterator IteratorType;
  };

  template <typename U>
  struct Nested<const U> {
    typedef typename U::const_iterator IteratorType;
  };


public:

  // typedefs
  typedef T Type;
  typedef InputContainerType InputContType;
  typedef typename Nested<InputContType>::IteratorType ForwardIterator;
  typedef std::pair<ForwardIterator, ForwardIterator> ForwardIterRange;
  typedef OutputContainerType<ForwardIterRange> DisjointRangeList;
  typedef typename DisjointRangeList::iterator OutputIterator;
};


/*
 ===========
 AlgMSS() : 
 ===========
  o ArithmeticType should be a 'regular type' and should behave as (or be) a
     built-in numeric type, such as a double.
  o iterator_traits<ForwardIterator>::value_type must be convertible to an
     ArithmeticType.
*/
template <class ForwardIterator, class OutputIterator, class ArithmeticType>
void AlgMSS(ForwardIterator beg, ForwardIterator end,
            OutputIterator out, ArithmeticType threshold) {

  // typedefs and using's
  typedef std::pair<ArithmeticType, ArithmeticType> DPType; // (L,R) in paper
  typedef std::pair<ForwardIterator, ForwardIterator> IterRange;
  typedef std::pair<IterRange, DPType> ListType;
  typedef std::list<ListType> LType;
  typedef std::list<typename LType::iterator> SearchList;
  using std::make_pair;

  // Locals
  SearchList searchList; // ==> algorithm == linear time
  typename SearchList::iterator sli;
  LType algList;
  typename LType::iterator oi, oj;
  ArithmeticType total = 0, resid = 0;
  bool found = false, nextRound = false, lastRound = false;
  DPType inProg;
  ListType keep;
  ForwardIterator mid;
  IterRange ir;

  // algorithm
  while ( nextRound || beg != end ) {
    found = false;
    resid = *beg - threshold;
    if ( nextRound || resid > 0 ) {
      if ( !nextRound ) {
        inProg.first = total;
        total += resid;
        inProg.second = total;
      }
      else
        inProg = keep.second;

      lastRound = nextRound;
      nextRound = false;
      if ( !searchList.empty() )
        oi = *searchList.begin();
      else
        oi = algList.begin();
      oj = algList.end();

      sli = searchList.begin();
      while ( oi != oj ) {
        if ( oi->second.first < inProg.first ) { // step 1 in paper
          found = true;
          if ( oi->second.second >= inProg.second ) { // step 3 in paper
            if ( !lastRound ) {
              mid = beg;
              algList.push_front(make_pair(make_pair(beg, ++mid), inProg));
            }
            else // lastRound was set from last iteration
              algList.push_front(keep);
            searchList.erase(searchList.begin(), sli);
            searchList.push_front(algList.begin());
          }
          else { // step 4 in paper
            mid = beg;
            ir = make_pair(oi->first.first, ++mid);
            DPType pt = make_pair(oi->second.first, inProg.second);
            searchList.erase(searchList.begin(), ++sli);
            algList.erase(algList.begin(), ++oi);
            nextRound = true;
            keep = make_pair(ir, pt);
          }
          break;
        }
        searchList.erase(sli++);
        if ( sli == searchList.end() )
          break;
        oi = *sli;
      } // while

      if ( !found ) { // step 2' in paper
        searchList.clear();

        if ( !algList.empty() ) {
          typedef typename LType::reverse_iterator RI;
          RI i = algList.rbegin(), j = algList.rend();
          while ( i != j ) {
            *out++ = i->first;
             ++i;
          } // while
          algList.clear();
        }

        if ( !lastRound ) {
          mid = beg;
          algList.push_front(make_pair(make_pair(beg, ++mid), inProg));
        }
        else { // lastRound was set from last iteration
          lastRound = false;
          algList.push_front(keep);
        }
        searchList.push_front(algList.begin());
      }
      else if ( nextRound )
        continue;
    }
    else // current score <= threshold
      total += resid;
    ++beg;
  } // while

  if ( !algList.empty() ) {
    typedef typename LType::reverse_iterator RI;
    RI i = algList.rbegin(), j = algList.rend();
    while ( i != j ) {
      *out++ = i->first;
      ++i;
    } // while
  }
}

} // namespace mss

#endif // MAXIMAL_SCORING_SUBSEQUENCE_H
