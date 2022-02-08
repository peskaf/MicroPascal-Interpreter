{ Variation of code from http://progopedia.com/example/fibonacci/58/ }
program fibonacci;

function fib(n:integer): integer;
begin
    if (n <= 2) then
        fib := 1
    else
        fib := fib(n-1) + fib(n-2);
end;

var
    i:integer;

begin
    for i := 1 to 16 do
        begin
        	if i = 1
        	then
        		writeln(i,'st Fibonacci number is ', fib(i))
        	else
        	if i = 2
        	then
        		writeln(i,'nd Fibonacci number is ', fib(i))
        	else
        		writeln(i,'th Fibonacci number is ', fib(i))
        end;
end.