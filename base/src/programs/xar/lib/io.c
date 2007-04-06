/*
 * Copyright (c) 2005 Rob Braun
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Rob Braun nor the names of his contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * 03-Apr-2005
 * DRI: Rob Braun <bbraun@opendarwin.org>
 */

#define _FILE_OFFSET_BITS 64

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/types.h>
#include <assert.h>

#include "config.h"
#ifndef HAVE_ASPRINTF
#include "asprintf.h"
#endif
#include "xar.h"
#include "filetree.h"
#include "archive.h"
#include "io.h"
#include "zxar.h"
#include "bzxar.h"
#include "md5.h"
#include "script.h"
#include "macho.h"

#if !defined(LLONG_MAX) && defined(LONG_LONG_MAX)
#define LLONG_MAX LONG_LONG_MAX
#endif

#if !defined(LLONG_MIN) && defined(LONG_LONG_MIN)
#define LLONG_MIN LONG_LONG_MIN
#endif

struct datamod xar_datamods[] = {
	{ (fromheap_in)NULL,
	  xar_md5_compressed,
	  xar_md5out_done,
	  xar_md5_uncompressed,
	  xar_md5_compressed,
	  xar_md5_done
	},
	{ (fromheap_in)NULL,
	  (fromheap_out)NULL,
	  (fromheap_done)NULL,
	  xar_script_in,
	  (toheap_out)NULL,
	  xar_script_done
	},
	{ (fromheap_in)NULL,
	  (fromheap_out)NULL,
	  (fromheap_done)NULL,
	  xar_macho_in,
	  (toheap_out)NULL,
	  xar_macho_done
	},
	{ xar_gzip_fromheap_in,
	  (fromheap_out)NULL,
	  xar_gzip_fromheap_done,
	  xar_gzip_toheap_in,
	  (toheap_out)NULL,
	  xar_gzip_toheap_done
	},
	{ xar_bzip_fromheap_in,
	  (fromheap_out)NULL,
	  xar_bzip_fromheap_done,
	  xar_bzip_toheap_in,
	  (toheap_out)NULL,
	  xar_bzip_toheap_done
	}
};

int32_t xar_attrcopy_to_heap(xar_t x, xar_file_t f, const char *attr, read_callback rcb) {
	int r, off, i;
	size_t bsize, rsize;
	int64_t readsize=0, writesize=0, inc = 0;
	void *inbuf;
	char *tmpstr = NULL, *tmpstr2 = NULL;
	const char *opt, *csum;
	off_t orig_heap_offset = XAR(x)->heap_offset;
	xar_file_t tmpf = NULL;

	opt = xar_opt_get(x, XAR_OPT_RSIZE);
	if( !opt ) {
		bsize = 4096;
	} else {
		bsize = strtol(opt, NULL, 0);
		if( ((bsize == LONG_MAX) || (bsize == LONG_MIN)) && (errno == ERANGE) ) {
			bsize = 4096;
		}
	}

	r = 1;
	while(r != 0) {
		inbuf = malloc(bsize);
		if( !inbuf )
			return -1;

		r = rcb(x, f, inbuf, bsize);
		if( r < 0 ) {
			free(inbuf);
			return -1;
		}

		readsize+=r;
		inc += r;
		rsize = r;

		/* filter the data through the in modules */
		for( i = 0; i < (sizeof(xar_datamods)/sizeof(struct datamod)); i++) {
			if( xar_datamods[i].th_in ) {
				xar_datamods[i].th_in(x, f, attr, &inbuf, &rsize);
			}
		}

		/* filter the data through the out modules */
		for( i = 0; i < (sizeof(xar_datamods)/sizeof(struct datamod)); i++) {
			if( xar_datamods[i].th_out )
				xar_datamods[i].th_out(x, f, attr, inbuf, rsize);
		}

		off = 0;
		if( rsize != 0 ) {
			do {
				r = write(XAR(x)->heap_fd, inbuf+off, rsize-off);
				if( (r < 0) && (errno != EINTR) )
					return -1;
				off += r;
				writesize += r;
			} while( off < rsize );
		}
		XAR(x)->heap_offset += off;
		free(inbuf);

	}


	/* If size is 0, don't bother having anything in the heap */
	if( readsize == 0 ) {
		XAR(x)->heap_offset = orig_heap_offset;
		lseek(XAR(x)->heap_fd, -writesize, SEEK_CUR);
		for( i = 0; i < (sizeof(xar_datamods)/sizeof(struct datamod)); i++) {
			if( xar_datamods[i].th_done )
				xar_datamods[i].th_done(x, NULL, attr);
		}
		return 0;
	}
	/* finish up anything that still needs doing */
	for( i = 0; i < (sizeof(xar_datamods)/sizeof(struct datamod)); i++) {
		if( xar_datamods[i].th_done )
			xar_datamods[i].th_done(x, f, attr);
	}

	XAR(x)->heap_len += writesize;
	asprintf(&tmpstr, "%s/archived-checksum", attr);
	xar_prop_get(f, tmpstr, &csum);
	free(tmpstr);
	tmpf = xmlHashLookup(XAR(x)->csum_hash, BAD_CAST(csum));
	if( tmpf ) {
		opt = xar_opt_get(x, XAR_OPT_LINKSAME);
		if( opt && (strcmp(attr, "data") == 0) ) {
			const char *id = xar_attr_get(tmpf, NULL, "id");
			xar_prop_set(f, "type", "hardlink");
			xar_attr_set(f, "type", "link", id);
			xar_prop_set(tmpf, "type", "hardlink");
			xar_attr_set(tmpf, "type", "link", "original");
			xar_prop_unset(f, "data");
			XAR(x)->heap_offset = orig_heap_offset;
			lseek(XAR(x)->heap_fd, -writesize, SEEK_CUR);
			XAR(x)->heap_len -= writesize;
			return 0;
		} 
		opt = xar_opt_get(x, XAR_OPT_COALESCE);
		if( opt ) {
			long long tmpoff;
			const char *offstr;
			asprintf(&tmpstr2, "%s/offset", attr);
			xar_prop_get(tmpf, tmpstr2, &offstr);
			if( tmpstr ) {
				tmpoff = strtoll(offstr, NULL, 10);
				XAR(x)->heap_offset = orig_heap_offset;
				lseek(XAR(x)->heap_fd, -writesize, SEEK_CUR);
				orig_heap_offset = tmpoff;
				XAR(x)->heap_len -= writesize;
			}
			
		}
	} else {
		xmlHashAddEntry(XAR(x)->csum_hash, BAD_CAST(csum), XAR_FILE(f));
	}

	asprintf(&tmpstr2, "%s/size", attr);
	asprintf(&tmpstr, "%"PRIu64, readsize);
	xar_prop_set(f, tmpstr2, tmpstr);
	free(tmpstr);
	free(tmpstr2);

	asprintf(&tmpstr, "%"PRIu64, (uint64_t)orig_heap_offset);
	asprintf(&tmpstr2, "%s/offset", attr);
	xar_prop_set(f, tmpstr2, tmpstr);
	free(tmpstr);
	free(tmpstr2);
	
	tmpstr = (char *)xar_opt_get(x, XAR_OPT_COMPRESSION);
	if( tmpstr && (strcmp(tmpstr, XAR_OPT_VAL_NONE) == 0) ) {
		asprintf(&tmpstr2, "%s/encoding", attr);
		xar_prop_set(f, tmpstr2, NULL);
		xar_attr_set(f, tmpstr2, "style", "application/octet-stream");
		free(tmpstr2);
	}

	asprintf(&tmpstr2, "%s/length", attr);
	asprintf(&tmpstr, "%"PRIu64, writesize);
	xar_prop_set(f, tmpstr2, tmpstr);
	free(tmpstr);
	free(tmpstr2);

	return 0;
}

/* xar_copy_from_heap
 * This is the arcmod extraction entry point for extracting the file's
 * data from the heap file.
 * It is assumed the heap_fd is already positioned appropriately.
 */
int32_t xar_attrcopy_from_heap(xar_t x, xar_file_t f, const char *attr, write_callback wcb) {
	int r, i;
	size_t bsize, def_bsize;
	int64_t fsize, inc = 0, seekoff;
	void *inbuf;
	const char *opt;
	char *tmpstr = NULL;

	opt = xar_opt_get(x, "rsize");
	if( !opt ) {
		def_bsize = 4096;
	} else {
		def_bsize = strtol(opt, NULL, 0);
		if( ((def_bsize == LONG_MAX) || (def_bsize == LONG_MIN)) && (errno == ERANGE) ) {
			def_bsize = 4096;
		}
	}

	asprintf(&tmpstr, "%s/offset", attr);
	xar_prop_get(f, tmpstr, &opt);
	free(tmpstr);
	if( !opt ) {
		wcb(x, f, NULL, 0);
		return 0;
	} else {
		seekoff = strtoll(opt, NULL, 0);
		if( ((seekoff == LLONG_MAX) || (seekoff == LLONG_MIN)) && (errno == ERANGE) ) {
			return -1;
		}
	}

	seekoff += XAR(x)->toc_count + sizeof(xar_header_t);
	if( XAR(x)->fd > 1 ) {
		r = lseek(XAR(x)->fd, seekoff, SEEK_SET);
		if( r == -1 ) {
			if( errno == ESPIPE ) {
				ssize_t rr;
				char *buf;
				unsigned int len;

				len = seekoff - XAR(x)->toc_count;
				len -= sizeof(xar_header_t);
				if( XAR(x)->heap_offset > len ) {
					xar_err_new(x);
					xar_err_set_file(x, f);
					xar_err_set_string(x, "Unable to seek");
					xar_err_callback(x, XAR_SEVERITY_NONFATAL, XAR_ERR_ARCHIVE_EXTRACTION);
				} else {
					len -= XAR(x)->heap_offset;
					buf = malloc(len);
					assert(buf);
					rr = read(XAR(x)->fd, buf, len);
					if( rr < len ) {
						xar_err_new(x);
						xar_err_set_file(x, f);
						xar_err_set_string(x, "Unable to seek");
						xar_err_callback(x, XAR_SEVERITY_NONFATAL, XAR_ERR_ARCHIVE_EXTRACTION);
					}
					free(buf);
				}
			} else {
				xar_err_new(x);
				xar_err_set_file(x, f);
				xar_err_set_string(x, "Unable to seek");
				xar_err_callback(x, XAR_SEVERITY_NONFATAL, XAR_ERR_ARCHIVE_EXTRACTION);
			}
		}
	}

	asprintf(&tmpstr, "%s/length", attr);
	xar_prop_get(f, tmpstr, &opt);
	free(tmpstr);
	if( !opt ) {
		return 0;
	} else {
		fsize = strtoll(opt, NULL, 10);
		if( ((fsize == LLONG_MAX) || (fsize == LLONG_MIN)) && (errno == ERANGE) ) {
			return -1;
		}
	}

	bsize = def_bsize;
	inbuf = malloc(bsize);
	if( !inbuf ) {
		return -1;
	}

	while(1) {
		/* Size has been reached */
		if( fsize == inc )
			break;
		if( (fsize - inc) < bsize )
			bsize = fsize - inc;
		r = read(XAR(x)->fd, inbuf, bsize);
		if( r == 0 )
			break;
		if( (r < 0) && (errno == EINTR) )
			continue;
		if( r < 0 ) {
			free(inbuf);
			return -1;
		}

		XAR(x)->heap_offset += r;
		inc += r;
		bsize = r;

		/* filter the data through the in modules */
		for( i = 0; i < (sizeof(xar_datamods)/sizeof(struct datamod)); i++) {
			if( xar_datamods[i].fh_in ) {
				int32_t ret;
				ret = xar_datamods[i].fh_in(x, f, attr, &inbuf, &bsize);
				if( ret < 0 )
					return -1;
			}
		}

		/* filter the data through the out modules */
		for( i = 0; i < (sizeof(xar_datamods)/sizeof(struct datamod)); i++) {
			if( xar_datamods[i].fh_out ) {
				int32_t ret;
				ret = xar_datamods[i].fh_out(x, f, attr, inbuf, bsize);
				if( ret < 0 )
					return -1;
			}
		}

		wcb(x, f, inbuf, bsize);
		free(inbuf);
		bsize = def_bsize;
		inbuf = malloc(bsize);
	}

	free(inbuf);
	/* finish up anything that still needs doing */
	for( i = 0; i < (sizeof(xar_datamods)/sizeof(struct datamod)); i++) {
		if( xar_datamods[i].fh_done ) {
			int32_t ret;
			ret = xar_datamods[i].fh_done(x, f, attr);
			if( ret < 0 )
				return ret;
		}
	}
	return 0;
}

/* xar_heap_to_archive
 * x: archive to operate on
 * Returns 0 on success, -1 on error
 * Summary: copies the heap into the archive.
 */
int32_t xar_heap_to_archive(xar_t x) {
	long bsize;
	ssize_t r;
	int off;
	const char *opt;
	char *b;

	opt = xar_opt_get(x, "rsize");
	if( !opt ) {
		bsize = 4096;
	} else {
		bsize = strtol(opt, NULL, 0);
		if( ((bsize == LONG_MAX) || (bsize == LONG_MIN)) && (errno == ERANGE) ) {
			bsize = 4096;
		}
	}

	b = malloc(bsize);
	if( !b ) return -1;

	while(1) {
		r = read(XAR(x)->heap_fd, b, bsize);
		if( r == 0 ) break;
		if( (r < 0) && (errno == EINTR) ) continue;
		if( r < 0 ) {
			free(b);
			return -1;
		}

		off = 0;
		do {
			r = write(XAR(x)->fd, b+off, bsize-off);
			if( (r < 0) && (errno != EINTR) )
				return -1;
			off += r;
		} while( off < bsize );
	}
	return 0;
}
