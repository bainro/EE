function read_serial(obj,~,app)

app.sdata = readline(obj);
% @NOTE timestamp based on total nano uptime
fprintf('%4.3f  %s\n',toc,app.sdata);

if contains(app.sdata,'running')
    if contains(app.sdata, 'Table #1')
        app.table_id.Text = "Table #1";
    elseif contains(app.sdata, 'Table #2')
        app.table_id.Text = "Table #2";
    elseif contains(app.sdata, 'Table #3')
        app.table_id.Text = "Table #3";
    elseif contains(app.sdata, 'Table #4')
        app.table_id.Text = "Table #4";
    end
    app.table_id.FontColor = [0 0 0];
    app.SelectUSBdeviceLabel.FontColor = [0 0 0];
    app.start.Enable = 'on';
    app.start_2.Enable = 'on';
end

if contains(app.sdata,'reward dispensed')
    if contains(app.sdata,'TOP')
        disp("lap detected on top track!")
        pt1 = app.result_directory.Value;
        pt2 = app.animal_id.Value + app.start_t + ".csv";
        pt2 = strrep(pt2,":","_"); % remove ':' from datetime
        top_csv = fullfile(pt1, pt2);
        writematrix(toc-app.tic_offset,top_csv,'WriteMode','append');
        % increment top track lap counter
        str_v = app.LapsEditField.Value;
        app.LapsEditField.Value = num2str(str2num(str_v) + 1);
    elseif contains(app.sdata,'BOTTOM')
        disp("lap detected on bottom track!");
        pt1 = app.result_directory2.Value;
        pt2 = app.animal_id_2.Value + app.start_t2 + ".csv";
        pt2 = strrep(pt2,":","_"); % remove ':' from datetime
        bot_csv = fullfile(pt1, pt2);
        writematrix(toc-app.tic_offset_2,bot_csv,'WriteMode','append');
        str_v = app.LapsEditField_2.Value;
        app.LapsEditField_2.Value = num2str(str2num(str_v) + 1);
    end
end

if contains(app.sdata, 'button pressed')
    if contains(app.sdata,'TOP')
        disp('Matlab ack top track button press')
        % ignore button press if not recording yet
        if app.stopped == false
            value = app.MotionDetectorSwitch_2.Value;
            if value == "Off"
                app.Lamp1.Color = [0, 1, 0];
                % tells arduino to resume top state machine updates
                write(app.arduino,['a' 1 1],"uint8");
                app.MotionDetectorSwitch_2.Value = "On";
            else
                app.Lamp1.Color = [1, 0, 0];
                % tells arduino to pause top state machine
                write(app.arduino,['a' 1 0],"uint8");
                app.MotionDetectorSwitch_2.Value = "Off";
            end
        end
    elseif contains(app.sdata,'BOTTOM')
        disp('Matlab ack bottom track button press')
        % ignore button press if not recording yet
        if app.stopped2 == false
            value = app.MotionDetectorSwitch.Value;
            if value == "Off"
                app.Lamp2.Color = [0, 1, 0];
                % tells arduino to resume top state machine updates
                write(app.arduino,['a' 0 1],"uint8");
                app.MotionDetectorSwitch.Value = "On";
            else
                app.Lamp2.Color = [1, 0, 0];
                % tells arduino to pause top state machine
                write(app.arduino,['a' 0 0],"uint8");
                app.MotionDetectorSwitch.Value = "Off";
            end
        end
    end
end
