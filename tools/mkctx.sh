#!/bin/bash

for file in $(find src/toy -type f -iname '*.c*' -exec basename {} \; | sort) ; do
    mod=${file/.*/}
    echo "extern void ${mod}_init();"
done > $1/ctx.c
echo "void ctx_init(){" >> $1/ctx.c
for file in $(find src/toy -type f -iname '*.c*' -exec basename {} \; | sort) ; do
    mod=${file/.*/}
    echo "    ${mod}_init();"
done >> $1/ctx.c
echo "}" >> $1/ctx.c
