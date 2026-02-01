#!/usr/bin/env python3
import sys
import time

# Headers ديال CGI
sys.stdout.write("Content-Type: text/plain\r\n\r\n")
sys.stdout.flush()

for i in range(70):
    sys.stdout.write(f"Line {i + 1}: This is a slow CGI output...\n")
    sys.stdout.flush() 
    time.sleep(1)

sys.stdout.write("\n--- CGI Finished ---\n")
sys.stdout.flush()
