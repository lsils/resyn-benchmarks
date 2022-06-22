# Benchmarks for the generalized resynthesis problem

## Problem definition
The *resynthesis problem* is the problem of finding a *dependency function*, represented by a *dependency circuit*, to re-express a *target function* using some of the given *divisor functions*. More precisely, given a collection of divisor functions `d_1, d_2, ..., d_N` and a target function `f` over the same set of variables `X = x_1, ..., x_I`, find a dependency function `h`, such that `f(X) = h( d_1(X), d_2(X), ..., d_N(X) )` for all possible value assignments to `X`.

The divisor functions and the target function are represented by simulation signatures, which are ordered values of the functions under a certain set of input value assignments (which can be incomplete).

The (single-output, completely-specified) resynthesis problem is generalized in two aspects: (1) incompletely-specified divisors and (2) multiple target functions with interdependent don't-care relations.

## Format
The benchmarks are stored in files with the `.resyn` extension. The format consists of (1) the mandatory problem specification, (2) optional solution record(s), and (3) optional comment section. Empty lines may be added to enhance readability and should be ignored by a parser.

#### Problem
The benchmark format starts with a line of the header:
```
resyn I N T L
```
where `I` is either the number of input variables (if projection functions are included in the divisor set) or 0 (otherwise),  
`N` is the number of divisor functions excluding projection functions (if any),  
`T` is the number of target functions,  
and `L` is the length of the simulation signatures.

After the header follows `I` lines of simulation signatures of the inputs, then `N` lines of simulation signatures of the divisors, and finally `2^T` lines of the specification of the target functions.

Each line of simulation signature consists of `L` characters of either `0`, `1`, or `-`. Note that a `-` bit does **not** mean it can be either 0 or 1 (like a don't-care), but rather that it is *neither* 0 nor 1, i.e., this bit of this divisor cannot contribute to constructing the target function. For example, given two divisors `d_1 = 1--0` and `d_2 = --01`, AND-ing them will give `d_1 AND d_2 = --00`.

The last `2^T` lines specify how the value combinations of the target functions are accepted. The `i`-th line (with `i` starting from 0) consists of `L` characters of either `0` or `1`, where a `1` bit signifies that, at this bit position, the target functions may evaluate to `(f_T, f_{T-1}, ..., f_1) = (i)_2`, where `(i)_2` is the binary representation of `i`. For example, the following specification (note that the comments after `//` are not part of the format)
```
0001 // (0)_2 = 00 : f_2 = 0, f_1 = 0
1011 // (1)_2 = 01 : f_2 = 0, f_1 = 1
0111 // (2)_2 = 10 : f_2 = 1, f_1 = 0
0101 // (3)_2 = 11 : f_2 = 1, f_1 = 1
```
is satisfied by a dependency circuit producing the following simulation signatures at the outputs
```
f_1 = 1101
f_2 = 011-
```
For `f_1`, the first bit must be 1, the second bit can be anything, the third bit must be different from `f_2`, and the fourth bit can be anything.  
For `f_2`, the first bit must be 0, the second bit must be 1, the third bit must be different from `f_1`, and the fourth bit can be anything.

Note that if there is a bit position where all of the `2^T` lines are 0, then the problem does not have a solution.

With `1 + I + N + 2^T` lines, a generalized resynthesis problem is specified. A benchmark file might end here. Or, more information may be given:

#### Possible/best solution
A benchmark may be delivered with one or multiple solution(s) in form of dependency circuit(s). A solution record starts from a solution header:
```
solution NAME TYPE K
```
where `NAME` is an arbitrary string without white space to identify different solutions,  
`TYPE := {aig | xag | mig}` represents the type of the dependency circuit,  
and `K` is the size of the circuit (number of gates).

Then, the solution circuit is written as a list of literals. A literal is a number `(2 * i + c)`, where `i` is an index, with 0 indexing the constant 0, 1 to `(I + N)` indexing the divisors, and all successive indexes for gates in the dependency circuit. A gate is represented by a pair (for aig and xag) or triple (for mig) of literals of their fanins. (For XAGs, an AND gate is represented by a pair of literals where the first literal is smaller than the second one, whereas the opposite represents an XOR gate.) After all gates are listed, outputs realizing the target functions are listed, also in terms of literals.

For example, the following records a solution to a resynthesis problem with `I = 0, N = 4` and `T = 1`, which is an XAG consisting of 3 gates and having the output function `(d1 AND d2) XOR (d3 AND d4)`:
```
solution example xag 3
2 4 6 8 12 10 14
```

#### Comments
Additional information may be recorded in the optional comment section. The comment section must be in the end of the file and start with a line (similar to the AIGER format)
```
c
```
Everything after this line should be ignored by a parser.
