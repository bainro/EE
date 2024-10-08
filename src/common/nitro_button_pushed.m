function  nitro_button_pushed(app);

app.nitro_state = ~app.nitro_state;

if app.nitro_state
    app.nitro.BackgroundColor = [0 1 0];
    write(app.arduino,['u' app.nitro_pin  1],"uint8");
else
    app.nitro.BackgroundColor = [0.96 0.96 0.96];
    write(app.arduino,['u' app.nitro_pin  0],"uint8");
end