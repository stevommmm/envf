FROM alpine:latest

WORKDIR /usr/src/envf/
COPY Makefile /usr/src/envf/
COPY src/ /usr/src/envf/src/

RUN apk add build-base \
 && make \
 && make install
