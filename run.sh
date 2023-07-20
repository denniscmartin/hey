#!/bin/bash

docker compose build
docker compose up -d 

#!/bin/bash

rectangle="
+----------------------+
|                      |
|   CLIENT CONSOLE     |
|                      |
+----------------------+"

echo "$rectangle"
echo "To view server logs run 'docker logs --follow hey-server-1' in other terminal"
echo ""

docker exec -it hey-client-1 ./build/hey_client
