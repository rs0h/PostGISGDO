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

/*
 * $PostgreSQL: pgsql/src/include/port/win32/sys/socket.h,v 1.7 2006/10/04 00:30:10 momjian Exp $
 */
#ifndef WIN32_SYS_SOCKET_H
#define WIN32_SYS_SOCKET_H

/*
 * Unfortunately, <wingdi.h> of VC++ also defines ERROR.
 * To avoid the conflict, we include <windows.h> here and undefine ERROR
 * immediately.
 *
 * Note: Don't include <wingdi.h> directly.  It causes compile errors.
 */
#include <winsock2.h>
#include <ws2tcpip.h>

#undef ERROR
#undef small

/* Restore old ERROR value */
#ifdef PGERROR
#define ERROR PGERROR
#endif

/*
 * we can't use the windows gai_strerror{AW} functions because
 * they are defined inline in the MS header files. So we'll use our
 * own
 */
#undef gai_strerror

#endif   /* WIN32_SYS_SOCKET_H */
