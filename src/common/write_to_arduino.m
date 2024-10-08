
function  write_to_arduino(app,vl)

l =  mod(vl,256); 
h =  floor(vl/256); 
write(app.arduino,[l h],"uint8");
 % while ~contains(char(app.sdata),'ackl')
     % toc;
     % drawnow;
     % pause(0.1);
% end

