% MATLAB Script to load CSV, bin events into 15-minute intervals, and count occurrences

% @TODO compare Prof's weekdays vs weekends
% @TODO how long does the drug affect Prof? How long until he's back to his
% non-drugged days (e.g. weekends) and the rest of the girl's activity?
% @TODO Don't include X bins immediately following dosing to infer NaNs
% @TODO Subplots comparing animals?
animal = "Prof";

% Load the CSV file
activity_file = 'C:\Users\Priyansha\Downloads\Motion Tracking Data\powerpuff girls\' + animal + '\activity.csv';
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
usb_file = 'C:\Users\Priyansha\Downloads\Motion Tracking Data\powerpuff girls\' + animal + '\usb_connection.csv'; 
usb_data = readtable(usb_file);
usb_data.(2) = datetime(usb_data.(2));
% check since we make this assumption below
assert(strcmp(usb_data.(1)(1), 'plugged'), 'First entry expected to be "plugged"');
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


k = 5;
% dynamically pad based on k to sim wrap around
wrappedCounts = [dayAvgCounts(end-k+1:end) dayAvgCounts dayAvgCounts(1:k)];
smoothedCs = movmean(wrappedCounts, k);
smoothedCs = smoothedCs(k+1:end-k);

figure;
plot(binEdges(1:60*24/n_min_bins), smoothedCs, '-', 'LineWidth', 2);
%plot(binEdges(2:end), smoothedCs, '-o', 'LineWidth', 2, 'MarkerSize', 6);
xlabel('Time (' + string(n_min_bins) + 'minute bins)');
ylabel('PIR Activity Detections');
title('Time of Day Versus Activity for ' + animal);
%grid on; 
xtickformat('HH:mm:ss');
% remove specific date from x-axis
xticklabels(char(xticks, 'hh:mm a'));