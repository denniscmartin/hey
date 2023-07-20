FROM debian:latest

WORKDIR /app

COPY CMakeLists.txt .
COPY main.c .

RUN apt-get update
RUN apt-get install cmake -y

RUN mkdir -p build
RUN cd build && cmake .. && make && chmod +x hey_server

CMD ["./build/hey_server"]

