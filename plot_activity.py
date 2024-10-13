import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import time

# Load the CSV file
df = pd.read_csv('Brain_Test_10_12_2024.csv')

# number of 5 min intervals
num_bins = int((df.iloc[-1,0] - df.iloc[0,0]) / 300)
hist, bins = np.histogram(df.iloc[:,0], bins=num_bins)
print(f"Bin Edges: {bins}")
print(f"Histogram Counts: {hist}")

# Plotting
plt.figure(figsize=(12, 6))
plt.plot(bins[1:], hist)
plt.title('PIR-based Activity Monitoring')
plt.xlabel('Time')
plt.ylabel('Activity Counts')
hr_ticks = ['6PM','7PM','8PM','9PM','10PM','11PM','12AM','1AM','2AM','3AM','4AM','5AM']
epoch_ticks = [1728781500 + i * 3600 for i in list(range(12))]
plt.xticks(epoch_ticks, hr_ticks, rotation=45)
plt.grid()
plt.tight_layout()
plt.show()
