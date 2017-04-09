#!/bin/bash

gdb --args __targets/tiger --filter=__targets/test.so \
	--init=red:lenna-r.png,green:lenna-g.png,blue:lenna-b.png \
	--dest=red:lenna-r1.png,green:lenna-g1.png,blue:lenna-b1.png \
	--iterations=1
