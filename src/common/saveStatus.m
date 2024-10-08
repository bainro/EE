function saveStatus(app,fn)      
    h = findobj(app.figure1,'Type','uicheckbox');
    xarr = zeros(length(h),1);
    for i = 1:length(h)
       xarr(i) = h(i).Value;
    end
    % creates sts struct
    sts.checkboxes = xarr;

    h = findobj(app.figure1,'Type','uieditfield');
    xa = [];
    for i = 1:length(h)
        xa{i} = h(i).Value;
    end
    sts.editfields = xa;

    h = findobj(app.figure1,'Type','uinumericeditfield');
    xa = [];
    for i = 1:length(h)
        xa{i} = h(i).Value;
    end
    sts.uinumericeditfields = xa;

    h = findobj(app.figure1,'Type','uiradiobutton');
    xa = [];
    for i = 1:length(h)
        xa{i} = h(i).Value;
    end
    sts.uiradiobuttons = xa;

    h = findobj(app.figure1,'Type','uiDropDown');
    xa = [];
    for i = 1:length(h)
        % fprintf('%s \n',h(i).Tag);
        xa{i} = h(i).Value;
    end
    sts.dropdowns = xa;
     
    ps = app.figure1.Position;
    save(fn,'sts','ps');
end