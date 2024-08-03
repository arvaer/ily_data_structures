A tree node has the following properties
            (data)
           /      \
      (left)      (right)
       /  \        /   \
     (l)  (r)     (l)  (r)

 and so on 

 A BST is one that is balanced. As such, there needs
 to be a way to compare these items.
 Since a set is essentially an unordered collection of objects
 how do we store items inside of it? If its going in a BST,
 then there has to be some notion of order. 
 We can just do string comparison I suppose. 
 So essentially, we can just hash things,
 turn that into a string of unsighned chars
 and after that, we can just lilterally do strcmp 
 so s1 > s2 -> so on an so forth. It should be impossible
 for s1 == s2.
 Instead of using Sha, we're going to use Base64 encoding. The man page is: EVP_EncodeINIT
 

So we need reasonable defualt options for these two following functions:
node creation and comparator. 
Default node creation will just be taking whatever binary data is there and turning it
into b64. This works since my primary usecase is strings but this can change
the default comparator will just be geq/leq
this is handled at initialization.




Dynamic sets

Dynamic set structures typically add:

    create(): creates a new, initially empty set structure.
        create_with_capacity(n): creates a new set structure, initially empty but capable of holding up to n elements.
    add(S,x): adds the element x to S, if it is not present already.
    remove(S, x): removes the element x from S, if it is present.
    capacity(S): returns the maximum number of values that S can hold.

Some set structures may allow only some of these operations. The cost of each operation will depend on the implementation, and possibly also on the particular values stored in the set, and the order in which they are inserted.
Additional operations

There are many other operations that can (in principle) be defined in terms of the above, such as:

    pop(S): returns an arbitrary element of S, deleting it from S.[1]
    pick(S): returns an arbitrary element of S.[2][3][4] Functionally, the mutator pop can be interpreted as the pair of selectors (pick, rest), where rest returns the set consisting of all elements except for the arbitrary element.[5] Can be interpreted in terms of iterate.[a]
    map(F,S): returns the set of distinct values resulting from applying function F to each element of S.
    filter(P,S): returns the subset containing all elements of S that satisfy a given predicate P.
    fold(A0,F,S): returns the value A|S| after applying Ai+1 := F(Ai, e) for each element e of S, for some binary operation F. F must be associative and commutative for this to be well-defined.
    clear(S): delete all elements of S.
    equal(S1', S2'): checks whether the two given sets are equal (i.e. contain all and only the same elements).
    hash(S): returns a hash value for the static set S such that if equal(S1, S2) then hash(S1) = hash(S2)

Other operations can be defined for sets with elements of a special type:

    sum(S): returns the sum of all elements of S for some definition of "sum". For example, over integers or reals, it may be defined as fold(0, add, S).
    collapse(S): given a set of sets, return the union.[6] For example, collapse({{1}, {2, 3}}) == {1, 2, 3}. May be considered a kind of sum.
    flatten(S): given a set consisting of sets and atomic elements (elements that are not sets), returns a set whose elements are the atomic elements of the original top-level set or elements of the sets it contains. In other words, remove a level of nesting – like collapse, but allow atoms. This can be done a single time, or recursively flattening to obtain a set of only atomic elements.[7] For example, flatten({1, {2, 3}}) == {1, 2, 3}.
    nearest(S,x): returns the element of S that is closest in value to x (by some metric).
    min(S), max(S): returns the minimum/maximum element of S.


