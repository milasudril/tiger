#!/bin/bash

gdb --args __targets/tiger --filter=example.cpp\
	--init=red:lenna-r.png,green:lenna-g.png,blue:lenna-b.png \
	--dest=red:__targets/lenna-r.png,green:__targets/lenna-g.png,blue:__targets/lenna-b.png \
	--iterations=1
