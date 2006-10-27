#!/bin/sh -e

        export PREFIX="%p" USE_UNSERMAKE=1
        . ./environment-helper.sh

        case $SW_VERSION in 
                6*|7*)
                        export CPPFLAGS="$CPPFLAGS -DHAVE_SYS_EXEC_H=1"
                        ;; 
        esac

        export CPPFLAGS="-I/usr/include/gssapi $CPPFLAGS"

        ./build-helper.sh cvs       %N %v %r make -f admin/Makefile.common cvs
        ./build-helper.sh configure %N %v %r ./configure %c $CONFIGURE_PARAMS
        ./build-helper.sh make      %N %v %r unsermake $UNSERMAKEFLAGS
#apidox#./build-helper.sh apidox    %N %v %r make apidox all_libraries="$ALL_LIBRARIES"
