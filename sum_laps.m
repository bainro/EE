animal_ids = ["bubbles" "prof" "mojojojo" "buttercup"];

for i=1:length(animal_ids)
    csv_dir = 'C:\Users\Priyansha\Downloads\Lap Tracking Data\Lap Tracking Data\powerpuff girls';
    csv_files = dir([csv_dir '/*.csv']);
    total_laps = 0;
    num_files = length(csv_files);
    num_days_this_animal = 0;
    for k=1:num_files
        filename = [csv_dir '/' csv_files(k).name];
        base_name = lower(csv_files(k).name);
        if contains(base_name, animal_ids(i))
            num_laps = length(readmatrix(filename));
            total_laps = total_laps + num_laps;
            num_days_this_animal = num_days_this_animal + 1;
        end
    end
    
    disp(total_laps + " total laps for " + animal_ids(i));
    disp(total_laps / num_days_this_animal + " average laps over the " + num_days_this_animal + " days.");
end
