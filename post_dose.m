% Script to compare activity after dosing between DCZ & control animals

drug_animals = ["aurelion", "wukong"];
control_animals = ["lulu", "sion"];

animal = "wukong";

% amount of time to visualize after dose administration
vis_dur = 360; % mins
n_min_bins = 15; % n-minute time bins
err_txt = "Post-dose duration should be a multiple of bin duration";
assert(mod(vis_dur, n_min_bins) == 0, err_txt);

% Load the CSV file
activity_file = '/home/rbain/git/EE/RAD motion data/' + animal + '/activity.csv';
data = readtable(activity_file);
% Convert the DateTime column to MATLAB datetime format if it's not already
data.(1) = datetime(data.(1));

% code has not been optimized to account for DST yet.
assert(range(isdst(data.(1))) == 0, 'daylight savings time change detected!');

dose_file = '/home/rbain/git/EE/RAD motion data/' + animal + '/doses.csv';
%disp("@TODO hack! remove!!!");
%dose_file = '/home/rbain/git/EE/RAD motion data/mush/doses.csv';
dose_data = readtable(dose_file);
dose_data.(1) = datetime(dose_data.(1));

% @TODO only ask for DCZ group (i.e. not control milk)
validInput = false;
input_str = 'Are the first 3 doses for ' + animal + ' DCZ ramping habituation doses? [y/n]: ';
while ~validInput
    userResponse = input(input_str, 's');
    userResponse = lower(userResponse);
    switch userResponse
        case {'yes', 'y'}
            validInput = true;
            % rm 1st 3 days where they were given 1/4, 1/2, and 3/4 doses
            dose_data = dose_data(4:end,:)
        case {'no', 'n'}
            validInput = true;
        otherwise
            disp('Invalid input. Please enter "yes" or "no".');
    end
end

dose_idx = 1;
n_bins = floor(vis_dur / n_min_bins);
act_counts = zeros(1, n_bins);
for i=1:length(data.(1))
    curr_act_t = data.(1)(i);
    curr_dose_t = dose_data.(1)(dose_idx);
    if curr_act_t < curr_dose_t
        continue
    end
    startTime = curr_act_t;
    endTime = startTime + minutes(vis_dur);
    binEdges = startTime:minutes(n_min_bins):endTime;
    % Count the number of events in each n_min_bins (e.g. 15m) interval
    act_counts = act_counts + histcounts(data.(1), binEdges);
    histcounts(data.(1), binEdges)
    dose_idx = dose_idx + 1;
    if dose_idx >= length(dose_data.(1))
        break
    end
end
act_counts = act_counts / dose_idx;

figure;

x_ticks = n_min_bins:n_min_bins:n_min_bins * n_bins;
plot(x_ticks, act_counts, '-', 'LineWidth', 2); % 'MarkerSize', 6
xlabel('Time (' + string(n_min_bins) + 'minute bins)');
ylabel('PIR Activity Detections');
title('Time of Day Versus Activity for ' + animal);
% grid on; 