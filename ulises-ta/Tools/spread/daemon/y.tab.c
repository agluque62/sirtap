/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     SEGMENT = 258,
     EVENTLOGFILE = 259,
     EVENTTIMESTAMP = 260,
     EVENTPRECISETIMESTAMP = 261,
     EVENTPRIORITY = 262,
     IPADDR = 263,
     NUMBER = 264,
     COLON = 265,
     PDEBUG = 266,
     PINFO = 267,
     PWARNING = 268,
     PERROR = 269,
     PCRITICAL = 270,
     PFATAL = 271,
     OPENBRACE = 272,
     CLOSEBRACE = 273,
     EQUALS = 274,
     STRING = 275,
     DEBUGFLAGS = 276,
     BANG = 277,
     DDEBUG = 278,
     DEXIT = 279,
     DPRINT = 280,
     DDATA_LINK = 281,
     DNETWORK = 282,
     DPROTOCOL = 283,
     DSESSION = 284,
     DCONF = 285,
     DMEMB = 286,
     DFLOW_CONTROL = 287,
     DSTATUS = 288,
     DEVENTS = 289,
     DGROUPS = 290,
     DMEMORY = 291,
     DSKIPLIST = 292,
     DACM = 293,
     DSECURITY = 294,
     DALL = 295,
     DNONE = 296,
     DEBUGINITIALSEQUENCE = 297,
     DANGEROUSMONITOR = 298,
     SOCKETPORTREUSE = 299,
     RUNTIMEDIR = 300,
     SPUSER = 301,
     SPGROUP = 302,
     ALLOWEDAUTHMETHODS = 303,
     REQUIREDAUTHMETHODS = 304,
     ACCESSCONTROLPOLICY = 305,
     MAXSESSIONMESSAGES = 306,
     SP_BOOL = 307,
     SP_TRIVAL = 308,
     LINKPROTOCOL = 309,
     PHOP = 310,
     PTCPHOP = 311,
     IMONITOR = 312,
     ICLIENT = 313,
     IDAEMON = 314,
     ROUTEMATRIX = 315,
     LINKCOST = 316
   };
#endif
/* Tokens.  */
#define SEGMENT 258
#define EVENTLOGFILE 259
#define EVENTTIMESTAMP 260
#define EVENTPRECISETIMESTAMP 261
#define EVENTPRIORITY 262
#define IPADDR 263
#define NUMBER 264
#define COLON 265
#define PDEBUG 266
#define PINFO 267
#define PWARNING 268
#define PERROR 269
#define PCRITICAL 270
#define PFATAL 271
#define OPENBRACE 272
#define CLOSEBRACE 273
#define EQUALS 274
#define STRING 275
#define DEBUGFLAGS 276
#define BANG 277
#define DDEBUG 278
#define DEXIT 279
#define DPRINT 280
#define DDATA_LINK 281
#define DNETWORK 282
#define DPROTOCOL 283
#define DSESSION 284
#define DCONF 285
#define DMEMB 286
#define DFLOW_CONTROL 287
#define DSTATUS 288
#define DEVENTS 289
#define DGROUPS 290
#define DMEMORY 291
#define DSKIPLIST 292
#define DACM 293
#define DSECURITY 294
#define DALL 295
#define DNONE 296
#define DEBUGINITIALSEQUENCE 297
#define DANGEROUSMONITOR 298
#define SOCKETPORTREUSE 299
#define RUNTIMEDIR 300
#define SPUSER 301
#define SPGROUP 302
#define ALLOWEDAUTHMETHODS 303
#define REQUIREDAUTHMETHODS 304
#define ACCESSCONTROLPOLICY 305
#define MAXSESSIONMESSAGES 306
#define SP_BOOL 307
#define SP_TRIVAL 308
#define LINKPROTOCOL 309
#define PHOP 310
#define PTCPHOP 311
#define IMONITOR 312
#define ICLIENT 313
#define IDAEMON 314
#define ROUTEMATRIX 315
#define LINKCOST 316




/* Copy the first part of user declarations.  */
#line 1 "./config_parse.y"

/*
 * The Spread Toolkit.
 *     
 * The contents of this file are subject to the Spread Open-Source
 * License, Version 1.0 (the ``License''); you may not use
 * this file except in compliance with the License.  You may obtain a
 * copy of the License at:
 *
 * http://www.spread.org/license/
 *
 * or in the file ``license.txt'' found in this distribution.
 *
 * Software distributed under the License is distributed on an AS IS basis, 
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License 
 * for the specific language governing rights and limitations under the 
 * License.
 *
 * The Creators of Spread are:
 *  Yair Amir, Michal Miskin-Amir, Jonathan Stanton, John Schultz.
 *
 *  Copyright (C) 1993-2009 Spread Concepts LLC <info@spreadconcepts.com>
 *
 *  All Rights Reserved.
 *
 * Major Contributor(s):
 * ---------------
 *    Ryan Caudy           rcaudy@gmail.com - contributions to process groups.
 *    Claudiu Danilov      claudiu@acm.org - scalable wide area support.
 *    Cristina Nita-Rotaru crisn@cs.purdue.edu - group communication security.
 *    Theo Schlossnagle    jesus@omniti.com - Perl, autoconf, old skiplist.
 *    Dan Schoenblum       dansch@cnds.jhu.edu - Java interface.
 *
 */



#include "arch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ARCH_PC_WIN95
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/param.h>

#else /* ARCH_PC_WIN95 */
#include <winsock.h>
#endif /* ARCH_PC_WIN95 */

#include "alarm.h"
#include "configuration.h"
#include "memory.h"
#include "objects.h"
#include "conf_body.h"
#include "acm.h"

        int     line_num, semantic_errors;
 extern char    *yytext;
 extern int     yyerror(char *str);
 extern void    yywarn(char *str);
 extern int     yylex();

 static int     num_procs = 0;
 static int     segment_procs = 0;
 static int     segments = 0;
 static int     rvec_num = 0;
 static int     procs_interfaces = 0;

 static int     authentication_configured = 0;

void    parser_init()
{
    num_procs = 0;
    segment_procs = 0;
    segments = 0;
    rvec_num = 0;
    procs_interfaces = 0;
}

static char *segment2str(int seg) {
  static char ipstr[40];
  int id = Config->segments[seg].bcast_address;
  sprintf(ipstr, "%d.%d.%d.%d:%d",
  	(id & 0xff000000)>>24,
  	(id & 0xff0000)>>16,
  	(id & 0xff00)>>8,
  	(id & 0xff),
	Config->segments[seg].port);
  return ipstr;
}
static void alarm_print_proc(proc *p, int port) {
  if(port == p->port)
    Alarmp( SPLOG_DEBUG, CONF, "\t%20s: %d.%d.%d.%d\n", p->name,
  	  (p->id & 0xff000000)>>24,
  	  (p->id & 0xff0000)>>16,
  	  (p->id & 0xff00)>>8,
  	  (p->id & 0xff));
  else
    Alarmp( SPLOG_DEBUG, CONF, "\t%20s: %d.%d.%d.%d:%d\n", p->name,
  	  (p->id & 0xff000000)>>24,
  	  (p->id & 0xff0000)>>16,
  	  (p->id & 0xff00)>>8,
  	  (p->id & 0xff),
	  p->port);
}

static int32u name2ip(char *name) {
  int anip, i1, i2, i3, i4;
  struct hostent *host_ptr;

  host_ptr = gethostbyname(name);
  
  if ( host_ptr == 0)
    Alarm( EXIT, "Conf_init: no such host %s\n",
	   name);
  
  memcpy(&anip, host_ptr->h_addr_list[0], 
	 sizeof(int32) );
  anip = htonl( anip );
  i1= ( anip & 0xff000000 ) >> 24;
  i2= ( anip & 0x00ff0000 ) >> 16;
  i3= ( anip & 0x0000ff00 ) >>  8;
  i4=   anip & 0x000000ff;
  return ((i1<<24)|(i2<<16)|(i3<<8)|i4);
}

static  void expand_filename(char *out_string, int str_size, const char *in_string)
{
  const char *in_loc;
  char *out_loc;
  char hostn[MAXHOSTNAMELEN+1];
  
  for ( in_loc = in_string, out_loc = out_string; out_loc - out_string < str_size; in_loc++ )
  {
          if (*in_loc == '%' ) {
                  switch( in_loc[1] ) {
                  case 'h':
                  case 'H':
                          gethostname(hostn, sizeof(hostn) );
                          out_loc += snprintf(out_loc, str_size - (out_loc - out_string), "%s", hostn); 
                          in_loc++;
                          continue;
                  default:
                          break;
                  }

          }
          *out_loc = *in_loc;
          out_loc++;
          if (*in_loc == '\0') break;
  }
  out_string[str_size-1] = '\0';
}

static  int 	get_parsed_proc_info( char *name, proc *p )
{
	int	i;

	for ( i=0; i < num_procs; i++ )
	{
		if ( strcmp( Config_procs[i].name, name ) == 0 )
		{
			*p = Config_procs[i];
			return( i );
		}
	}
	return( -1 );
}
/* convert_segment_to_string()
 * char * segstr : output string
 * int strsize : length of output string space
 * segment *seg : input segment structure
 * int return : length of string written or -1 if error (like string not have room)
 * 
 *
 * The format of the returned string will be as shown below with each segment appended
 * to the string. Each use of IPB will be replaced with the broadcast IP address, port
 * with the port. The optional section is a list of interfaces tagged with D or C
 * and idnetified by ip address. 
 *
 * "Segment IP:port host1name host1ip (ALL/ANY/C/D/M IP)+ host2name host2ip (ALL/ANY/C/D/M IP )+ ..."
 *
 */
static  int    convert_segment_to_string(char *segstr, int strsize, segment *seg)
{
    int         i,j;
    size_t      curlen = 0;
    char        temp_str[200];

    sprintf(temp_str, "Segment %d.%d.%d.%d:%d ", 
            (seg->bcast_address & 0xff000000)>>24, 
            (seg->bcast_address & 0xff0000)>>16, 
            (seg->bcast_address & 0xff00)>>8, 
            (seg->bcast_address & 0xff), 
            seg->port );

    strncat( segstr, temp_str, strsize - curlen);
    curlen += strlen(temp_str);

    for (i = 0; i < seg->num_procs; i++) {
        sprintf(temp_str, "%s %d.%d.%d.%d ", 
                seg->procs[i]->name, 
                (seg->procs[i]->id & 0xff000000)>>24, 
                (seg->procs[i]->id & 0xff0000)>>16, 
                (seg->procs[i]->id & 0xff00)>>8, 
                (seg->procs[i]->id & 0xff) );
        strncat( segstr, temp_str, strsize - curlen);
        curlen += strlen(temp_str);

        /* Now add all interfaces */
        for ( j=0 ; j < seg->procs[i]->num_if; j++) {
            /* add addional interface specs to string */
            if ( seg->procs[i]->ifc[j].type & IFTYPE_ANY )
            {
                strncat( segstr, "ANY ", strsize - curlen);
                curlen += 4;
            }
            if ( seg->procs[i]->ifc[j].type & IFTYPE_DAEMON )
            {
                strncat( segstr, "D ", strsize - curlen);
                curlen += 2;
            }
            if ( seg->procs[i]->ifc[j].type & IFTYPE_CLIENT )
            {
                strncat( segstr, "C ", strsize - curlen);
                curlen += 2;
            }
            if ( seg->procs[i]->ifc[j].type & IFTYPE_MONITOR )
            {
                strncat( segstr, "M ", strsize - curlen);
                curlen += 2;
            }
            sprintf(temp_str, "%d.%d.%d.%d ", 
                (seg->procs[i]->ifc[j].ip & 0xff000000)>>24, 
                (seg->procs[i]->ifc[j].ip & 0xff0000)>>16, 
                (seg->procs[i]->ifc[j].ip & 0xff00)>>8, 
                (seg->procs[i]->ifc[j].ip & 0xff) );
            strncat( segstr, temp_str, strsize - curlen);
            curlen += strlen(temp_str);
        }
    }

    /* terminate each segment by a newline */
    strncat( segstr, "\n", strsize - curlen);
    curlen += 1;

    if (curlen > strsize) {
        /* ran out of space in string -- should never happen. */
        Alarmp( SPLOG_ERROR, CONF, "config_parse.y:convert_segment_to_string: The segment string is too long! %d characters attemped is more then %d characters allowed", curlen, strsize);
        Alarmp( SPLOG_ERROR, CONF, "config_parse.y:convert_segment_to_string:The error occured on segment %d.%d.%d.%d. Successful string was: %s\n",
                (seg->bcast_address & 0xff000000)>>24, 
                (seg->bcast_address & 0xff0000)>>16, 
                (seg->bcast_address & 0xff00)>>8, 
                (seg->bcast_address & 0xff), 
                segstr);
        return(-1);
    }

    Alarmp( SPLOG_DEBUG, CONF, "config_parse.y:convert_segment_to_string:The segment string is %d characters long:\n%s", curlen, segstr);
    return(curlen);
}

#define PROC_NAME_CHECK( stoken ) { \
                                            char strbuf[80]; \
                                            int ret; \
                                            proc p; \
                                            if ( strlen((stoken)) >= MAX_PROC_NAME ) { \
                                                snprintf(strbuf, 80, "Too long name(%d max): %s)\n", MAX_PROC_NAME, (stoken)); \
                                                return (yyerror(strbuf)); \
                                            } \
                                            ret = get_parsed_proc_info( stoken, &p ); \
                                            if (ret >= 0) { \
                                                snprintf(strbuf, 80, "Name not unique. name: %s equals (%s, %d.%d.%d.%d)\n", (stoken), p.name, IP1(p.id), IP2(p.id), IP3(p.id), IP4(p.id) ); \
                                                return (yyerror(strbuf)); \
                                            } \
                                         }
#define PROCS_CHECK( num_procs, stoken ) { \
                                            char strbuf[80]; \
                                            if ( (num_procs) >= MAX_PROCS_RING ) { \
                                                snprintf(strbuf, 80, "%s (Too many daemons configured--%d max)\n", (stoken), MAX_PROCS_RING); \
                                                return (yyerror(strbuf)); \
                                            } \
                                         }
#define SEGMENT_CHECK( num_segments, stoken )  { \
                                            char strbuf[80]; \
                                            if ( (num_segments) >= MAX_SEGMENTS ) { \
                                                snprintf(strbuf, 80, "%s (Too many segments configured--%d max)\n", (stoken), MAX_SEGMENTS); \
                                                return( yyerror(strbuf)); \
                                            } \
                                         }
#define SEGMENT_SIZE_CHECK( num_procs, stoken )  { \
                                            char strbuf[80]; \
                                            if ( (num_procs) >= MAX_PROCS_SEGMENT ) { \
                                                snprintf(strbuf, 80, "%s (Too many daemons configured in segment--%d max)\n", (stoken), MAX_PROCS_SEGMENT); \
                                                return( yyerror(strbuf)); \
                                            } \
                                         }
#define INTERFACE_NUM_CHECK( num_ifs, stoken )  { \
                                            char strbuf[80]; \
                                            if ( (num_ifs) >= MAX_INTERFACES_PROC ) { \
                                                snprintf(strbuf, 80, "%s (Too many interfaces configured in proc--%d max)\n", (stoken), MAX_INTERFACES_PROC); \
                                                return( yyerror(strbuf)); \
                                            } \
                                         }




/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 542 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  40
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   118

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  62
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  17
/* YYNRULES -- Number of rules.  */
#define YYNRULES  71
/* YYNRULES -- Number of states.  */
#define YYNSTATES  114

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   316

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     8,    11,    14,    15,    17,    19,
      21,    23,    25,    27,    29,    31,    33,    35,    37,    39,
      41,    43,    45,    47,    49,    51,    53,    56,    60,    61,
      63,    65,    67,    69,    71,    73,    79,    83,    87,    91,
      93,    95,    99,   103,   107,   109,   113,   117,   121,   125,
     129,   133,   137,   141,   147,   150,   151,   157,   162,   165,
     167,   169,   171,   173,   176,   177,   180,   181,   184,   189,
     192,   193
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      63,     0,    -1,    64,    -1,    69,    64,    -1,    68,    64,
      -1,    76,    64,    -1,    -1,    23,    -1,    24,    -1,    25,
      -1,    26,    -1,    27,    -1,    28,    -1,    29,    -1,    30,
      -1,    31,    -1,    32,    -1,    33,    -1,    34,    -1,    35,
      -1,    36,    -1,    37,    -1,    38,    -1,    39,    -1,    40,
      -1,    41,    -1,    66,    65,    -1,    66,    22,    65,    -1,
      -1,    11,    -1,    12,    -1,    13,    -1,    14,    -1,    15,
      -1,    16,    -1,    21,    19,    17,    66,    18,    -1,     7,
      19,    67,    -1,     4,    19,    20,    -1,     5,    19,    20,
      -1,     5,    -1,     6,    -1,    43,    19,    52,    -1,    44,
      19,    53,    -1,    45,    19,    20,    -1,    42,    -1,    46,
      19,    20,    -1,    47,    19,    20,    -1,    48,    19,    20,
      -1,    49,    19,    20,    -1,    50,    19,    20,    -1,    51,
      19,     9,    -1,    54,    19,    55,    -1,    54,    19,    56,
      -1,     3,     8,    17,    70,    18,    -1,    71,    70,    -1,
      -1,    20,     8,    17,    74,    18,    -1,    20,    17,    74,
      18,    -1,    20,     8,    -1,    20,    -1,    57,    -1,    58,
      -1,    59,    -1,    73,    72,    -1,    -1,    75,    74,    -1,
      -1,    73,     8,    -1,    60,    17,    77,    18,    -1,    77,
      78,    -1,    -1,    61,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   329,   329,   338,   339,   340,   341,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   361,   362,   365,   369,   373,   375,
     376,   377,   378,   379,   380,   383,   391,   398,   406,   412,
     418,   424,   430,   448,   452,   456,   460,   464,   488,   512,
     521,   525,   529,   534,   568,   569,   572,   592,   613,   634,
     658,   659,   660,   663,   667,   670,   671,   674,   690,   696,
     697,   700
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "SEGMENT", "EVENTLOGFILE",
  "EVENTTIMESTAMP", "EVENTPRECISETIMESTAMP", "EVENTPRIORITY", "IPADDR",
  "NUMBER", "COLON", "PDEBUG", "PINFO", "PWARNING", "PERROR", "PCRITICAL",
  "PFATAL", "OPENBRACE", "CLOSEBRACE", "EQUALS", "STRING", "DEBUGFLAGS",
  "BANG", "DDEBUG", "DEXIT", "DPRINT", "DDATA_LINK", "DNETWORK",
  "DPROTOCOL", "DSESSION", "DCONF", "DMEMB", "DFLOW_CONTROL", "DSTATUS",
  "DEVENTS", "DGROUPS", "DMEMORY", "DSKIPLIST", "DACM", "DSECURITY",
  "DALL", "DNONE", "DEBUGINITIALSEQUENCE", "DANGEROUSMONITOR",
  "SOCKETPORTREUSE", "RUNTIMEDIR", "SPUSER", "SPGROUP",
  "ALLOWEDAUTHMETHODS", "REQUIREDAUTHMETHODS", "ACCESSCONTROLPOLICY",
  "MAXSESSIONMESSAGES", "SP_BOOL", "SP_TRIVAL", "LINKPROTOCOL", "PHOP",
  "PTCPHOP", "IMONITOR", "ICLIENT", "IDAEMON", "ROUTEMATRIX", "LINKCOST",
  "$accept", "Config", "ConfigStructs", "AlarmBit", "AlarmBitComp",
  "PriorityLevel", "ParamStruct", "SegmentStruct", "Segmentparams",
  "Segmentparam", "IfType", "IfTypeComp", "Interfaceparams",
  "Interfaceparam", "RouteStruct", "Routevectors", "Routevector", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    62,    63,    64,    64,    64,    64,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    66,    66,    66,    67,
      67,    67,    67,    67,    67,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    69,    70,    70,    71,    71,    71,    71,
      72,    72,    72,    73,    73,    74,    74,    75,    76,    77,
      77,    78
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     2,     2,     0,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     3,     0,     1,
       1,     1,     1,     1,     1,     5,     3,     3,     3,     1,
       1,     3,     3,     3,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     5,     2,     0,     5,     4,     2,     1,
       1,     1,     1,     2,     0,     2,     0,     2,     4,     2,
       0,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       6,     0,     0,    39,    40,     0,     0,    44,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       2,     6,     6,     6,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    70,
       1,     4,     3,     5,    55,    37,    38,    29,    30,    31,
      32,    33,    34,    36,    28,    41,    42,    43,    45,    46,
      47,    48,    49,    50,    51,    52,     0,    59,     0,    55,
       0,    68,    71,    69,    58,    64,    53,    54,    35,     0,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      64,     0,     0,    64,    27,     0,    67,    60,    61,    62,
      63,    57,    65,    56
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    19,    20,    99,    70,    53,    21,    22,    68,    69,
     110,   101,   102,   103,    23,    66,    73
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -96
static const yytype_int8 yypact[] =
{
      -3,     7,    -2,    19,   -96,    31,    33,   -96,    34,    35,
      36,    37,    44,    45,    72,    73,    75,    76,    79,    62,
     -96,    -3,    -3,    -3,    80,    78,    81,    74,    82,    41,
      47,    83,    84,    85,    86,    87,    88,    93,   -42,   -96,
     -96,   -96,   -96,   -96,    89,   -96,   -96,   -96,   -96,   -96,
     -96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,
     -96,   -96,   -96,   -96,   -96,   -96,   -12,    -1,    92,    89,
      43,   -96,   -96,   -96,    94,    95,   -96,   -96,   -96,    -4,
     -96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,
     -96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,
      95,     1,    96,    95,   -96,    97,   -96,   -96,   -96,   -96,
     -96,   -96,   -96,   -96
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -96,   -96,   -11,    38,   -96,   -96,   -96,   -96,    49,   -96,
     -96,   -96,   -95,   -96,   -96,   -96,   -96
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -67
static const yytype_int8 yytable[] =
{
       1,     2,     3,     4,     5,   105,    71,    74,   112,   106,
      41,    42,    43,    64,    65,    24,    75,    25,     6,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    26,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    72,
      27,    17,    28,    29,    30,    31,    32,    18,   107,   108,
     109,    78,    40,    33,    34,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    47,    48,    49,    50,    51,
      52,    35,    36,    55,    37,    38,    39,    44,    45,    54,
      56,    46,    63,    57,    58,    59,    60,    61,    62,    67,
      76,   100,     0,   -66,   111,   113,     0,   104,    77
};

static const yytype_int8 yycheck[] =
{
       3,     4,     5,     6,     7,   100,    18,     8,   103,     8,
      21,    22,    23,    55,    56,     8,    17,    19,    21,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    19,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    61,
      19,    54,    19,    19,    19,    19,    19,    60,    57,    58,
      59,    18,     0,    19,    19,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    11,    12,    13,    14,    15,
      16,    19,    19,    52,    19,    19,    17,    17,    20,    17,
      53,    20,     9,    20,    20,    20,    20,    20,    20,    20,
      18,    17,    -1,    18,    18,    18,    -1,    79,    69
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,    21,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    54,    60,    63,
      64,    68,    69,    76,     8,    19,    19,    19,    19,    19,
      19,    19,    19,    19,    19,    19,    19,    19,    19,    17,
       0,    64,    64,    64,    17,    20,    20,    11,    12,    13,
      14,    15,    16,    67,    17,    52,    53,    20,    20,    20,
      20,    20,    20,     9,    55,    56,    77,    20,    70,    71,
      66,    18,    61,    78,     8,    17,    18,    70,    18,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    65,
      17,    73,    74,    75,    65,    74,     8,    57,    58,    59,
      72,    18,    74,    18
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 330 "./config_parse.y"
    {
			  Config->num_segments = segments;
			  Config->num_total_procs = num_procs;
			  Alarmp( SPLOG_DEBUG, CONF, "Finished configuration file.\n");
           Alarmp( SPLOG_DEBUG, CONF, "config_parse.y:The full segment string is %d characters long:\n%s", strlen(ConfStringRep), ConfStringRep);
			}
    break;

  case 7:
#line 344 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 8:
#line 345 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 9:
#line 346 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 10:
#line 347 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 11:
#line 348 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 12:
#line 349 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 13:
#line 350 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 14:
#line 351 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 15:
#line 352 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 16:
#line 353 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 17:
#line 354 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 18:
#line 355 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 19:
#line 356 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 20:
#line 357 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 21:
#line 358 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 22:
#line 359 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 23:
#line 360 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 24:
#line 361 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 25:
#line 362 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 26:
#line 366 "./config_parse.y"
    {
			  (yyval).mask = ((yyvsp[(1) - (2)]).mask | (yyvsp[(2) - (2)]).mask);
			}
    break;

  case 27:
#line 370 "./config_parse.y"
    {
			  (yyval).mask = ((yyvsp[(1) - (3)]).mask & ~((yyvsp[(3) - (3)]).mask));
			}
    break;

  case 28:
#line 373 "./config_parse.y"
    { (yyval).mask = NONE; }
    break;

  case 29:
#line 375 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 30:
#line 376 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 31:
#line 377 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 32:
#line 378 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 33:
#line 379 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 34:
#line 380 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 35:
#line 384 "./config_parse.y"
    {
			  if (! Alarm_get_interactive() ) {
                            Alarm_clear_types(ALL);
			    Alarm_set_types((yyvsp[(4) - (5)]).mask);
			    Alarmp( SPLOG_DEBUG, CONF, "Set Alarm mask to: %x\n", Alarm_get_types());
                          }
			}
    break;

  case 36:
#line 392 "./config_parse.y"
    {
                            if (! Alarm_get_interactive() ) {
                                Alarm_set_priority((yyvsp[(3) - (3)]).number);
                            }
                        }
    break;

  case 37:
#line 399 "./config_parse.y"
    {
			  if (! Alarm_get_interactive() ) {
                            char file_buf[MAXPATHLEN];
                            expand_filename(file_buf, MAXPATHLEN, (yyvsp[(3) - (3)]).string);
                            Alarm_set_output(file_buf);
                          }
			}
    break;

  case 38:
#line 407 "./config_parse.y"
    {
			  if (! Alarm_get_interactive() ) {
			    Alarm_enable_timestamp((yyvsp[(3) - (3)]).string);
                          }
			}
    break;

  case 39:
#line 413 "./config_parse.y"
    {
			  if (! Alarm_get_interactive() ) {
			    Alarm_enable_timestamp(NULL);
                          }
			}
    break;

  case 40:
#line 419 "./config_parse.y"
    {
			  if (! Alarm_get_interactive() ) {
                            Alarm_enable_precise_timestamp();
                          }
			}
    break;

  case 41:
#line 425 "./config_parse.y"
    {
			  if (! Alarm_get_interactive() ) {
                            Conf_set_dangerous_monitor_state((yyvsp[(3) - (3)]).boolean);
                          }
			}
    break;

  case 42:
#line 431 "./config_parse.y"
    {
                            port_reuse state;
                            if ((yyvsp[(3) - (3)]).number == 1)
                            {
                                state = port_reuse_on;
                            }
                            else if ((yyvsp[(3) - (3)]).number == 0)
                            {
                                state = port_reuse_off;
                            }
                            else
                            {
                                /* Default to AUTO. */
                                state = port_reuse_auto;
                            }
                            Conf_set_port_reuse_type(state);
                        }
    break;

  case 43:
#line 449 "./config_parse.y"
    {
                            Conf_set_runtime_dir((yyvsp[(3) - (3)]).string);
                        }
    break;

  case 44:
#line 453 "./config_parse.y"
    {
                            Conf_set_debug_initial_sequence();
                        }
    break;

  case 45:
#line 457 "./config_parse.y"
    {
                            Conf_set_user((yyvsp[(3) - (3)]).string);
                        }
    break;

  case 46:
#line 461 "./config_parse.y"
    {
                            Conf_set_group((yyvsp[(3) - (3)]).string);
                        }
    break;

  case 47:
#line 465 "./config_parse.y"
    {
                            char auth_list[MAX_AUTH_LIST_LEN];
                            int i, len;
                            char *c_ptr;
                            if (!authentication_configured) {
                                Acm_auth_set_disabled("NULL");
                            }
                            authentication_configured = 1;

                            strncpy(auth_list, (yyvsp[(3) - (3)]).string, MAX_AUTH_LIST_LEN);
                            len = strlen(auth_list); 
                            for (i=0; i < len; )
                            {
                                c_ptr = strchr(&auth_list[i], ' ');
                                if (c_ptr != NULL)
                                {
                                    *c_ptr = '\0';
                                }
                                Acm_auth_set_enabled(&auth_list[i]);    
                                i += strlen(&auth_list[i]);
                                i++; /* for null */
                            }
                        }
    break;

  case 48:
#line 489 "./config_parse.y"
    {
                            char auth_list[MAX_AUTH_LIST_LEN];
                            int i, len;
                            char *c_ptr;
                            if (!authentication_configured) {
                                Acm_auth_set_disabled("NULL");
                            }
                            authentication_configured = 1;

                            strncpy(auth_list, (yyvsp[(3) - (3)]).string, MAX_AUTH_LIST_LEN);
                            len = strlen(auth_list); 
                            for (i=0; i < len; )
                            {
                                c_ptr = strchr(&auth_list[i], ' ');
                                if (c_ptr != NULL)
                                {
                                    *c_ptr = '\0';
                                } 
                                Acm_auth_set_required(&auth_list[i]);    
                                i += strlen(&auth_list[i]);
                                i++; /* for null */
                            }
                        }
    break;

  case 49:
#line 513 "./config_parse.y"
    {
                            int ret;
                            ret = Acm_acp_set_policy((yyvsp[(3) - (3)]).string);
                            if (!ret)
                            {
                                    yyerror("Invalid Access Control Policy name. Make sure it is spelled right and any needed mocdules are loaded");
                            }
                        }
    break;

  case 50:
#line 522 "./config_parse.y"
    {
                            Conf_set_max_session_messages((yyvsp[(3) - (3)]).number);
			}
    break;

  case 51:
#line 526 "./config_parse.y"
    {
                            Conf_set_link_protocol(HOP_PROT);
			}
    break;

  case 52:
#line 530 "./config_parse.y"
    {
                            Conf_set_link_protocol(TCP_PROT);
			}
    break;

  case 53:
#line 535 "./config_parse.y"
    { int i;
                          int added_len;
                          SEGMENT_CHECK( segments, inet_ntoa((yyvsp[(2) - (5)]).ip.addr) );
			  Config->segments[segments].num_procs = segment_procs;
			  Config->segments[segments].port = (yyvsp[(2) - (5)]).ip.port;
			  Config->segments[segments].bcast_address =
			    (yyvsp[(2) - (5)]).ip.addr.s_addr;
			  if(Config->segments[segments].port == 0)
			    Config->segments[segments].port = DEFAULT_SPREAD_PORT;
			  Alarmp( SPLOG_DEBUG, CONF, "Successfully configured Segment %d [%s] with %d procs:\n",
				segments,
				segment2str(segments),
				segment_procs);
			  for(i=(num_procs - segment_procs); i<num_procs; i++) {
                                  /* This '1' is to keep each proc with the same port as the segment.*/
			    if( 1 || Config_procs[i].port==0)  {
			      Config_procs[i].port=
				Config->segments[segments].port;
			    }
			    alarm_print_proc(&Config_procs[i],
			    	Config->segments[segments].port);
			  }
                          /* generate string representation of segment */
                          added_len = convert_segment_to_string(&ConfStringRep[ConfStringLen], MAX_CONF_STRING - ConfStringLen, &Config->segments[segments] );
                          if (added_len == -1 )
                              yyerror("Failed to update string with segment!\n");
                          ConfStringLen += added_len;

			  segments++;
			  segment_procs = 0;
			}
    break;

  case 56:
#line 573 "./config_parse.y"
    { 
                          PROC_NAME_CHECK( (yyvsp[(1) - (5)]).string );
                          PROCS_CHECK( num_procs, (yyvsp[(1) - (5)]).string );
                          SEGMENT_CHECK( segments, (yyvsp[(1) - (5)]).string );
                          SEGMENT_SIZE_CHECK( segment_procs, (yyvsp[(1) - (5)]).string );
                          if (procs_interfaces == 0)
                                  yyerror("Interfaces section declared but no actual interface addresses defined\n");
                          strcpy(Config_procs[num_procs].name, (yyvsp[(1) - (5)]).string);
                          Config_procs[num_procs].id = (yyvsp[(2) - (5)]).ip.addr.s_addr;
 		          Config_procs[num_procs].port = (yyvsp[(2) - (5)]).ip.port;
			  Config_procs[num_procs].seg_index = segments;
			  Config_procs[num_procs].index_in_seg = segment_procs;
                          Config_procs[num_procs].num_if = procs_interfaces;
			  Config->segments[segments].procs[segment_procs] = 
                                  &Config_procs[num_procs];
			  num_procs++;
			  segment_procs++;
                          procs_interfaces = 0;
			}
    break;

  case 57:
#line 593 "./config_parse.y"
    { 
                          PROC_NAME_CHECK( (yyvsp[(1) - (4)]).string );
                          PROCS_CHECK( num_procs, (yyvsp[(1) - (4)]).string );
                          SEGMENT_CHECK( segments, (yyvsp[(1) - (4)]).string );
                          SEGMENT_SIZE_CHECK( segment_procs, (yyvsp[(1) - (4)]).string );
                          if (procs_interfaces == 0)
                                  yyerror("Interfaces section declared but no actual interface addresses defined\n");
                          strcpy(Config_procs[num_procs].name, (yyvsp[(1) - (4)]).string);
                          Config_procs[num_procs].id =
			    name2ip(Config_procs[num_procs].name);
 		          Config_procs[num_procs].port = 0;
			  Config_procs[num_procs].seg_index = segments;
			  Config_procs[num_procs].index_in_seg = segment_procs;
                          Config_procs[num_procs].num_if = procs_interfaces;
			  Config->segments[segments].procs[segment_procs] = 
                                  &Config_procs[num_procs];
			  num_procs++;
			  segment_procs++;
                          procs_interfaces = 0;
			}
    break;

  case 58:
#line 614 "./config_parse.y"
    { 
                          PROC_NAME_CHECK( (yyvsp[(1) - (2)]).string );
                          PROCS_CHECK( num_procs, (yyvsp[(1) - (2)]).string );
                          SEGMENT_CHECK( segments, (yyvsp[(1) - (2)]).string );
                          SEGMENT_SIZE_CHECK( segment_procs, (yyvsp[(1) - (2)]).string );
                          strcpy(Config_procs[num_procs].name, (yyvsp[(1) - (2)]).string);
                          Config_procs[num_procs].id = (yyvsp[(2) - (2)]).ip.addr.s_addr;
 		          Config_procs[num_procs].port = (yyvsp[(2) - (2)]).ip.port;
			  Config_procs[num_procs].seg_index = segments;
			  Config_procs[num_procs].index_in_seg = segment_procs;
                          Config_procs[num_procs].num_if = 1;
                          Config_procs[num_procs].ifc[0].ip = Config_procs[num_procs].id;
                          Config_procs[num_procs].ifc[0].port = Config_procs[num_procs].port;
                          Config_procs[num_procs].ifc[0].type = IFTYPE_ALL | IFTYPE_ANY;
			  Config->segments[segments].procs[segment_procs] = 
                                  &Config_procs[num_procs];
			  num_procs++;
			  segment_procs++;
                          procs_interfaces = 0;
			}
    break;

  case 59:
#line 635 "./config_parse.y"
    { 
                          PROC_NAME_CHECK( (yyvsp[(1) - (1)]).string );
                          PROCS_CHECK( num_procs, (yyvsp[(1) - (1)]).string );
                          SEGMENT_CHECK( segments, (yyvsp[(1) - (1)]).string );
                          SEGMENT_SIZE_CHECK( segment_procs, (yyvsp[(1) - (1)]).string );
                          strcpy(Config_procs[num_procs].name, (yyvsp[(1) - (1)]).string);
                          Config_procs[num_procs].id =
			    name2ip(Config_procs[num_procs].name);
 		          Config_procs[num_procs].port = 0;
			  Config_procs[num_procs].seg_index = segments;
			  Config_procs[num_procs].index_in_seg = segment_procs;
                          Config_procs[num_procs].num_if = 1;
                          Config_procs[num_procs].ifc[0].ip = Config_procs[num_procs].id;
                          Config_procs[num_procs].ifc[0].port = Config_procs[num_procs].port;
                          Config_procs[num_procs].ifc[0].type = IFTYPE_ALL | IFTYPE_ANY;
			  Config->segments[segments].procs[segment_procs] = 
                                  &Config_procs[num_procs];
			  num_procs++;
			  segment_procs++;
                          procs_interfaces = 0;
			}
    break;

  case 60:
#line 658 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 61:
#line 659 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 62:
#line 660 "./config_parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 63:
#line 664 "./config_parse.y"
    {
			  (yyval).mask = ((yyvsp[(1) - (2)]).mask | (yyvsp[(2) - (2)]).mask);
			}
    break;

  case 64:
#line 667 "./config_parse.y"
    { (yyval).mask = 0; }
    break;

  case 67:
#line 675 "./config_parse.y"
    { 
                          PROCS_CHECK( num_procs, (yyvsp[(1) - (2)]).string );
                          SEGMENT_CHECK( segments, (yyvsp[(1) - (2)]).string );
                          SEGMENT_SIZE_CHECK( segment_procs, (yyvsp[(1) - (2)]).string );
                          INTERFACE_NUM_CHECK( procs_interfaces, (yyvsp[(1) - (2)]).string );
                          Config_procs[num_procs].ifc[procs_interfaces].ip = (yyvsp[(2) - (2)]).ip.addr.s_addr;
                          Config_procs[num_procs].ifc[procs_interfaces].port = (yyvsp[(2) - (2)]).ip.port;
                          if ((yyvsp[(1) - (2)]).mask == 0)
                                  Config_procs[num_procs].ifc[procs_interfaces].type = IFTYPE_ALL;
                          else 
                                  Config_procs[num_procs].ifc[procs_interfaces].type = (yyvsp[(1) - (2)]).mask;
                          procs_interfaces++;
			}
    break;

  case 68:
#line 691 "./config_parse.y"
    { 
			  Alarmp( SPLOG_DEBUG, CONF, "Successfully configured Routing Matrix for %d Segments with %d rows in the routing matrix\n",segments, rvec_num);
			}
    break;

  case 71:
#line 701 "./config_parse.y"
    { 
                                int rvec_element;
                                for (rvec_element = 0; rvec_element < segments; rvec_element++) {
                                        if ((yyvsp[(1) - (1)]).cost[rvec_element] < 0) yyerror("Wrong number of entries for routing matrix");
                                        LinkWeights[rvec_num][rvec_element] = (yyvsp[(1) - (1)]).cost[rvec_element];
                                }
                                rvec_num++;
                        }
    break;


/* Line 1267 of yacc.c.  */
#line 2402 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 710 "./config_parse.y"

void yywarn(char *str) {
        fprintf(stderr, "-------Parse Warning-----------\n");
        fprintf(stderr, "Parser warning on or before line %d\n", line_num);
        fprintf(stderr, "Error type; %s\n", str);
        fprintf(stderr, "Offending token: %s\n", yytext);
}
int yyerror(char *str) {
  fprintf(stderr, "-------------------------------------------\n");
  fprintf(stderr, "Parser error on or before line %d\n", line_num);
  fprintf(stderr, "Error type; %s\n", str);
  fprintf(stderr, "Offending token: %s\n", yytext);
  exit(1);
}

