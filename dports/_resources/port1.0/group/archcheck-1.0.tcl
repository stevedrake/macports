# $Id$
# 
# Copyright (c) 2009 The MacPorts Project
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
# 
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. Neither the name of The MacPorts Project nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 
# 
# This PortGroup checks that the architecture(s) of the given files match
# the architecture(s) we are trying to install this port as now. This is
# a crutch to get us by until a proper solution is implemented in base.
# See #20728.
# 
# Usage:
# 
#   PortGroup               archcheck 1.0
#   archcheck.files         file1 file2 ...
#
# Files can (and probably usually should) be specified relative to ${prefix}.

options archcheck.files
default archcheck.files {}

pre-extract {
    if {[variant_exists universal] && [variant_isset universal]} {
        set requested_archs ${configure.universal_archs}
    } else {
        set requested_archs ${configure.build_arch}
    }
    foreach file ${archcheck.files} {
        # Prepend prefix if necessary.
        if {"/" != [string index ${file} 0]} {
            set file [file join ${prefix} ${file}]
        }
        set file_archs [string trim [strsed [exec lipo -info ${file}] {s/.*://}]]
        foreach requested_arch ${requested_archs} {
            if {9 <= ${os.major} && ${requested_arch} == "ppc"} {
                set requested_arch ppc7400
            }
            if {-1 == [string first " ${requested_arch} " " ${file_archs} "]} {
                ui_error "You cannot install ${name} for the architecture(s) ${requested_archs}"
                ui_error "because ${file} only contains the architecture(s) ${file_archs}."
                if {[variant_exists universal] && [variant_isset universal] && 1 == [llength ${file_archs}]} {
                    ui_error "Try reinstalling the port that provides ${file} with the +universal variant."
                }
                return -code error "incompatible architectures in dependencies"
            }
        }
    }
}
