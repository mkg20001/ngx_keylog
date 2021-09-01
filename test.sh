#!/bin/sh

while sleep 1s; do
  curl -k https://localhost:3443 -v
  curl -k https://localhost:3543 -v
done
