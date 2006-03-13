#!/bin/sh -e

        export HOME=/tmp
        export PREFIX="%p"
        . ./environment-helper.sh
        export lt_cv_sys_max_cmd_len=65536

        export CC=gcc CXX=g++

        ./build-helper.sh cvs       %N %v %r make -f admin/Makefile.common cvs
        ./build-helper.sh configure %N %v %r ./configure %c $CONFIGURE_PARAMS
        perl -pi.bak -e 's, kgpg , ,' Makefile
        ./build-helper.sh make      %N %v %r make all all_libraries="$ALL_LIBRARIES"
