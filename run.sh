#!/bin/bash

sudo docker compose build
sudo docker compose up -d 

rectangle="
+----------------------+
|                      |
|   CLIENT CONSOLE     |
|                      |
+----------------------+"

echo "$rectangle"
echo "To view server logs run 'docker logs --follow hey-server-1' in other terminal"
echo ""

sudo docker exec -it hey-client-1 ./build/hey_client
