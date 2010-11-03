/* $Header: /pjproject/pjlib/src/test/fifobuf_test.cpp 4     5/12/05 9:53p Bennylp $
 */
/* 
 * PJLIB - PJ Foundation Library
 * (C)2003-2005 Benny Prijono <bennylp@bulukucing.org>
 *
 * Author:
 *  Benny Prijono <bennylp@bulukucing.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <pj/fifobuf.h>
#include <pj/pool.h>
#include "libpj_test.h"
#include <stdlib.h>
#include <stdio.h>

int fifobuf_test()
{
    enum { SIZE = 1024, MAX_ENTRIES = 128, MIN_SIZE = 4, MAX_SIZE = 64, LOOP=10000 };
    pj_pool_t *pool;
    pj_fifobuf_t fifo;
    unsigned available = SIZE;
    void *entries[MAX_ENTRIES];
    void *buffer;
    int i;

    pool = (*mem->create_pool)(mem, NULL, SIZE+256, 0, NULL);
    if (!pool)
	return -1;

    buffer = pj_pool_alloc(pool, SIZE);
    if (!buffer)
	return -1;

    pj_fifobuf_init (&fifo, buffer, SIZE);
    
    // Test 1
    for (i=0; i<LOOP*MAX_ENTRIES; ++i) {
	int size;
	int c, f;
	c = i%2;
	f = (i+1)%2;
	do {
	    size = MIN_SIZE+(rand() % MAX_SIZE);
	    entries[c] = pj_fifobuf_alloc (&fifo, size);
	} while (entries[c] == 0);
	if ( i!=0) {
	    pj_fifobuf_free(&fifo, entries[f]);
	}
    }
    if (entries[(i+1)%2])
	pj_fifobuf_free(&fifo, entries[(i+1)%2]);

    if (pj_fifobuf_max_size(&fifo) < SIZE-4) {
	pj_assert(0);
	return -1;
    }

    // Test 2
    entries[0] = pj_fifobuf_alloc (&fifo, MIN_SIZE);
    if (!entries[0]) return -1;
    for (i=0; i<LOOP*MAX_ENTRIES; ++i) {
	int size = MIN_SIZE+(rand() % MAX_SIZE);
	entries[1] = pj_fifobuf_alloc (&fifo, size);
	if (entries[1])
	    pj_fifobuf_unalloc(&fifo, entries[1]);
    }
    pj_fifobuf_unalloc(&fifo, entries[0]);
    if (pj_fifobuf_max_size(&fifo) < SIZE-4) {
	pj_assert(0);
	return -2;
    }

    // Test 3
    for (i=0; i<LOOP; ++i) {
	int count, j;
	for (count=0; available>=MIN_SIZE+4 && count < MAX_ENTRIES;) {
	    int size = MIN_SIZE+(rand() % MAX_SIZE);
	    entries[count] = pj_fifobuf_alloc (&fifo, size);
	    if (entries[count]) {
		available -= (size+4);
		++count;
	    }
	}
	for (j=0; j<count; ++j) {
	    pj_fifobuf_free (&fifo, entries[j]);
	}
	available = SIZE;
    }

    if (pj_fifobuf_max_size(&fifo) < SIZE-4) {
	pj_assert(0);
	return -3;
    }
    pj_pool_release(pool);
    return 0;
}