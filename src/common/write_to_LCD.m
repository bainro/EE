function write_to_LCD(app,str1,str2)

str1 = ['<'  str1  '>'];
str2 = ['<'  str2  '>'];
% need to always do both rows
write(app.arduino,'l',"uint8");  % activate first row
write(app.arduino,str1,"char");  % write first row
write(app.arduino,'m',"uint8"); % activate second row
write(app.arduino,str2,"char"); % write second row
            