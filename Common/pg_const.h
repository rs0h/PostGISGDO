// Copyright 2011 Intergraph Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// This file is derived from a PostgreSQL source file

/*-------------------------------------------------------------------------
 *
 * pg_type.h
 *	  definition of the system "type" relation (pg_type)
 *	  along with the relation's initial contents.
 *
 *
 * Portions Copyright (c) 1996-2010, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/include/catalog/pg_type.h,v 1.212 2010/01/05 01:06:57 tgl Exp $
 *
 * NOTES
 *	  the genbki.pl script reads this file and generates .bki
 *	  information from the DATA() statements.
 *
 *-------------------------------------------------------------------------
 */
#ifndef PG_CONST_H
#define PG_CONST_H

#define VARHDRSZ 4

/* ----------------
 *		compiler constants for pg_type
 * ----------------
 */
#define Natts_pg_type					28
#define Anum_pg_type_typname			1
#define Anum_pg_type_typnamespace		2
#define Anum_pg_type_typowner			3
#define Anum_pg_type_typlen				4
#define Anum_pg_type_typbyval			5
#define Anum_pg_type_typtype			6
#define Anum_pg_type_typcategory		7
#define Anum_pg_type_typispreferred		8
#define Anum_pg_type_typisdefined		9
#define Anum_pg_type_typdelim			10
#define Anum_pg_type_typrelid			11
#define Anum_pg_type_typelem			12
#define Anum_pg_type_typarray			13
#define Anum_pg_type_typinput			14
#define Anum_pg_type_typoutput			15
#define Anum_pg_type_typreceive			16
#define Anum_pg_type_typsend			17
#define Anum_pg_type_typmodin			18
#define Anum_pg_type_typmodout			19
#define Anum_pg_type_typanalyze			20
#define Anum_pg_type_typalign			21
#define Anum_pg_type_typstorage			22
#define Anum_pg_type_typnotnull			23
#define Anum_pg_type_typbasetype		24
#define Anum_pg_type_typtypmod			25
#define Anum_pg_type_typndims			26
#define Anum_pg_type_typdefaultbin		27
#define Anum_pg_type_typdefault			28


/* ----------------
 *		initial contents of pg_type
 * ----------------
 */

/*
 * Keep the following ordered by OID so that later changes can be made more
 * easily.
 *
 * For types used in the system catalogs, make sure the values here match
 * TypInfo[] in bootstrap.c.
 */

/* OIDS 1 - 99 */
#define BOOLOID			16
#define BYTEAOID		17
#define CHAROID			18
#define NAMEOID			19
#define INT8OID			20
#define INT2OID			21
#define INT2VECTOROID	22
#define INT4OID			23
#define REGPROCOID		24
#define TEXTOID			25
#define OIDOID			26
#define TIDOID		    27
#define XIDOID          28
#define CIDOID          29
#define OIDVECTOROID	30

#define XMLOID          142

#define POINTOID		600
#define LSEGOID			601
#define PATHOID			602
#define BOXOID			603
#define POLYGONOID		604
#define LINEOID			628

/* OIDS 700 - 799 */
#define FLOAT4OID 700
#define FLOAT8OID 701
#define ABSTIMEOID		702
#define RELTIMEOID		703
#define TINTERVALOID	704
#define UNKNOWNOID		705

#define CIRCLEOID		718
#define CASHOID 790
#define MACADDROID 829
#define INETOID 869
#define CIDROID 650

/* OIDS 900 - 999 */

/* OIDS 1000 - 1099 */
#define INT4ARRAYOID		1007
#define TEXTARRAYOID		1009
#define FLOAT4ARRAYOID 1021
#define ACLITEMOID		1033
#define CSTRINGARRAYOID		1263
#define BPCHAROID		1042
#define VARCHAROID		1043
#define DATEOID			1082
#define TIMEOID			1083

/* OIDS 1100 - 1199 */
#define TIMESTAMPOID	1114
#define TIMESTAMPTZOID	1184
#define INTERVALOID		1186

/* OIDS 1200 - 1299 */
#define TIMETZOID		1266
#define BITOID	 1560
#define VARBITOID	  1562

/* OIDS 1600 - 1699 */

/* OIDS 1700 - 1799 */
#define NUMERICOID		1700
#define REFCURSOROID	1790
#define REGPROCEDUREOID 2202
#define REGOPEROID		2203
#define REGOPERATOROID	2204
#define REGCLASSOID		2205
#define REGTYPEOID		2206
#define REGTYPEARRAYOID 2211
#define UUIDOID         2950
#define TSVECTOROID		3614
#define GTSVECTOROID	3642
#define TSQUERYOID		3615
#define REGCONFIGOID	3734
#define REGDICTIONARYOID	3769

/*
 * pseudo-types
 *
 * types with typtype='p' represent various special cases in the type system.
 *
 * These cannot be used to define table columns, but are valid as function
 * argument and result types (if supported by the function's implementation
 * language).
 *
 * Note: cstring is a borderline case; it is still considered a pseudo-type,
 * but there is now support for it in records and arrays.  Perhaps we should
 * just treat it as a regular base type?
 */
#define RECORDOID		2249
#define RECORDARRAYOID	2287
#define CSTRINGOID		2275
#define ANYOID			2276
#define ANYARRAYOID		2277
#define VOIDOID			2278
#define TRIGGEROID		2279
#define LANGUAGE_HANDLEROID		2280
#define INTERNALOID		2281
#define OPAQUEOID		2282
#define ANYELEMENTOID	2283
#define ANYNONARRAYOID	2776
#define ANYENUMOID		3500


/*
 * macros
 */
#define  TYPTYPE_BASE		'b' /* base type (ordinary scalar type) */
#define  TYPTYPE_COMPOSITE	'c' /* composite (e.g., table's rowtype) */
#define  TYPTYPE_DOMAIN		'd' /* domain over another type */
#define  TYPTYPE_ENUM		'e' /* enumerated type */
#define  TYPTYPE_PSEUDO		'p' /* pseudo-type */

#define  TYPCATEGORY_INVALID	'\0'	/* not an allowed category */
#define  TYPCATEGORY_ARRAY		'A'
#define  TYPCATEGORY_BOOLEAN	'B'
#define  TYPCATEGORY_COMPOSITE	'C'
#define  TYPCATEGORY_DATETIME	'D'
#define  TYPCATEGORY_ENUM		'E'
#define  TYPCATEGORY_GEOMETRIC	'G'
#define  TYPCATEGORY_NETWORK	'I'		/* think INET */
#define  TYPCATEGORY_NUMERIC	'N'
#define  TYPCATEGORY_PSEUDOTYPE 'P'
#define  TYPCATEGORY_STRING		'S'
#define  TYPCATEGORY_TIMESPAN	'T'
#define  TYPCATEGORY_USER		'U'
#define  TYPCATEGORY_BITSTRING	'V'		/* er ... "varbit"? */
#define  TYPCATEGORY_UNKNOWN	'X'

/* Is a type OID a polymorphic pseudotype?	(Beware of multiple evaluation) */
#define IsPolymorphicType(typid)  \
	((typid) == ANYELEMENTOID || \
	 (typid) == ANYARRAYOID || \
	 (typid) == ANYNONARRAYOID || \
	 (typid) == ANYENUMOID)

#endif   /* PG_CONST_H */
