
import csv
import time
import http.server
from urllib.parse import urlparse, parse_qs

class httpHandler(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        # Parse the URL
        parsed_path = urlparse(self.path)
        query_params = parse_qs(parsed_path.query)

        # Check if the requested path is /motion
        if parsed_path.path == '/motion':
            animal_id = query_params.get('animal_id', [None])[0]
            if animal_id == None:
                return
            # Open the CSV file in append mode
            with open(f'./{animal_id}.csv', mode='a') as f:
                writer = csv.writer(f)
                writer.writerow([time.time()])
            self.send_response(200)  # HTTP status 200 OK
            self.send_header('Content-type', 'text/plain')
            self.end_headers()
            response_message = f'acknowledged'
            self.wfile.write(response_message.encode())
        # allows us to run and stop from matlab GUI
        elif parsed_path.path == '/shutdown':
            exit(0)

def run(server_class=http.server.HTTPServer, handler_class=httpHandler, port=8080):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    httpd.serve_forever()

if __name__ == "__main__":
    run()