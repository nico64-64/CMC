#! /bin/bash

gcc cmc.c -o cmc -lSDL2 -lSDL2_ttf -lSDL2_image -lm -Wall -Wextra -ggdb -fsanitize=address
./cmc