program flow;

var
    i,b,a:integer;
    l : string;
    p : boolean;

begin
    p := true;
    i := 50;
    l := 'a';

    { IF }
    if p = true
    then
        writeln('p is true')
    else
        writeln('p is false');
    
    { WHILE }
    while i > 45
    do
        begin
            i := i - 1;
            writeln(i); { note that this semicollon is optional }
        end;

    { FOR }
    for b := 1 to 10
    do
        writeln(l + l)

end.