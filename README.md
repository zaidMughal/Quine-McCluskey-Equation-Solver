# Quine-Mcklusky-Equation-Solver
  The Quine–McCluskey algorithm(or the method of prime implicants) is a method used for minimization of Boolean functions that was developed by Willard V. Quine and extended by Edward J. McCluskey. It is functionally identical to Karnaugh mapping a basic Boolean equation solving method), but the tabular form makes it more efficient for use in computer algorithms, and it also gives a deterministic way to check that the minimal form of a Boolean function has been reached. It is sometimes referred to as the tabulation method.
  
  Another advantage this method has is that it not only gives a simplified solution but also gives all possible solutions. Though in this readme, I am not explaining more about this method as many resources are available online.
  
  This program uses linked list of arrays of linked lists as its structure to compute and get to end result and gives all possible solutions.The pre-built libraries this program uses is tribool library from boost C++ library package. https://www.boost.org/users/download/
  
  This include support for a new type of variable as well as its mathematical operations. It can store three states: true, false and undefined rather then conventional two states of boolean. Why We need this? To gain most simplified solution we need to make full use of dont care conditions too, which are denoted by this third state. So to not spend time in making it from scratch, we used this library.
