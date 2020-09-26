datatype 'a Tree = Empty | Node of 'a * 'a Tree * 'a Tree

fun insert x Empty = Node(x, Empty, Empty)
    (* Nothing in stored, so we have to reconstruct the tree.*)
  | insert x (Node(y, left, right)) =
        if x < y then
            Node(y, insert x left, right)
        else 
            Node(y, left, insert x right)


fun member x Empty = false
  | member x (Node(y, left, right)) =
        if x < y then
            member x left
        else if x > y then 
            member x right
        else 
            true