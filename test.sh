#!/bin/bash

__targets/tiger --filter=example.cpp --params-list --channels-list \
	--init=red:lenna-r.png,green:lenna-g.png,blue:lenna-b.png \
	--dest=red:__targets/lenna-r.png,green:__targets/lenna-g.png,blue:__targets/lenna-b.png \
	--iterations=1

__targets/tiger --objdir=__targets --params-list --channels-list --filter=example.cpp\
	--init=red:lenna-r.png,green:lenna-g.png,blue:lenna-b.png \
	--dest=red:__targets/lenna-r.png,green:__targets/lenna-g.png,blue:__targets/lenna-b.png \
	--iterations=1

__targets/tiger --filter=grayscott.cpp --params-list --channels-list --objdir=__targets \
	--params=d:2,k:0.0610,F:0.0380\
	--init=u:lenna-r.png,v:lenna-b.png --dest=u:__targets/gs-u.png,v:__targets/gs-v.png\
	--iterations=100000
