function read_serial_RAD(obj,~,app)
    app.sdata = readline(obj);
    % @NOTE timestamp based on total nano uptime
    fprintf('%4.3f  %s\n',toc,app.sdata);
    
    if contains(app.sdata, 'MOTION CAGE')
        p1 = app.result_directory.Value;
        i = str2num(erase(app.sdata, "MOTION CAGE "));
        a_dir = fullfile(p1, app.animal_names(i));
        if ~exist(a_dir, 'dir')
           mkdir(a_dir);
        end
        csv_path = fullfile(a_dir, "activity.csv");
        t = string(datetime());
        writematrix(t,csv_path,'WriteMode','append');
    elseif contains(app.sdata, 'PLUGGED IN')
        i = str2num(erase(app.sdata, "PLUGGED IN CAGE "));
        name = app.animal_names(i);
        disp("[matlab] cage #" + i + " (" + name + ") plugged in");
        app.CageDropDown.Value = app.CageDropDown.Items{i};
        app.USBs(i) = true;
        app.change_cage_pub();
        % record it was plugged in at this time in CSV file
        p1 = app.result_directory.Value;
        a_dir = fullfile(p1, name);
        if ~exist(a_dir, 'dir')
           mkdir(a_dir);
        end
        csv_path = fullfile(a_dir, "usb_connection.csv");
        t = ["plugged" string(datetime())];
        writematrix(t, csv_path, 'WriteMode', 'append');
    elseif contains(app.sdata, 'UNPLUGGED')
        i = str2num(erase(app.sdata, "UNPLUGGED CAGE "));
        name = app.animal_names(i);
        disp("[matlab] cage #" + i + " (" + name + ") unplugged");
        app.CageDropDown.Value = app.CageDropDown.Items{i};
        app.USBs(i) = false;
        app.change_cage_pub();
        % record it was unplugged
        p1 = app.result_directory.Value;
        a_dir = fullfile(p1, name);
        if ~exist(a_dir, 'dir')
           mkdir(a_dir);
        end
        csv_path = fullfile(a_dir, "usb_connection.csv");
        t = ["unplugged" string(datetime())];
        writematrix(t, csv_path, 'WriteMode', 'append');
    end
end