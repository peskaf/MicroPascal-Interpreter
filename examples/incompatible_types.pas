program incomp_types;

function fun(num1 : integer; string1 : string): boolean;
var i : integer;
begin
    for i := 10 downto num1 do
    begin
	writeln(string1);
    end;
    fun := true;
end;

begin
    writeln(fun(1,false));
end.