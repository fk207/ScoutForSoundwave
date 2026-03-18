FROM alpine

RUN apk add gcc make git musl-dev

WORKDIR /app

COPY . .

RUN make

CMD ["./elevate/src/elevate", "input8.txt", "--mode=dp"]
