## ALL Maximal Scoring Subsequences ##
> Shane Neph


Overview
=========
An efficient implementation of the linear time all Maximal Scoring Subsequences algorithm. 
  
Ruzzo, W. L., and Tompa, M. 1999. A Linear Time Algorithm for Finding All 
Maximal Scoring Subsequences. Seventh International Conference on Intelligent 
Systems for Molecular Biology. 234-241.  

Given a sequence of numbers (scores) and a threshold, this algorithm finds every non-overlapping, contiguous subsequence giving greatest total score.  The threshold is often zero if you have both +/- scores, or it might be the median of your data.  The linear time algorithm improves upon the quadratic time algorithms previously known.  Numerous applications in computational biology exist.  I have used it in applications where a 2-state hidden markov model might have been used.  Not only is this method faster, but I found it to be more robust to noisy scientific measurements in several contexts.  In my applications, I cared to find those subsequences with a certain number of elements or greater and filtered out all smaller ones.  

The implementation is available in one header: include/MSS.hpp  

```
template <class ForwardIterator,
          class OutputIterator,
          class ArithmeticType>  
void AlgMSS(ForwardIterator beg, ForwardIterator end,  
            OutputIterator out, ArithmeticType threshold);  
```

Example applications of how the algorithm may be used with built-in arrays and STL containers are available in  
share/  
Briefly, beg and end mark the endpoints of your sequence of numbers, results are written to out, and threshold partitions the numbers.
A threshold of zero often makes sense for sequences of positive and negative values.  The median value might make sense when measurements are all positive.  

Some words about using the algorithm with STL containers are available at the end of each STL example file.
