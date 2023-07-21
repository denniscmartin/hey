# Hey!

Hey! is a simple TCP streaming app.

## Usage

Make sure you have Docker installed.

```bash
./run.sh
```

Using docker compose, this script build the Docker images (server and client), create a user-defined bridge network, and run both containers. 
After that, you will be provided with the client shell to send messages to the server.

To close the connection type `exit` from the client shell and press `ENTER`.

Some things that I find interesting:
- Run `docker logs --follow hey-server-1` in another terminal. Doing that you can see the messages arrive at the server.
- If you have Wireshark installed you can use it to sniff the packets sent in the Docker network. This is good to understand TCP.

## What could you do next?

A good exercise will be to implement logic for some commands in the server. For example, you could program the server to send to the client the current
date every time the server receives the command `date`.

## References

This is the material I used to develop Hey!. The Beej's Guide to Network Programming is a nice introduction. Super easy to read:
- [What is a web server](https://developer.mozilla.org/en-US/docs/Learn/Common_questions/Web_mechanics/What_is_a_web_server)
- [HTTP Protocol](https://www.rfc-editor.org/rfc/pdfrfc/rfc7231.txt.pdf)
- [Socket programming Oracle](https://docs.oracle.com/cd/E19253-01/817-4415/6mjum5som/index.html)
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/html/split/index.html)
- [How does port number really work](https://stackoverflow.com/questions/13190176/how-does-port-number-really-work-in-tcp/29045432#29045432)
