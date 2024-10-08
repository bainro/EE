function  init_fx(app)

rng('shuffle');
tic;

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