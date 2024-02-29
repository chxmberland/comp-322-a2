#!/bin/bash

echo "Compiling and excecuting $1 assuming a g++ compiler is present."

g++ $1 -o out

./out