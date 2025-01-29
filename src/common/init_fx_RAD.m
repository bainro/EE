function  init_fx_RAD(app)

    rng('shuffle');
    tic;
    
    load_RAD(app,'RAD_settings.mat');
    % updates cage_1's name after the load above
    change_cage_pub(app, []);

    % look for arduino nanos
    devs = fcom();
    [r,c] = size(devs);
    nanos = [];
    for i = 1:r
        [str,n] = devs{i,:};
        if ~isempty(str)
            nanos = [nanos string(n)];
        end
    end
    
    % no arduinos detected :c
    if isempty(nanos)
        fg = uifigure;
        msg = 'No Arduino Detected, Connnect Arduino and try again!';
        title = 'No Arduino Detected';
        an = uiconfirm(fg,msg,title,'Options',{'OK'});
        delete(fg);
        return;
    end
    
    app.nano_list.Items = nanos;
    assert(length(nanos) < 2, "Too many arduinos detected!");
    comstr = ['COM' num2str(nanos(1))];
    
    app.arduino = [];
    % create serial communication object on port COM
    app.arduino=serialport(comstr,115200);
    app.arduino.Timeout = 0.05;

    function errFcn(e, app)
        if contains(e.ID, 'lostConnectionState')
            fig = uifigure;
            uialert(fig,"The serial connection to the Arduino was lost. This app will be forced to exit. The RAD GUI must be restarted.","DISCONNECTED!");
            app.delete();
        end
    end
    app.arduino.ErrorOccurredFcn = @(e)errFcn(e, app);
    
    % start serial reads in background
    configureTerminator(app.arduino,"CR/LF");
    configureCallback(app.arduino,"terminator",@(src,evnt)read_serial_RAD(src,evnt,app))

end