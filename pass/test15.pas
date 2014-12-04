var
	x1: string;
	x2: string;
	x3: string;
	x4: string;
	tmp: string;
	a: integer;

function cmp_string(str1: string; str2: string): integer;
begin

	write('-----------------------------------------'#10'');
	write('"'+str1+'"', ' & ', '"'+str2+'"', ''#10'');
	write('-----------------------------------------'#10'');
	if str1 < str2 then
	begin
		write('"'+str1+'"', ' je mensi nez          ', '"'+str2+'"', ''#10'')
	end
	else begin end;

	if str1 <= str2 then
	begin
		write('"'+str1+'"', ' je mensi nebo roven   ', '"'+str2+'"', ''#10'')
	end
	else begin end;

	if str1 > str2 then
	begin
		write('"'+str1+'"', ' je vetsi nez          ', '"'+str2+'"', ''#10'')
	end
	else begin end

	if str1 >= str2 then
	begin
		write('"'+str1+'"', ' je vetsi nebo roven   ', '"'+str2+'"', ''#10'')
	end
	else begin end

	if str1 = str2 then
	begin
		write('"'+str1+'"', ' je roven              ', '"'+str2+'"', ''#10'')
	end
	else begin end;

	write(''#10'');
	cmp_string := 3
end;

{ Test prace s retezci }
begin
	x1 := 'this ';
	x2 := 'team ';
	x3 := 'was ';
	x4 := 'awesome';


	write('konkatenace'#10'');
	tmp := x1 + x2 + x3 + x4 + ''#10''#10'';
	write(tmp);


	write('indexy jednotlivych podretezcu:'#10'');
	write(find(tmp, x1), ' ', find(tmp, x2), ' ', find(tmp ,x3), ' ', find(tmp, x4), ''#10''#10'');



	write('experimenty s podretezci:'#10'');
	write(copy(x1, 2, 3) + ' ');
	write(copy(x2, 1, 6000)); 		{ preteceni retezce slova - nemelo by nic zpusobit}
	write(copy(x1, 3, 4));
	write(copy(x4, 1, 7), ''#10''#10'');

	
	write('porovnavani retezcu:'#10'');
	a := cmp_string(x1, x2);
	a := cmp_string('', x4);
	a := cmp_string('', '');


	write('jednotlive retezce serazene bez mezer podle abecedy:'#10'');
	write(sort(copy(x1, 1, 4)), ''#10'');
	write(sort(copy(x2, 1, 4)), ''#10'');
	write(sort(copy(x3, 1, 3)), ''#10'');
	write(sort(x4), ''#10''#10'');

	write('delky jednotlivych retezcu:'#10'');
	write(x1+' ', length(x1), ''#10'');
	write(x2+' ', length(x2), ''#10'');
	write(x3+' ', length(x3), ''#10'');
	write(x4+' ', length(x4), ''#10'')
end.