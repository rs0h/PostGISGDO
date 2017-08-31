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
 * hba.h
 *	  Interface to hba.c
 *
 *
 * $PostgreSQL: pgsql/src/include/libpq/hba.h,v 1.62 2010/04/19 19:02:18 sriggs Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef HBA_H
#define HBA_H

#include "nodes/pg_list.h"
#include "libpq/pqcomm.h"


typedef enum UserAuth
{
	uaReject,
	uaImplicitReject,
	uaKrb5,
	uaTrust,
	uaIdent,
	uaPassword,
	uaMD5,
	uaGSS,
	uaSSPI,
	uaPAM,
	uaLDAP,
	uaCert,
	uaRADIUS
} UserAuth;

typedef enum IPCompareMethod
{
	ipCmpMask,
	ipCmpSameHost,
	ipCmpSameNet
} IPCompareMethod;

typedef enum ConnType
{
	ctLocal,
	ctHost,
	ctHostSSL,
	ctHostNoSSL
} ConnType;

typedef struct
{
	int			linenumber;
	ConnType	conntype;
	char	   *database;
	char	   *role;
	struct sockaddr_storage addr;
	struct sockaddr_storage mask;
	IPCompareMethod ip_cmp_method;
	UserAuth	auth_method;

	char	   *usermap;
	char	   *pamservice;
	bool		ldaptls;
	char	   *ldapserver;
	int			ldapport;
	char	   *ldapbinddn;
	char	   *ldapbindpasswd;
	char	   *ldapsearchattribute;
	char	   *ldapbasedn;
	char	   *ldapprefix;
	char	   *ldapsuffix;
	bool		clientcert;
	char	   *krb_server_hostname;
	char	   *krb_realm;
	bool		include_realm;
	char	   *radiusserver;
	char	   *radiussecret;
	char	   *radiusidentifier;
	int			radiusport;
} HbaLine;

/* kluge to avoid including libpq/libpq-be.h here */
typedef struct Port hbaPort;

extern bool load_hba(void);
extern void load_ident(void);
extern int	hba_getauthmethod(hbaPort *port);
extern int check_usermap(const char *usermap_name,
			  const char *pg_role, const char *auth_user,
			  bool case_sensitive);
extern bool pg_isblank(const char c);

#endif   /* HBA_H */
