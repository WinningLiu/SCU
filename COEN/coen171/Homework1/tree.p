program tree(input, output);

type
    tree1 = ^node;

    node = record
        info : integer;
        left : ^node;
        right : ^node
    end;

function insert(var root : tree1; value : integer) : tree1;
begin
    if root = nil then begin
        new(root);
        root^.left := nil;
        root^.right := nil;
        root^.info := value
    end
    else if root^.info < value then
        root^.right := insert(root^.right, value)
    else if root^.info > value then
        root^.left := insert(root^.left, value);

    insert := root
end;

function member(root : tree1; value : integer) : boolean;
begin
    if root = nil then
        member := false
    else if root^.info = value then
        member := true
    else if root^.info < value then
        member := member(root^.right, value)
    else 
        member := member(root^.left, value)
end;

var t : tree1;
var x : integer;
begin
    t := nil;
    readln(x);
    
    if x <> -1 then begin
    repeat
        t := insert(t, x);
        readln(x);
    until x = -1;
    end;

    readln(x);

    if x <> -1 then begin
    repeat 
        writeln(member(t, x));
        readln(x);
    until x = -1
    end;
end.