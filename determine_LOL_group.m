% MATLAB Script to load CSV, bin events into 15-minute intervals, and count occurrences

figure;
animals = ["Aurelion", "Sion", "Vex", "Lulu", "WUKONG"]

for l=1:length(animals)

    animal = animals(l);

    % Load the CSV file
    activity_file = 'C:\Users\MCN Lab User\Desktop\EE\' + animal + '\activity.csv';
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
    usb_file = 'C:\Users\MCN Lab User\Desktop\EE\' + animal + '\usb_connection.csv'; 
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

    k = 5;
    smoothedCs = movmean(eventCounts, k);

    % it is not necessarily in this order...
    %days_of_week = {'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday', 'Sunday'};
    num_days = binEdges(1:60*24/n_min_bins);

    num_days = length(eventCounts) / (60*24/n_min_bins);
    if mod(num_days, 7) == 0
        total_days_shown = num_days;
    else
        % helps pad our weeks with blank graphs
        total_days_shown = 7*floor(num_days/7) + 7;
    end

    for i = 1:total_days_shown
        if i > 3
            continue;
        end

        subplot(length(animals), 3, i+(l-1)*3);
        disp("MODIFIED FOR ONE-OFF TASK! NOT GENERAL PROGRAM ANYMORE!!")
        
        if i <= num_days
            % Plot the data for this day
            start_i = 96 * (i-1) +  1;
            end_i =   96 * (i-1) + 96;
            plot(binEdges(start_i+1:end_i+1), smoothedCs(start_i:end_i), '-', 'LineWidth', 2);  
        else
            % Plot an empty graph for missing data
            plot(NaN);  
        end

        title(string(animal), 'FontSize', 12);
    end

end
