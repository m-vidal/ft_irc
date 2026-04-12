# ft_irc

[![Ask DeepWiki](https://devin.ai/assets/askdeepwiki.png)](https://deepwiki.com/m-vidal/Ft_IRC)

An IRC (Internet Relay Chat) server implementation in C++98, compliant with RFC 2812. This project provides a robust, non-blocking server capable of handling multiple clients simultaneously using `poll()`. It supports user authentication, channel management, private messaging, and various channel modes.

## ✨ Features

- **Concurrent Client Handling**: Manages multiple simultaneous connections using `poll()`.
- **User Authentication**: Secure connection process with `PASS`, `NICK`, and `USER` commands.
- **Channel Management**: Create, join, part, and manage channels with commands like `JOIN`, `PART`, `TOPIC`, `KICK`, `INVITE`, `LIST`, and `NAMES`.
- **Messaging**: Supports public channel messages and private one-on-one communication via `PRIVMSG` and `NOTICE`.
- **Channel Modes**: Implements key channel modes for enhanced control:
  - `i`: Invite-only
  - `t`: Topic protection
  - `k`: Channel key (password)
  - `o`: Operator status management
  - `l`: User limit
  - `n`: No messages from outside users
- **IRC Numeric Replies**: Provides standard numeric replies and error codes for client-server communication.
- **Operator Privileges**: Channel creators are automatically granted operator status, with the ability to grant it to others.

## 🛠️ Tech Stack

- **Language**: C++98
- **Concurrency**: `poll()` system call for non-blocking I/O.
- **Networking**: Berkeley Sockets API.

## 🚀 Getting Started

### Prerequisites

- A C++ compiler (like `g++` or `clang++`)
- `make`

### Compilation

Clone the repository and compile the server using the provided `Makefile`:

```bash
git clone https://github.com/m-vidal/Ft_IRC.git
cd Ft_IRC
make
```

This will create an executable file named `ircserv`.

### Running the Server

Execute the server by providing a port number and a password as arguments. The port must be within the range of 6660-7000.

```bash
./ircserv <port> <password>
```

Example:

```bash
./ircserv 6667 mysecretpassword
```

The server will start and listen for incoming connections on the specified port.

### Connecting with an IRC Client

You can connect to the server using any standard IRC client (e.g., `irssi`, `HexChat`, `mIRC`).

**Connection Details:**
- **Server Address**: `127.0.0.1` or `localhost`
- **Port**: The port you specified when running `ircserv`.
- **Password**: The password you specified.

**Example using `irssi`:**

1.  Start `irssi`.
2.  Add the server to your network list:
    ```
    /server add -network ft_irc -port 6667 -password mysecretpassword localhost
    ```
3.  Connect to the server:
    ```
    /connect ft_irc
    ```

Once connected, you can set your nickname and begin chatting.

```
/nick your_nickname
/join #channel_name
```

## 📋 Commands Implemented

The server supports the following IRC commands:

| Command     | Description                                               |
|-------------|-----------------------------------------------------------|
| **PASS**    | Provide the server password for authentication.           |
| **NICK**    | Set or change your nickname.                              |
| **USER**    | Specify your username and real name.                      |
| **PING**    | Responds with a `PONG` to check if the connection is alive.|
| **QUIT**    | Disconnect from the server.                               |
| **JOIN**    | Join a channel. Creates one if it doesn't exist.          |
| **PART**    | Leave a channel.                                          |
| **PRIVMSG** | Send a private message to a user or a channel.            |
| **NOTICE**  | Send a notice to a user or a channel (no auto-reply).     |
| **KICK**    | Eject a user from a channel (operator only).              |
| **TOPIC**   | View or set the topic for a channel.                      |
| **INVITE**  | Invite a user to a channel.                               |
| **MODE**    | View or change channel/user modes.                        |
| **LIST**    | List all available channels on the server.                |
| **NAMES**   | List all users in a specific channel.                     |

## 📁 File Structure

```
.
├── Makefile          # Build instructions for the project.
├── inc/              # Header files for classes and definitions.
│   ├── Channel.hpp
│   ├── Server.hpp
│   ├── User.hpp
│   └── replyCodes.hpp
└── src/              # Source code implementation.
    ├── Channel.cpp
    ├── Server.cpp
    ├── User.cpp
    └── main.cpp