FROM alpine

RUN apk add gcc make git musl-dev

WORKDIR /

RUN make

CMD ["./elevate","input8.txt", "--mode=dp" ]
