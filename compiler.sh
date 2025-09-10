#! /bin/bash

gcc cmc.c -o cmc -lSDL2 -lSDL2_ttf -lSDL2_image -Wall -Wextra -fsanitize=address -ggdb
./cmc