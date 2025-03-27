# ft_irc — IRC Server in C++

`ft_irc` is a custom implementation of an **IRC (Internet Relay Chat) server** written in C++.  
It supports multiple clients, channel management, message broadcasting, user authentication, and follows the RFC 2812 protocol standard as closely as possible.

This project was developed as part of the 42 School curriculum and demonstrates deep understanding of **network programming**, **socket management**, and **multi-client handling**.

---

## Objectives

- Build a fully functioning **IRC server**
- Handle multiple concurrent users via non-blocking sockets / `poll()`
- Implement a subset of the **IRC protocol (RFC 2812)**:
  - Nicknames, users, messages, channels, and more
- Maintain stability and performance with many users
- Structure clean, object-oriented server logic in **C++**

---

## Supported IRC Commands

| Command   | Description                            |
|-----------|----------------------------------------|
| `PASS`    | Client authentication                  |
| `NICK`    | Set/change nickname                    |
| `USER`    | Define user details                    |
| `JOIN`    | Join a channel                         |
| `PART`    | Leave a channel                        |
| `PRIVMSG` | Send private message to user/channel   |
| `NOTICE`  | Similar to PRIVMSG, no auto-reply      |
| `PING`    | Keep connection alive (with `PONG`)    |
| `QUIT`    | Disconnect from server                 |
| `MODE`    | (basic channel/user mode support)      |
| `TOPIC`   | Set a channel topic                    |
| `KICK`    | Kick a user from a channel             |
