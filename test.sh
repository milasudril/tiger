#!/bin/bash

gdb --args __targets/tiger-cli --filter=/home/torbjorr/Skrivbord/test.cpp --params-list --channels-list --objdir=__targets \
	--params=d_pred:1 \
	--init=pred:/home/torbjorr/Skrivbord/grey.png,prey:testdata/lenna-g.png \
	--dest=pred:__targets/gs-u.png,prey:__targets/gs-v.png \
	--iterations=1

#__targets/tiger-cli --filter=engine/example.cpp --params-list --channels-list \
#	--init=red:testdata/lenna-r.png,green:testdata/lenna-g.png,blue:testdata/lenna-b.png \
#	--dest=red:__targets/lenna-r.png,green:__targets/lenna-g.png,blue:__targets/lenna-b.png \
#	--iterations=1

#__targets/tiger-cli --filter=testdata/grayscott.cpp --params-list --channels-list --objdir=__targets \
#	--params=d:2,k:0.0610,F:0.0380\
#	--init=u:testdata/lenna-r.png,v:testdata/lenna-b.png --dest=u:__targets/gs-u.png,v:__targets/gs-v.png\
#	--iterations=10000
