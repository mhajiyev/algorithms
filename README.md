# Algorithms and Data Structures
Various algorithm implementations

# Knapsack Problem
* Solution uses dynamic programming approach
* Runtime: O(mn) where n is the number of items and m is the size of the knapsack

# Prim's Algorithm
Improved Prim's algorithm for finding minimum spanning tree
Runtime : O(V^2) where V is the number of vertices in the graph

# Kruskal's Algorithm 
Improved Kruskal's algorithm for finding minimum spanning tree
Runtime : O(E*logE) where E is the number of edges in the graph

# DNA Sequence Alignment Problem
*The solution employs dynamic programming approach
*There is a user defined gap and mismatch penalty ; goal is to align two DNA strings with minimum penalty incurred
*Runtime: O(ab) where a is the length of string1 and b is the length of string2 

# Dual Simplex algorithm with general bounds 
*for solving linear programs that uses duality theory. This algorithm commences with an artificially defined basis at the first iteration. Consequently, it removes some basic variables from the basis in addition to adding variables to the basis that are either at lower bound or at upperbound.When optimality&(in)/feasibilty conditions are reached the algorithm stops.Generally, this algorithm is designed to solve problems of type:

minimize cx
such that Ax=b
where l<=x<=u

x,c,l,u are vectors of size n with rational entries
b is a vector of size m with rational entries
A is a constraint matrix of size (m,n)
The algorithm computes x* where x*<x for any basic solution x.

# Kosaraju's algorithm for finding the number of strongly connected components in a directed graph
