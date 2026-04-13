*This project has been created as part of the 42 curriculum by atambo, m-vidal, marcsilv.*

# ft_irc

## Description

This project is a fully functional IRC (Internet Relay Chat) server written in C++98, compliant with [RFC 2812](https://datatracker.ietf.org/doc/html/rfc2812). The goal of this project is to implement a real network server from scratch, gaining a deep understanding of socket programming, non-blocking I/O, and the IRC protocol.

The server is capable of handling multiple simultaneous client connections using the `poll()` system call. It supports user authentication, channel creation and management, private messaging, operator privileges, and a full set of channel modes — all in line with the standard IRC specification.

### Key Features

- **Concurrent Client Handling**: Manages multiple simultaneous connections using `poll()` for non-blocking I/O.
- **User Authentication**: Secure connection handshake with `PASS`, `NICK`, and `USER` commands.
- **Channel Management**: Create, join, leave, and manage channels with `JOIN`, `PART`, `TOPIC`, `KICK`, `INVITE`, `LIST`, and `NAMES`.
- **Messaging**: Public channel messages and private one-on-one communication via `PRIVMSG` and `NOTICE`.
- **Channel Modes**: Implements the following modes:
  - `i` - Invite-only
  - `t` -Topic definition 
  - `k` - Channel key (password)
  - `o` - Operator status grant/revoke
  - `l` - User limit
- **IRC Numeric Replies**: Standard numeric replies and error codes for proper client-server communication.
- **Operator Privileges**: Channel creators are automatically granted operator status and may promote others.
### Prerequisites

- A C++98 compatible compiler (`clang++` or `g++`)
- `make` build utility
- An IRC client for testing (recommended: `irssi`)

### Compilation & Usage

```bash
# Clone the repository
git clone <repository-url> ft_irc
cd ft_irc

# Compile the server
make

# Run the server with default port and password
./ircserv <port> <password>

# Example:
./ircserv 6667 mysecretpassword
```
### Commands Implemented

| Command     | Description                                                |
|-------------|------------------------------------------------------------|
| `PASS`      | Provide the server password for authentication.            |
| `NICK`      | Set or change your nickname.                               |
| `USER`      | Specify your username and real name.                       |
| `PING`      | Server responds with `PONG` to verify the connection.      |
| `QUIT`      | Disconnect from the server.                                |
| `JOIN`      | Join a channel; creates it if it does not exist.           |
| `PART`      | Leave a channel.                                           |
| `PRIVMSG`   | Send a private message to a user or channel.               |
| `NOTICE`    | Send a notice to a user or channel (no automatic reply).   |
| `KICK`      | Eject a user from a channel (operator only).               |
| `TOPIC`     | View or set the topic of a channel.                        |
| `INVITE`    | Invite a user to a channel.                                |
| `MODE`      | View or change channel/user modes.                         |
| `LIST`      | List all available channels on the server.                 |
| `NAMES`     | List all users in a specific channel.                      |

## 📚 Resources

- [RFC 2812 — Internet Relay Chat: Client Protocol](https://datatracker.ietf.org/doc/html/rfc2812) — The primary specification this server is built against.
- [RFC 1459 — Internet Relay Chat Protocol](https://datatracker.ietf.org/doc/html/rfc1459) — The original IRC RFC, useful as a companion reference.
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/) — Comprehensive guide to socket programming in C/C++.
- [poll() man page (Linux)](https://man7.org/linux/man-pages/man2/poll.2.html) — Reference for the `poll()` system call used for non-blocking I/O.
- [Modern IRC documentation (ircdocs.horse)](https://modern.ircdocs.horse/) — Up-to-date, readable documentation of the IRC protocol with practical examples.
