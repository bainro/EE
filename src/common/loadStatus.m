     
function loadStatus(app,fn)
    err = 0;
    load(fn,'sts','ps');

    h = findobj(app.figure1,'Type','uicheckbox');
    for i = 1:length(h)
        try
               h(i).Value = sts.checkboxes(i);
        catch
            err=1;
            break;
        end
    end

     h = findobj(app.figure1,'Type','uieditfield');
     for i = 1:length(h)
         try
            h(i).Value = sts.editfields{i};
         catch
             err=1;
             break;
         end
     end

     h = findobj(app.figure1,'Type','uinumericeditfield');
     for i = 1:length(h)
         try
            h(i).Value = sts.uinumericeditfields{i};
         catch
             err=1;
             break;
         end
     end


     h = findobj(app.figure1,'Type','uiradiobutton');
      for i = 1:length(h)
          try
            h(i).Value = sts.uiradiobuttons{i};
          catch
             err=1;
              break;
          end
      end

     h = findobj(app.figure1,'Type','uiDropDown');
      for i = 1:length(h)
           try
             h(i).Value = sts.dropdowns{i};
           catch
            err=1;
            break
           end
      end

       app.figure1.Position = ps;

end        
        