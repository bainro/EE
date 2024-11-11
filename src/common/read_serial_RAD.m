function read_serial_RAD(obj,~,app)
    app.sdata = readline(obj);
    % @NOTE timestamp based on total nano uptime
    fprintf('%4.3f  %s\n',toc,app.sdata);
    
    if contains(app.sdata, 'MOTION CAGE')
        p1 = app.result_directory.Value;
        i = str2num(erase(app.sdata, "MOTION CAGE "));
        p2 = app.animal_names(i) + ".csv";
        csv_path = fullfile(p1, p2);
        t = string(datetime());
        writematrix(t,csv_path,'WriteMode','append');
    elseif contains(app.sdata, 'PLUGGED IN')
        i = str2num(erase(app.sdata, "PLUGGED IN CAGE "));
        name = app.animal_names(i);
        disp("[matlab] cage #" + i + " (" + name + ") plugged in");
        app.CageDropDown.Value = app.CageDropDown.Items{i};
        app.change_cage_pub();
    elseif contains(app.sdata, 'UNPLUGGED')
        i = str2num(erase(app.sdata, "UNPLUGGED CAGE "));
        name = app.animal_names(i);
        disp("[matlab] cage #" + i + " (" + name + ") unplugged");
        app.CageDropDown.Value = app.CageDropDown.Items{i};
        app.change_cage_pub();
    end
end