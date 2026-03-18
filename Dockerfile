FROM alpine

RUN apk add gcc make git linux-headers musl-dev

RUN git clone https://github.com/fk207/hw1-fk207&& cd ./soundwave/src  && make CFLAGS='-Wno-error -Wno-implicit-function-declaration -Wno-int-conversion'
