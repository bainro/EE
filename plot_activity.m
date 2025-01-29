% MATLAB Script to load CSV, bin events into 15-minute intervals, and count occurrences

% @TODO Don't include X bins immediately following dosing to infer NaNs
animal = "prof";

% Load the CSV file
activity_file = '/home/rbain/git/EE/RAD motion data/' + animal + '/activity.csv';
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

% @TODO OMIT! This is hardcoded
disp("@TODO OMIT! This is hardcoded");
% Only include a certain range of dates
%%{
start_dt = datetime(2024, 12, 14);
end_dt = datetime(2025, 1, 5);
%end_dt = datetime(2025, 1, 15);
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

saved_counts = eventCounts;

% omit weekend days for the dosed mice (index might be off by 1)
for i=1:length(eventCounts)
    if isweekend(binEdges(i))
        eventCounts(i) = NaN;
    end
end

% replace NaNs with mean value of other days at that time
byDayCounts = reshape(eventCounts, 60*24/n_min_bins, []).';
m = mean(byDayCounts, 'omitnan');
[r_i, c_i] = find(isnan(byDayCounts));
for i=1:length(r_i)
    byDayCounts(r_i(i), c_i(i))
    byDayCounts(r_i(i), c_i(i)) = m(1, c_i(i));
    m(1, c_i(i))
end
% flatten back into a row vector
byDayCounts = reshape(byDayCounts.', 1, []);

% Display the results in the CMD window
disp('Time Bins (' + string(n_min_bins) + '-minute intervals) and Event Counts:');
for i = 1:length(eventCounts)
    fprintf('%s - %s: %d events\n', ...
        datestr(binEdges(i)), datestr(binEdges(i+1)), round(byDayCounts(i)));
end

dayAvgCounts = mean(reshape(byDayCounts, 60*24/n_min_bins, []).');

k = 3;
% dynamically pad based on k to sim wrap around
wrappedCounts = [dayAvgCounts(end-k+1:end) dayAvgCounts dayAvgCounts(1:k)];
smoothedCs = movmean(wrappedCounts, k);
smoothedCs = smoothedCs(k+1:end-k);

figure;
plot(binEdges(1:60*24/n_min_bins), smoothedCs, '-', 'LineWidth', 6);

eventCounts = saved_counts;

% omit weekdays for the dosed mice (index might be off by 1)
for i=1:length(eventCounts)
    if ~isweekend(binEdges(i))
        eventCounts(i) = NaN;
    end
end

% replace NaNs with mean value of other days at that time
byDayCounts = reshape(eventCounts, 60*24/n_min_bins, []).';
m = mean(byDayCounts, 'omitnan');
[r_i, c_i] = find(isnan(byDayCounts));
for i=1:length(r_i)
    byDayCounts(r_i(i), c_i(i))
    byDayCounts(r_i(i), c_i(i)) = m(1, c_i(i));
    m(1, c_i(i))
end
% flatten back into a row vector
byDayCounts = reshape(byDayCounts.', 1, []);

% Display the results in the CMD window
disp('Time Bins (' + string(n_min_bins) + '-minute intervals) and Event Counts:');
for i = 1:length(eventCounts)
    fprintf('%s - %s: %d events\n', ...
        datestr(binEdges(i)), datestr(binEdges(i+1)), round(byDayCounts(i)));
end

dayAvgCounts = mean(reshape(byDayCounts, 60*24/n_min_bins, []).');

k = 3;
% dynamically pad based on k to sim wrap around
wrappedCounts = [dayAvgCounts(end-k+1:end) dayAvgCounts dayAvgCounts(1:k)];
smoothedCs = movmean(wrappedCounts, k);
smoothedCs = smoothedCs(k+1:end-k);
hold on;
plot(binEdges(1:60*24/n_min_bins), smoothedCs, '-', 'LineWidth', 6);

xlabel('Time (' + string(n_min_bins) + 'minute bins)');
ylabel('PIR Activity Detections');
animal = char(animal);
animal(1) = upper(animal(1));
animal = string(animal);
title('Time of Day Versus Activity for ' + animal);
%grid on; 
xtickformat('HH:mm:ss');
% remove specific date from x-axis
xticklabels(char(xticks, 'hh:mm a'));
legend({'weekdays','weekends'},'Location','northwest');
fontsize(24, "points");
hold off;