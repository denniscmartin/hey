FROM debian:latest

WORKDIR /app

COPY CMakeLists.txt .
COPY src src/

RUN apt-get update
RUN apt-get install cmake -y

RUN mkdir -p build
RUN cd build && cmake .. && make && chmod +x hey

CMD ["./build/hey"]

