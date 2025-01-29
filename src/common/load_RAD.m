function load_RAD(app,fn)
    if isfile(fn)
        load(fn,'a_names');
        app.animal_names = a_names;
    end
end

