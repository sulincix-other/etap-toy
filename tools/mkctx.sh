#!/bin/bash

for file in $(ls src/toy/) ; do
    mod=${file/.*/}
    echo "extern void ${mod}_init();"
done > $1/ctx.c
echo "void ctx_init(){" >> $1/ctx.c
for file in $(ls src/toy/) ; do
    mod=${file/.*/}
    echo "    ${mod}_init();"
done >> $1/ctx.c
echo "}" >> $1/ctx.c
