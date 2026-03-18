FROM alpine

RUN apk add gcc make git musl-dev

WORKDIR /app

RUN make CFLAGS='-Wno-error -Wno-implicit-function-declaration'

CMD ["./elevate","input8.txt", "--mode=dp" ]
