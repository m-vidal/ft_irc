/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marcsilv@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 17:55:06 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/14 08:06:17 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <string>

int main(int ac, char **av) {
	if (ac != 3)
		std::cerr << "The executable runs as follows:\n\t./ircserv <port> <password>" << "\n";
	(void)av; 

/*

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket failed"); return 1; }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(6667);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind failed");
        return 1;
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen failed");
        return 1;
    }

    std::vector<pollfd> fds;
    fds.push_back({server_fd, POLLIN, 0});

    std::cout << "IRC Server listening on 6667. Connect with HexChat!\n";

    while (true) {
        int poll_count = poll(fds.data(), fds.size(), -1);
        if (poll_count < 0) { perror("poll failed"); break; }

        for (size_t i = 0; i < fds.size(); i++) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == server_fd) {
                    int new_client = accept(server_fd, NULL, NULL);
                    if (new_client != -1) {
                        fds.push_back({new_client, POLLIN, 0});
                        std::cout << "New client connected on FD " << new_client << "\n";
                    }
                } else {
                    char buf[1024];
                    memset(buf, 0, 1024);
                    int bytes = recv(fds[i].fd, buf, sizeof(buf) - 1, 0);

                    if (bytes <= 0) {
                        std::cout << "FD " << fds[i].fd << " disconnected.\n";
                        close(fds[i].fd);
                        fds.erase(fds.begin() + i);
                        i--; 
                    } else {
                        std::string raw(buf);
                        std::cout << "Raw from client: " << raw;

                        // 1. Respond to PING immediately
                        if (raw.find("PING") != std::string::npos) {
                            std::string pong = ":localhost PONG localhost :localhost\r\n";
                            send(fds[i].fd, pong.c_str(), pong.length(), 0);
                        }

                        // 2. Acknowledge NICK/USER to unlock HexChat
                        // We look for "USER" because it's usually the last part of the handshake
                        if (raw.find("USER") != std::string::npos) {
                            std::string welcome = ":localhost 001 guest :Welcome to the C++ IRC Server!\r\n";
                            send(fds[i].fd, welcome.c_str(), welcome.length(), 0);
                            
                            // Send MOTD (Message of the Day) to make HexChat look 'ready'
                            std::string motd = ":localhost 372 guest :--- You are now connected! ---\r\n";
                            std::string end = ":localhost 376 guest :End of /MOTD command.\r\n";
                            send(fds[i].fd, motd.c_str(), motd.length(), 0);
                            send(fds[i].fd, end.c_str(), end.length(), 0);
                        }
                    }
                }
            }
        }
    }
    close(server_fd);
    return 0; */
}

