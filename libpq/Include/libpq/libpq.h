// PostgreSQL Database Management System
// (formerly known as Postgres, then as Postgres95)
//
// Portions Copyright (c) 1996-2010, The PostgreSQL Global Development Group
//
// Portions Copyright (c) 1994, The Regents of the University of California
//
// Permission to use, copy, modify, and distribute this software and its
// documentation for any purpose, without fee, and without a written agreement
// is hereby granted, provided that the above copyright notice and this
// paragraph and the following two paragraphs appear in all copies.
//
// IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
// DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
// LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION,
// EVEN IF THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY
// OF SUCH DAMAGE.
//
// THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED HEREUNDER IS
// ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATIONS
// TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

/*-------------------------------------------------------------------------
 *
 * libpq.h
 *	  POSTGRES LIBPQ buffer structure definitions.
 *
 *
 * Portions Copyright (c) 1996-2010, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/include/libpq/libpq.h,v 1.75 2010/02/26 02:01:24 momjian Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef LIBPQ_H
#define LIBPQ_H

#include <sys/types.h>
#include <netinet/in.h>

#include "lib/stringinfo.h"
#include "libpq/libpq-be.h"

/* ----------------
 * PQArgBlock
 *		Information (pointer to array of this structure) required
 *		for the PQfn() call.  (This probably ought to go somewhere else...)
 * ----------------
 */
typedef struct
{
	int			len;
	int			isint;
	union
	{
		int		   *ptr;		/* can't use void (dec compiler barfs)	 */
		int			integer;
	}			u;
} PQArgBlock;

/*
 * External functions.
 */

/*
 * prototypes for functions in pqcomm.c
 */
extern int StreamServerPort(int family, char *hostName,
	unsigned short portNumber, char *unixSocketName, pgsocket ListenSocket[],
				 int MaxListen);
extern int	StreamConnection(pgsocket server_fd, Port *port);
extern void StreamClose(pgsocket sock);
extern void TouchSocketFile(void);
extern void pq_init(void);
extern void pq_comm_reset(void);
extern int	pq_getbytes(char *s, size_t len);
extern int	pq_getstring(StringInfo s);
extern int	pq_getmessage(StringInfo s, int maxlen);
extern int	pq_getbyte(void);
extern int	pq_peekbyte(void);
extern int	pq_getbyte_if_available(unsigned char *c);
extern int	pq_putbytes(const char *s, size_t len);
extern int	pq_flush(void);
extern int	pq_putmessage(char msgtype, const char *s, size_t len);
extern void pq_startcopyout(void);
extern void pq_endcopyout(bool errorAbort);

/*
 * prototypes for functions in be-secure.c
 */
extern int	secure_initialize(void);
extern bool secure_loaded_verify_locations(void);
extern void secure_destroy(void);
extern int	secure_open_server(Port *port);
extern void secure_close(Port *port);
extern ssize_t secure_read(Port *port, void *ptr, size_t len);
extern ssize_t secure_write(Port *port, void *ptr, size_t len);

#endif   /* LIBPQ_H */
