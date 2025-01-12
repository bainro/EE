% MATLAB Script to load CSV, bin events into 15-minute intervals, and count occurrences

animal = "prof";

% Load the CSV file
activity_file = '/home/rbain/git/EE/RAD motion data/' + animal + '/activity.csv';
% activity_file = 'C:\Users\Priyansha\Downloads\Motion Tracking Data\powerpuff girls\' + animal + '\activity.csv';
data = readtable(activity_file);
% Convert the DateTime column to MATLAB datetime format if it's not already
data.(1) = datetime(data.(1));

% code has not been optimized to account for DST yet.
assert(range(isdst(data.(1))) == 0, 'daylight savings time change detected!');

startTime = min(data.(1));
endTime = max(data.(1));
% Generate 15-minute time bins
n_min_bins = 15;
err_txt = "Bin duration (e.g. 5 minutes) should fit nicely into a day";
assert(mod(60*24, n_min_bins) == 0, err_txt);
binEdges = startTime:minutes(n_min_bins):endTime;

% Count the number of events in each 15-minute interval
eventCounts = histcounts(data.(1), binEdges);

% filter out low counts due to cage being unplugged
usb_file = '/home/rbain/git/EE/RAD motion data/' + animal + '/usb_connection.csv';
usb_data = readtable(usb_file);
usb_data.(2) = datetime(usb_data.(2));

% presumably these edge cases are produced by the prints
% being missed from being so early after serial connection? 

% check since we make this assumption below
if ~strcmp(usb_data.(1)(1), 'plugged')
    % grab the first moment of activity and add it as necessary event
    usb_data = [table("plugged", data.(1)(1)); usb_data];
end

new_usb_data = usb_data;
last_was_unplugged = false;
for i=2:1:length(usb_data.(1))
    curr_is_unplugged = strcmp(usb_data.(1)(i), 'unplugged');
    if curr_is_unplugged
        if last_was_unplugged
            % i.e. two 'unplugged' events in a row
            % Add a "plugged in" event between using the activity data.
            % Find the first activity after the 1st 'unplugged' event,
            for o=1:1:length(data.(1))
                if data.(1)(o) > usb_data.(2)(i-1)
                    break 
                end
            end
            new_usb_data = [usb_data(1:i-1,:); table("plugged", data.(1)(o)); usb_data(i:end, :)];
        end
    else
        if ~last_was_unplugged
            % i.e. two 'plugged' events in a row
            % Add an uplugged in between using the activity data.
            % find the last activity before the 2nd 'plugged' event?
            for o=1:1:length(data.(1))
                if data.(1)(o) > usb_data.(2)(i)
                    break 
                end
            end
            new_usb_data = [usb_data(1:i-1,:); table("unplugged", data.(1)(o)); usb_data(i:end, :)];
        end
    end
    last_was_unplugged = curr_is_unplugged;
end
usb_data = new_usb_data;

%{
% this logic will introduce some, but acceptable error
j = 1;
for i=2:2:length(usb_data.(2))
    if i == length(usb_data.(2))
        continue % no activity can be logged when unplugged
    end
    unplug_t = usb_data.(2)(i);
    plug_t = usb_data.(2)(i+1);
    unplug_dur = minutes(plug_t - unplug_t);
    % loop thru binEdges until we find one further in time
    while unplug_t > binEdges(j)
        j = j + 1;
    end
    % throw away the counts for these bins
    unplug_dur = unplug_dur + n_min_bins - minutes(binEdges(j)-unplug_t);
    endBinJ = j-1+floor(unplug_dur/n_min_bins);
    if endBinJ <= length(eventCounts)
        eventCounts(j-1:endBinJ) = NaN;
    end
end
%}

% @TODO OMIT! This is hardcoded
disp("@TODO OMIT! This is hardcoded");
% Only include a certain range of dates
%%{
start_dt = datetime(2024, 12, 21);
end_dt = datetime(2025, 1, 5);
for i=1:length(eventCounts)
    if binEdges(i) < start_dt
        eventCounts(i) = NaN;
    end
    if binEdges(i) > end_dt
        eventCounts(i) = NaN;
    end
end
%}

% Loop to generate datetimes 15 minutes earlier
startDatetime = binEdges(1);
datetimesArray = startDatetime;
startOfDay = dateshift(startDatetime, 'start', 'day') + minutes(n_min_bins);
while datetimesArray(1) > startOfDay
    datetimesArray = [datetimesArray(1) - minutes(n_min_bins); datetimesArray];
end

% number of start padded bins (includes a dupe)
n_start_pad = length(datetimesArray);
if n_start_pad > 1
    eventNaNs = NaN(1, n_start_pad-1);
    eventCounts = [eventNaNs eventCounts];
    binEdges = [datetimesArray(1:n_start_pad-1).' binEdges]; 
end

% Loop to generate datetimes 15 minutes later
endDatetime = binEdges(end);
datetimesArray = endDatetime;
while datetimesArray(end) < dateshift(endDatetime, 'end', 'day')
    datetimesArray = [datetimesArray; datetimesArray(end) + minutes(n_min_bins)];
end

% number of tail padded bins (includes a dupe)
n_end_pad = length(datetimesArray);
if n_end_pad > 1
    eventNaNs = NaN(1, n_end_pad-1);
    eventCounts = [eventCounts eventNaNs];
    binEdges = [binEdges datetimesArray(2:n_end_pad).']; 
end

k = 1;
smoothedCs = movmean(eventCounts, k);

dose_file = '/home/rbain/git/EE/RAD motion data/' + animal + '/doses.csv';
dose_data = readtable(dose_file);
dose_data.(1) = datetime(dose_data.(1));

max_c = max(eventCounts);
max_c = max_c + 10 - mod(max_c, 10); % pad to next multiple of 10
num_days = length(eventCounts) / (60*24/n_min_bins);
for i=1:num_days
    start_i = 96 * (i-1) +  1;
    end_i =   96 * (i-1) + 96;
    if isnan(smoothedCs(start_i:end_i))
        continue
    end
    fig = figure();
    plot(binEdges(start_i+1:end_i+1), smoothedCs(start_i:end_i), '-', 'LineWidth', 6); 
    for j=1:length(dose_data.(1))
        curr_dose = dose_data.(1)(j);
        if curr_dose > binEdges(start_i)
            if curr_dose < binEdges(end_i)
                xline(curr_dose, 'LineWidth', 6);
            end
        end
    end
    ylim([0 max_c]);
    fontsize(24, "points");
    [~, DayName] = weekday(binEdges(start_i+10), 'long');
    title(DayName);
end

%{
if mod(num_days, 7) == 0
    total_days_shown = num_days;
else
    % helps pad our weeks with blank graphs
    total_days_shown = 7*floor(num_days/7) + 7;
end

for i = 1:total_days_shown
    subplot(floor(total_days_shown/7), 7, i);
    
    if i <= num_days
        % Plot the data for this day
        start_i = 96 * (i-1) +  1;
        end_i =   96 * (i-1) + 96;
        plot(binEdges(start_i+1:end_i+1), smoothedCs(start_i:end_i), '-', 'LineWidth', 6);  
    else
        % Plot an empty graph for missing data
        plot(NaN);  
    end
    
    %title(days_of_week{i}, 'FontSize', 12);
end

sgtitle("Activity Data for Each Day of the Week");
%}