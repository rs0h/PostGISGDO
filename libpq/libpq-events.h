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
 * libpq-events.h
 *	  This file contains definitions that are useful to applications
 *	  that invoke the libpq "events" API, but are not interesting to
 *	  ordinary users of libpq.
 *
 * Portions Copyright (c) 1996-2010, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/interfaces/libpq/libpq-events.h,v 1.5 2010/01/02 16:58:12 momjian Exp $
 *
 *-------------------------------------------------------------------------
 */

#ifndef LIBPQ_EVENTS_H
#define LIBPQ_EVENTS_H

#include "libpq-fe.h"

#ifdef __cplusplus
extern		"C"
{
#endif

/* Callback Event Ids */
			typedef enum
{
	PGEVT_REGISTER,
	PGEVT_CONNRESET,
	PGEVT_CONNDESTROY,
	PGEVT_RESULTCREATE,
	PGEVT_RESULTCOPY,
	PGEVT_RESULTDESTROY
} PGEventId;

typedef struct
{
	PGconn	   *conn;
} PGEventRegister;

typedef struct
{
	PGconn	   *conn;
} PGEventConnReset;

typedef struct
{
	PGconn	   *conn;
} PGEventConnDestroy;

typedef struct
{
	PGconn	   *conn;
	PGresult   *result;
} PGEventResultCreate;

typedef struct
{
	const PGresult *src;
	PGresult   *dest;
} PGEventResultCopy;

typedef struct
{
	PGresult   *result;
} PGEventResultDestroy;

typedef int (*PGEventProc) (PGEventId evtId, void *evtInfo, void *passThrough);

/* Registers an event proc with the given PGconn. */
extern int PQregisterEventProc(PGconn *conn, PGEventProc proc,
					const char *name, void *passThrough);

/* Sets the PGconn instance data for the provided proc to data. */
extern int	PQsetInstanceData(PGconn *conn, PGEventProc proc, void *data);

/* Gets the PGconn instance data for the provided proc. */
extern void *PQinstanceData(const PGconn *conn, PGEventProc proc);

/* Sets the PGresult instance data for the provided proc to data. */
extern int	PQresultSetInstanceData(PGresult *result, PGEventProc proc, void *data);

/* Gets the PGresult instance data for the provided proc. */
extern void *PQresultInstanceData(const PGresult *result, PGEventProc proc);

/* Fires RESULTCREATE events for an application-created PGresult. */
extern int	PQfireResultCreateEvents(PGconn *conn, PGresult *res);

#ifdef __cplusplus
}
#endif

#endif   /* LIBPQ_EVENTS_H */
