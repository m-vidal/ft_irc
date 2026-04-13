# ft_irc

*This project has been created as part of the 42 curriculum by atambo, marcsilv, mvidal.*

## 📝 Description

**ft_irc** is a custom IRC (Internet Relay Chat) server implementation written in **C++98**.  
The goal of this project is to recreate a functional IRC server compliant with the **RFC 2812** protocol.

The server handles multiple simultaneous client connections using non-blocking I/O through the `poll()` system call. It manages networking logic, user authentication, channel management, operator privileges, and real-time messaging.

### Key Features

- **Non-blocking Concurrency**: Efficiently manages multiple clients using a single `poll()` loop.
- **Full Authentication**: Handles `PASS`, `NICK`, and `USER` command sequences.
- **Channel Operations**: Full support for `JOIN`, `PART`, `KICK`, `INVITE`, `TOPIC`, and `MODE`.
- **Communication**: Private messaging (`PRIVMSG`) and server notices (`NOTICE`).
- **Channel Modes**: Support for `i` (invite-only), `t` (topic protection), `k` (key), `o` (operator), `l` (limit), and `n` (no external messages).

## 🚀 Instructions

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
## 📚 Resources

- [Modern IRC Documentation](https://modern.ircdocs.horse/) — Excellent and up-to-date reference of the IRC protocol as implemented in practice.
- [RFC 1459 - IRC Protocol](https://datatracker.ietf.org/doc/html/rfc1459) — Original IRC protocol specification.
- [RFC 2812 - IRC Client Protocol](https://datatracker.ietf.org/doc/html/rfc2812) — Protocol for IRC clients.