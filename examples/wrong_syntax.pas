program wrong_syntax;
var i : integer;
begin
    i := 5;
    while i <= 45 { 'do' missing }
        writeln(i)
end.