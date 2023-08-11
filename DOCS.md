
# Table of Contents

1.  [Usage](#org6a0ed95)
2.  [What could you do next?](#orgb192b61)


<a id="org6a0ed95"></a>

# Usage

Make sure you have Docker installed

    ./run.sh

Using docker compose, this script build the Docker images (server and client), create a user-defined bridge network, and run both containers. After that, you will be provided with the client shell to send messages to the server.

To close the connection type `exit` from the client shell and press `ENTER`.

Some things that I find interested:

-   Run `docker logs --follow hey-server-1` in another terminal. Doing that you can see the messages arriving at the server.
-   If you have Wireshark installed you can use it to sniff the packets sent in the Docker network. This is good to understand TCP.


<a id="orgb192b61"></a>

# What could you do next?

A good exercise will be to implement logic for some commands in the server. For example, you could program the server to send to the client the current date every time the server receives the command `date`.

