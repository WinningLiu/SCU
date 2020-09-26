quicksort([],[])
quicksort([H|T], X) :- partition(T, L, R, H), 
quicksort(L, Lsort), 
quicksort(R, Rsort), 
append(Lsorted, [H|Rsort], X).

partition([],[],[],_).
partition([H|X],[H|Y], Z, P) :- H < P, partition(X, Y, Z, P).
partition([H|X],Y, [H|Z], P) :- H >= P, partition(X, Y, Z, P).