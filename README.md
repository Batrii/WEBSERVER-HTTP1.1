# WEBSERVER-HTTP1.1

A lightweight, non-blocking HTTP/1.1 web server written from scratch in **C++98**, built as a project for the **1337/42 network** curriculum (the *webserv* project). It implements the core of the HTTP protocol on top of raw sockets — request parsing, static file serving, directory listing, redirects, CGI execution, file uploads, and multi-client handling driven entirely by `epoll`.

No third-party HTTP or networking libraries are used — sockets, the event loop, and the protocol layer are all hand-rolled, as required by the subject.

## Features

- **HTTP/1.1 methods:** `GET`, `POST`, `DELETE`
- **Non-blocking, event-driven core** built on `epoll` (single-threaded, single-process)
- **Multiple virtual servers**, each with its own port, name, and routing table, all multiplexed on one event loop
- **JSON-based configuration** — routes, ports, index files, root directories, custom error pages, and CGI settings are all defined per server
- **Static file serving** with directory listing (`dictlist`) support
- **Redirects** (internal and external)
- **CGI execution** (e.g. Python), including CGI timeout handling and validation of CGI response headers
- **File uploads and deletes** via `POST` / `DELETE`
- **Chunked/streamed responses** for large files instead of buffering the whole file in memory
- **Basic session/user handling** for a demo login flow
- **Body size limiting** (413 responses) and proper handling of malformed requests
- Graceful handling of client disconnects (`SIGPIPE` ignored, sockets cleaned up on `EPOLLHUP`/`EPOLLERR`)

## Project structure

```
.
├── makefile
├── .server/
│   ├── .config/           # JSON server configuration files
│   ├── main/               # program entry point
│   ├── parse/               # CLI args + JSON config parsing
│   ├── lib/                  # core headers: server/route model, request, response,
│   │                          #   client state, CGI, users, time, console logging...
│   └── run/                   # the runtime: socket setup, epoll event loop,
│       ├── epoll_handle.cpp    #   read/write event handling
│       ├── required_checks_epoll.cpp
│       └── method/              # GET / POST / DELETE / CGI handlers
└── app/
    └── my-app/              # example web app served by the default config
```

## Configuration

The server is configured through a JSON file, e.g. `.server/.config/example.json`:

```json
{
  "servers": [
    {
      "port": "3000",
      "name": "my-app",
      "version": "0.1.0",
      "index": "index.html",
      "root": "./",
      "routes": [
        { "path": "/upload", "source": "upload.html" },
        { "path": "/delete", "source": "delete/deleteme.html", "method": ["GET", "DELETE"] },
        { "path": "/list", "dictlist": "true" },
        { "path": "/redirect1", "redirect": "/redirect2" },
        {
          "path": "/cgi",
          "cgi_script": "python.py",
          "cgi_interpreter": "/usr/bin/python3"
        }
      ]
    }
  ]
}
```

Each entry under `servers` maps to one listening socket. Each server can define multiple `routes`, controlling static files, directory listing, redirects, allowed methods, and CGI.

## Build & run

```bash
make                 # compiles the "webserver" binary
./webserver           # runs with the default config

./webserver --config-file=example     # use a specific config in .server/.config/
./webserver --config-rules             # print all available config options
./webserver --help                      # usage
```

Cleanup targets: `make clean` (objects), `make fclean` (objects + binary), `make re` (rebuild).

## How it works

1. **Startup** — for every server defined in the config, a `socket()` is opened, set non-blocking (`O_NONBLOCK`), bound with `SO_REUSEADDR`, and put into `listen()`. Every listening socket is registered with a single `epoll` instance.
2. **Event loop** — `epoll_wait()` blocks until any socket is ready. Depending on which fd fired:
   - a **listening socket** → `accept()` a new client and register it for `EPOLLIN`,
   - a **CGI pipe** → read the child process's output incrementally,
   - a **client socket** → hand off to the read/write handlers.
3. **Reading a request** — bytes are read incrementally into a per-client buffer until the request is fully parsed (headers + body complete, or a size limit is hit), then the appropriate method handler builds a response.
4. **Writing a response** — the socket is switched to `EPOLLOUT`; headers and body (or a streamed file, chunk by chunk) are written back without blocking the event loop, then the client is reset and switched back to `EPOLLIN` for keep-alive, or closed if the request asked for `Connection: close`.

This design lets one thread handle many simultaneous clients (plus CGI child processes) without blocking on any single slow connection.

## Team & contributions

This project was built by a team of two as part of the 1337/42 network curriculum:

- **[bnafiai]** — networking layer: socket creation/binding/listening, non-blocking I/O setup, and the `epoll`-based multiplexing/event loop (accepting connections, dispatching read/write events, CGI pipe handling).
- **abdael-m** — HTTP protocol layer: request/response parsing, method handlers (GET/POST/DELETE), CGI execution, configuration parsing, and the demo app.

## Notes

- Written in strict `C++98` (`-std=c++98 -Wall -Wextra -Werror`) as required by the project subject.
- No external HTTP frameworks — everything from the TCP handshake up is implemented manually.
- Built and tested on Linux (relies on `epoll`, which is Linux-specific).
