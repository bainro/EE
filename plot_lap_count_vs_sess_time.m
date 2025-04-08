animal_ids = ["aurelion" "sion" "lulu" "wukong" "pikachu" "cubone" "rayquaza" "arceus"];

accumulated_laps = [];

for i=1:length(animal_ids)
    csv_dir = 'C:\Users\Dingus\Desktop\LOLDATA';
    csv_files = dir([csv_dir '/*.csv']);
    total_laps = 0;
    num_files = length(csv_files);
    num_days_this_animal = 0;
    for k=1:num_files
        filename = [csv_dir '/' csv_files(k).name];
        base_name = lower(csv_files(k).name);
        if contains(base_name, animal_ids(i))
            lap_times = readmatrix(filename) / 60;
            num_laps = length(lap_times);
            for l=1:num_laps
                assert(lap_times(l) >= 0, 'values should be POS!');
            end
            accumulated_laps = [accumulated_laps transpose(lap_times)];
            total_laps = total_laps + num_laps;
            num_days_this_animal = num_days_this_animal + 1;
        end
    end
end

% Set the bin size to 2 mins
bin_size = 1; 

% Create custom bin edges, covering the range of the data
min_edge = 0;
max_edge = 60;

% Create the bin edges starting from the minimum to the maximum, with a step size of 2
edges = min_edge:bin_size:max_edge;

figure;
histogram(accumulated_laps, edges);
title('test');
xlabel('session time (mins)');
ylabel('number of laps');
grid on;  % Display grid lines
