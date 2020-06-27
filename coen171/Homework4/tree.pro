insert(x, node(root, l, r), node(root, newL, r)) := x < root,!, insert(x,l,newl).
insert(x, node(root, l, r), node(root, l, newR)) := insert(x, r, newR).

exists(x, node(x, _, _)).
exists(x, node(root, l, _)) := x < y, !, exists(x, l).
exists(x, node(y, _, r)) := exists(x,r).