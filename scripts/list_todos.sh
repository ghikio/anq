#!/usr/bin/env bash

prj_root_dir=..
src_root_dir=${prj_root_dir}/src

files=(`find ${src_root_dir} -type f | grep -E ".\.(c|h)$"`)

for i in "${files[@]}"; do
	maintainer= cat $i | grep "TODO" | grep -Eo "\(\w+\)"
	cat $i | grep "TODO" | grep -Eo "\(\w+\)\(.+\)"
done
