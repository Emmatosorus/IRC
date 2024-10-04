---
id: ft_irc
aliases: []
tags: []
---

# ft_irc
## Allowed functions
### Core networking functions and structs for servers
- `struct addrinfo` is a linked list that contains information about the host or service, for uses with sockets.
```c
struct addrinfo {
    int              ai_flags;     // AI_PASSIVE, AI_CANONNAME, etc.
    int              ai_family;    // AF_INET, AF_INET6, AF_UNSPEC
    int              ai_socktype;  // SOCK_STREAM, SOCK_DGRAM
    int              ai_protocol;  // use 0 for "any"
    size_t           ai_addrlen;   // size of ai_addr in bytes
    struct sockaddr *ai_addr;      // struct sockaddr_in or _in6
    char            *ai_canonname; // full canonical hostname
    struct addrinfo *ai_next;      // linked list, next node
};

// can be used in place of the sockaddr, more convenient and modern
// should be casted to `struct sockaddr`
struct sockaddr_in {
    short int          sin_family;  // Address family, AF_INET
    unsigned short int sin_port;    // Port number
    struct in_addr     sin_addr;    // Internet address
    unsigned char      sin_zero[8]; // Same size as struct sockaddr
};

```

The following funcions are *server functions*.
They are usde in order:
- `getaddrinfo()`
- `socket()`
- `bind()`
- `listen()`
- `accept()`
- `recv()`/`sendv()`
- `close()`

#### `getaddrinf()`
- `getaddrinfo()` initializes and returns a linked list of `addrinfo`'s.
    - This function allows us to avoid using older `gethostbyname()` and `getservbyname()`.
    - Accepted params:
        - `const char* node` host name to connect to, or an IP address (like `www.example.com`).
        - `const char* service` port number or a service name from `/etc/services` (`"80"` or `"printer"`).
        - `const struct addrinfo* hints` `addrinfo` struct with some of the information filled.
        - `struct addrinfo** res` pointer to a pointer, which will be overriden with a pointer to a newly allocated `addrinfo`.
    - Returns an int, `0` for OK, special error code for fail.
        - On fail returns defined error codes like `EAI_FAIL` or `EAI_MEMORY`.
    - Example use: 
    ```c
    struct addrinfo  hints;
    struct addrinfo* res;
    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me
    // NULL +  AI_PASSIVE means that it listen on the host's IP address
    if (getaddrinfo(NULL, "3490", &hints, &res) != 0) {
        // ... handle getaddrinfo error
    }
    ```

#### `freeaddrinfo()`
- `freeaddrinfo()` frees the `addrinfo` linked list.

#### `socket()`
- `socket()` returns a file descriptor of a new socket.
    - Accepted params:
        - `int domain` version of the Internet Protocol: value like `PF_INET` or `PF_INET6` for IPv4 or IPv6 respectively.
            - `AF_INET` and `AF_INET6` can be used. They have the same value.
        - `int type` type of socket: value like `SOCK_STREAM` or `SOCK_DGRAM` for stream socket or datagram socket respectively.
            - There are more socket types than those two.
        - `int protocol` used protocol: an integer that represents the protocol number.
            - `0` chooses the protocol based on `type`. For example, `tcp` for `SOCK_STREAM`, `udp` for `SOCK_DGRAM`.
            - Otherwise, `getprotobyname()` can be used, more specifically `p_proto` field of the returned structure.
    - Returns a file descriptor of a socket, or `-1` on error.
        - This function sets `errno` on error as well.
    - Example use:
    ```c
    // assuming we have addrinfo struct `res`
    int s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (s == -1) {
        // ... handle socket error
    }
    ```

#### `bind()`
- `bind()` binds socket to the specific IP and port.
    - Accepts params:
        - `int sockfd` file descriptor of a socket, to which we bind IP and port.
        - `struct sockaddr* addr` a field that is located on the `addrinfo` struct.
        - `int addrlen` a field that is located on the `addrinfo` struct.
    - Returns `0` on success, or `-1` on error.
        - This function sets `errno` on error as well.
    - Example use:
    ```c
    // assuming we have socked file descriptor `scokfd` and addrinfo struct `res`
    if (bind(sockfd, res->ai_addr, res->ai_addrlen) != 0) {
        // ... handle bind error
    }
    ```

#### `listen()`
- `listen()` tells a socket to listen for incoming connections.
    - It opens up a socket for the connections.
    - It is necessary to `bind()` socket to a port before calling this function. All clients will have to connect to this port.
    - Accepts params:
        - `int sockfd` file descriptor of a socket that is going to listen to connections.
        - `int backlog` the number of connections allowed on the incoming queue.
            - All connections have to wait before they get `accept()`ed.
    - Returns `0` on success, or `-1` on error.
        - This function sets `errno` on error as well.
    - Example use:
    ```c
    // assuming we have socked file descriptor `scokfd`
    if (listen(sockfd, 10) != 0) {
        // ... handle listen error
    }
    ```

#### `accept()`
- `accept()` accepts incoming connection to a listening socket.
    - `listen()` is useless without `accept()`. `listen()` opens up the socket for connections, but they need to be `accept()`ed.
    - **IMPORTANT**: It is a blocking call by default.
    - Accepts params:
        - `int sockfd` file descriptor of a `listen()`ing socket.
        - `struct sockaddr* addr` a field that is located on the `addrinfo` struct.
        - `socklen_t* addrlen` pointer to a `sizeof(struct sockaddr)`.
    - Returns a new file descriptor on success, or `-1` on error.
        - A new file descriptor is used then for using `send()` and `recv()` on it.
        - This function sets `errno` on error as well.

#### `send()`, `recv()`
- `send()` sends data over an `accept()`ed socket.
- `recv()` receives data over an `accept()`ed socket.
    - Used for a TCP `SOCK_STREAM` connected sockets.
    - They both accept params:
        - `int sockfd` file descriptor of a socket for sending data to.
        - `const void* buf` pointer to the data we want to send.
        - `size_t len` number of bytes of the sent data.
        - `int flags` allows to specify information about the data.
            - `0` for no flags.
    - They both return the number of sent bytes on success, or `-1` on error.
        - This function sets `errno` on error as well.
        - `recv()` also can return `0` if the other side closed connection.

#### `close()`
- `close()` ol' reliable, t his time for closing the connection between sockets.

### Important networking functions
#### `setsockopt()`
- `setsockopt()` set options for a socket.
    - One important option to set is `SO_REUSEADDR` to avoid random errors related to cleaning up of sockets.
    - Accepts params:
        - `int s` file descriptor of a socket that we want to configure.
        - `int level` level of configuration, typically `SOL_SOCKET`.
        - `int optname` number of the option to set, like `SO_REUSEADDR` or `SO_BINDTODEVICE`.
        - `const void* optval` can be `NULL`, a value for the `optname`.
        - `socklen_t optlen` size of the `optval`.
    - Returns `0` on success, or `-1` on error.
        - This function sets `errno` on error as well.
    - Example use:
    ```c
    int yes=1;

    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
        perror("setsockopt");
        exit(1);
    }
    ```

#### `connect()`
- `connect()` connects socket to a server.
    - The opposite of `listen()`, `accept()`. It is a function for a client.
    - if `bind()` was not called, it binds to your IP and a random port automatically.
    - Accepted params:
        - `int sockfd` file descriptor of a socket.
        - `const struct sockaddr* serv_addr` a field that is located on the `addrinfo` struct.
        - `socklen_t addrlen`  a field that is located on the `addrinfo` struct.
    - Returns `0` on success, or `-1` on error.
        - This function sets `errno` on error as well.
    - Example use:
    ```c
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) != 0) {
        // ... handle connect error
    }
    ```

### Utility networking functions
#### `getprotobyname()`
- `getprotobyname()` accepts `char*`, which is a name of the protocol: `tcp`, `udp` etc, and returns a number of this protocol.

#### `htonl()`, `ntohl()`, `htons()`, `ntohs()`
- `htonl()`, `ntohl()` are used for the conversion of bit order of numbers of type `long`.
- `htons()`, `ntohs()` are used for the conversion of bit order of numbers of type `short`.
    - We should always assume that the bit order is incorrect and use those functions.
    - `htonl()`, `htons()` host bit order to network bit order.
        - Always used on the data that we send.
    - `ntohl()`, `ntohs()` network bit order to host bit order.
        - Always used on the data that we receive.

#### `inet_ntoa()`, `inet_addr()`
- `inet_ntoa()`
- `inet_addr()`

#### `fcntl()`
- `fcntl()`
 
### Useless functions
- `gethostbyname()`, `gethostbyaddr()`
    - Deprecated. Should use `getaddrinfo()` instead.
