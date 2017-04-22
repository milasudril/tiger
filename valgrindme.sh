#!/bin/bash

array=(valgrind/*.supp)
VALGRIND_SUPPRESSIONS=${array[@]/#/--suppressions=}
echo $VALGRIND_SUPPRESSIONS
G_SLICE=always-malloc G_DEBUG=gc-friendly valgrind \
	$VALGRIND_SUPPRESSIONS __targets/tiger-ui
