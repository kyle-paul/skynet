FROM gcc:latest

RUN apt-get update && apt-get install --no-install-recommends -y ninja-build cmake

