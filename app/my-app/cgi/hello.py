#!/usr/bin/env python3
import os
import sys

# 1. MANDATORY: The HTTP Response Header
# CGI scripts must print the headers followed by a blank line (\r\n\r\n)
print("Content-Type: text/html\r\n\r\n")

# 2. HTML Structure
print("<!DOCTYPE html>")
print("<html>")
print("<head><title>CGI Environment Inspector</title></head>")
print("<style>body{font-family:sans-serif; padding:20px;} table{border-collapse:collapse; width:100%;} th,td{border:1px solid #ccc; padding:8px; text-align:left;} th{background:#eee;}</style>")
print("<body>")

print("<h1>CGI Environment Variables</h1>")
print("<p>This list shows every variable your C++ server passed via <code>execve</code>.</p>")

# 3. List the Environment Variables
print("<table>")
print("<tr><th>Variable Name</th><th>Value</th></tr>")

# os.environ.items() iterates through the char** envp array
# Keys are sorted for better readability
for key in sorted(os.environ.keys()):
    print(f"<tr><td><strong>{key}</strong></td><td>{os.environ[key]}</td></tr>")

print("</table>")

# 4. Handle POST Data (if any)
if os.environ.get("REQUEST_METHOD") == "POST":
    print("<h2>POST Body Data</h2>")
    try:
        # We MUST use CONTENT_LENGTH to know how many bytes to read from stdin
        content_length = int(os.environ.get('CONTENT_LENGTH', 0))
        if content_length > 0:
            body = sys.stdin.read(content_length)
            print(f"<pre style='background:#f4f4f4; padding:10px;'>{body}</pre>")
        else:
            print("<p><i>Body is empty (CONTENT_LENGTH was 0 or missing).</i></p>")
    except Exception as e:
        print(f"<p style='color:red;'>Error reading stdin: {e}</p>")

print("</body></html>")