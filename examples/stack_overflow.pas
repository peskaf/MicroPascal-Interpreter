program stack_overflow;

{ recursion without base case }
function a(num1 : integer): integer;
var result : integer;
begin
   result := a(num1) + 1;
   a := result;
end;

begin
   writeln(a(0));
end.