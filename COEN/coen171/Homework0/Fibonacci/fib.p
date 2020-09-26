(* Print the 10th Fibonacci number. *)

program Fibonacci(input, output);
    function fib(n : integer) : integer;
    begin
	if (n = 0) or (n = 1) then
	    fib := 1
	else
	    fib := fib(n - 1) + fib(n - 2)
    end;
begin
    writeln(fib(10))
end.
