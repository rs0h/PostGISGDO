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
 * ip.h
 *	  Definitions for IPv6-aware network access.
 *
 * These definitions are used by both frontend and backend code.  Be careful
 * what you include here!
 *
 * Copyright (c) 2003-2010, PostgreSQL Global Development Group
 *
 * $PostgreSQL: pgsql/src/include/libpq/ip.h,v 1.24 2010/02/26 02:01:24 momjian Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef IP_H
#define IP_H

#include "getaddrinfo.h"
#include "libpq/pqcomm.h"


#ifdef	HAVE_UNIX_SOCKETS
#define IS_AF_UNIX(fam) ((fam) == AF_UNIX)
#else
#define IS_AF_UNIX(fam) (0)
#endif

typedef void (*PgIfAddrCallback) (struct sockaddr * addr,
											  struct sockaddr * netmask,
											  void *cb_data);

extern int pg_getaddrinfo_all(const char *hostname, const char *servname,
				   const struct addrinfo * hintp,
				   struct addrinfo ** result);
extern void pg_freeaddrinfo_all(int hint_ai_family, struct addrinfo * ai);

extern int pg_getnameinfo_all(const struct sockaddr_storage * addr, int salen,
				   char *node, int nodelen,
				   char *service, int servicelen,
				   int flags);

extern int pg_range_sockaddr(const struct sockaddr_storage * addr,
				  const struct sockaddr_storage * netaddr,
				  const struct sockaddr_storage * netmask);

extern int pg_sockaddr_cidr_mask(struct sockaddr_storage * mask,
					  char *numbits, int family);

#ifdef HAVE_IPV6
extern void pg_promote_v4_to_v6_addr(struct sockaddr_storage * addr);
extern void pg_promote_v4_to_v6_mask(struct sockaddr_storage * addr);
#endif

extern int	pg_foreach_ifaddr(PgIfAddrCallback callback, void *cb_data);

#endif   /* IP_H */
