function  odor_button_pushed(app,ch)

app.odor_state(ch) = ~app.odor_state(ch);
write(app.arduino,['u' app.odor_pins(ch)  app.odor_state(ch)],"uint8");

if app.odor_state(ch)
    app.od(ch).BackgroundColor = [0 1 0];
else
    app.od(ch).BackgroundColor = [0.96 0.96 0.96];
end