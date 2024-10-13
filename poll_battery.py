import csv
import time
import http.client

url = r"192.168.0.3"
csv_file = r"battery_levels.csv" 

# Function to poll the website
def poll_website():
    conn = http.client.HTTPConnection(url, 80)
    conn.request("GET", "/battery")  # You can modify the path if needed
    response = conn.getresponse()
    return response.read().decode()  # Returns the status and content

# Function to append response to CSV
def append_to_csv(timestamp, V):
    with open(csv_file, mode='a', newline='') as file:
        writer = csv.writer(file)
        writer.writerow([timestamp, V])

# Main polling loop
try:
    while True:
        timestamp = time.time()
        voltage = poll_website()
        append_to_csv(timestamp, voltage)
        time.sleep(60)  # Wait for 1 minute
        
except KeyboardInterrupt:
    print("Polling stopped.")