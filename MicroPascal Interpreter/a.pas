
{
{Function example from https://www.tutorialspoint.com/pascal/pascal_functions.htm}
program exFunction;
var
   a, b, ret : integer;

{function definition}
function max(num1, num2: integer): integer;
var
   {local variable declaration}
   result: integer;

begin
   if (num1 > num2) then
      result := num1
   
   else
      result := num2;
   max := result;
end;

begin
   a := 100;
   b := 200;
   {calling a function to get max value}
   ret := max(a, b);
   
   writeln( 'Max value is : ', ret );
end.}

{Example from https://www.tutorialspoint.com/pascal/pascal_variable_scope.htm}
program exGlobal;
var
   a, b, c: integer;
procedure display;

var
   a, b, c: integer;

begin
   { local variables }
   a := 10;
   b := 20;
   c := a + b;
   
   writeln('Winthin the procedure display');
   writeln(' Displaying the global variables a, b, and c');
   
   writeln('value of a = ', a , ' b =  ',  b, ' and c = ', c);
   writeln('Displaying the local variables a, b, and c');
   
   writeln('value of a = ', a , ' b =  ',  b, ' and c = ', c);
end;

begin
   a:= 100;
   b:= 200;
   c:= 300;
   
   writeln('Winthin the program exlocal');
   writeln('value of a = ', a , ' b =  ',  b, ' and c = ', c);   
   
   display();
end.