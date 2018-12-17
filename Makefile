gammonfor2: gammonfor2.c gammonboard.c gammonmove.c gammonchecks.c display.c
	gcc -Wall gammonfor2.c gammonboard.c gammonmove.c gammonchecks.c display.c -g -o gammonfor2


test:  gammonboard.c gammonmove.c gammonchecks.c display.c test.c MinMax.c boardeval.c
	gcc -Wall  gammonboard.c gammonmove.c gammonchecks.c display.c test.c MinMax.c boardeval.c -g -o test
