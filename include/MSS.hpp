/*

Program: MSS
Version: 1.0

Copyright (C) 2006-2013 by Shane Neph and John Stamatoyannopoulos, Department of
Genome Sciences, University of Washington.
Contact: sjn@u.washington.edu. All rights reserved

MSS software is provided to you at no cost to you. You have the right to
perform, copy, modify, display and distribute this software. If you distribute
MSS software or modifications thereof, you agree to grant users the same rights
as are granted to you under this license.

You retain in MSS software and any modifications to MSS software, the
copyright, trademark, or other notices pertaining to MSS as provided by the
author and University of Washington.

If the use of the MSS software results in outcomes which will be published,
please specify the version of MSS software you used and cite the following
reference:

Ruzzo, W. L., and Tompa, M. 1999. A Linear Time Algorithm for Finding All
Maximal Scoring Subsequences. Seventh International Conference on Intelligent
Systems for Molecular Biology. 234-241.

You acknowledge that Shane Neph, John Stamatoyannopoulos and University of
Washington may develop modifications to MSS software that may be
substantially similar to your modifications of MSS software, and that Shane
Neph, John Stamatoyannopoulos and the University of Washington shall not be
constrained in any way by you in the use or management of such modifications.
You acknowledge the right of Shane Neph, John Stamatoyannopoulos and University
of Washington to prepare and publish modifications to MSS software that may be
substantially similar or functionally equivalent to your modifications and
improvements, and if you obtain patent protection for any modification or
improvement to MSS software, you agree not to allege or enjoin infringement of
your patent by Shane Neph, John Stamatoyannopoulos or the University of
Washington.

Any risk associated with using the MSS software is with you and your
Institution. This software is provided ``AS IS'' and any express or implied
warranties, including, but not limited to, the implied warranties of
merchantability and fitness for a particular purpose, are disclaimed. In no
event shall Shane Neph, John Stamatoyannopoulos or the University of Washington
be liable for any direct, indirect, incidental, special, exemplary, or
consequential damages (including, but not limited to, procurement of substitute
goods or services; loss of use, data, or profits; or business interruption)
however caused and on any theory of liability, whether in contract, strict
liability, or tort (including negligence or otherwise) arising in any way out
of the use of this software, even if advised of the possibility of such damage.

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
