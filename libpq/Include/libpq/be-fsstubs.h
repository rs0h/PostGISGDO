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
 * be-fsstubs.h
 *
 *
 *
 * Portions Copyright (c) 1996-2010, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/include/libpq/be-fsstubs.h,v 1.35 2010/02/26 02:01:24 momjian Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef BE_FSSTUBS_H
#define BE_FSSTUBS_H

#include "fmgr.h"

/*
 * LO functions available via pg_proc entries
 */
extern Datum lo_import(PG_FUNCTION_ARGS);
extern Datum lo_import_with_oid(PG_FUNCTION_ARGS);
extern Datum lo_export(PG_FUNCTION_ARGS);

extern Datum lo_creat(PG_FUNCTION_ARGS);
extern Datum lo_create(PG_FUNCTION_ARGS);

extern Datum lo_open(PG_FUNCTION_ARGS);
extern Datum lo_close(PG_FUNCTION_ARGS);

extern Datum loread(PG_FUNCTION_ARGS);
extern Datum lowrite(PG_FUNCTION_ARGS);

extern Datum lo_lseek(PG_FUNCTION_ARGS);
extern Datum lo_tell(PG_FUNCTION_ARGS);
extern Datum lo_unlink(PG_FUNCTION_ARGS);
extern Datum lo_truncate(PG_FUNCTION_ARGS);

/*
 * compatibility option for access control
 */
extern bool lo_compat_privileges;

/*
 * These are not fmgr-callable, but are available to C code.
 * Probably these should have had the underscore-free names,
 * but too late now...
 */
extern int	lo_read(int fd, char *buf, int len);
extern int	lo_write(int fd, const char *buf, int len);

/*
 * Cleanup LOs at xact commit/abort
 */
extern void AtEOXact_LargeObject(bool isCommit);
extern void AtEOSubXact_LargeObject(bool isCommit, SubTransactionId mySubid,
						SubTransactionId parentSubid);

#endif   /* BE_FSSTUBS_H */
