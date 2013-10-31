## Maximal Scoring Subsequences (MSS) ##
> Shane Neph


Overview
=========
An efficient implementation of the linear time all Maximal Scoring Subsequences algorithm. 
  
Ruzzo, W. L., and Tompa, M. 1999. A Linear Time Algorithm for Finding All 
Maximal Scoring Subsequences. Seventh International Conference on Intelligent 
Systems for Molecular Biology. 234-241.  

Given a sequence of numbers (scores), this algorithm finds every non-overlapping, contiguous subsequence giving greatest total score.  The linear time algorithm improves upon the quadratic time algorithms previously known.  Numerous applications in computational biology exist.  I have used it in applications where a 2-state hidden markov model might have been used.  Not only is this method faster, but I found it to be more robust to noisy scientific measurements in several contexts.  In my applications, I cared to find subsequences of a certain size or greater and filtered all smaller ones out.

The implementation is available in one header: include/MSS.hpp  


Example applications of how the algorithm may be used with built-in arrays and STL containers are available
in share/

Some words about using it with STL containers are available at the end of each STL example file.
