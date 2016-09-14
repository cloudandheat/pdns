/*
 * This file is part of PowerDNS or dnsdist.
 * Copyright -- PowerDNS.COM B.V. and its contributors
 * originally authored by Norbert Sendetzky
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * In addition, for the avoidance of any doubt, permission is granted to
 * link this program with OpenSSL and to (re)distribute the binaries
 * produced as the result of such linking.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#include <map>
#include <string>
#include <vector>
#include <stdexcept>
#include <inttypes.h>
#include <errno.h>
#include <lber.h>
#include <ldap.h>




#ifndef POWERLDAP_HH
#define POWERLDAP_HH

using std::map;
using std::string;
using std::vector;

class LdapAuthenticator;

class PowerLDAP
{
        LDAP* d_ld;
        string d_hosts;
        int d_port;
        bool d_tls;

        const string getError( int rc = -1 );
        int waitResult( int msgid = LDAP_RES_ANY, int timeout = 0, LDAPMessage** result = NULL );
        void ensureConnect();
        
public:
        typedef map<string, vector<string> > sentry_t;
        typedef vector<sentry_t> sresult_t;

        PowerLDAP( const string& hosts = "ldap://127.0.0.1/", uint16_t port = LDAP_PORT, bool tls = false );
        ~PowerLDAP();

        bool connect();

        void getOption( int option, int* value );
        void setOption( int option, int value );

        void bind( LdapAuthenticator *authenticator );
        void bind( const string& ldapbinddn = "", const string& ldapsecret = "", int method = LDAP_AUTH_SIMPLE, int timeout = 5 );
        void simpleBind( const string& ldapbinddn = "", const string& ldapsecret = "" );
        int search( const string& base, int scope, const string& filter, const char** attr = 0 );

        bool getSearchEntry( int msgid, sentry_t& entry, bool dn = false, int timeout = 5 );
        void getSearchResults( int msgid, sresult_t& result, bool dn = false, int timeout = 5 );

        static const string escape( const string& tobe );
};



#endif
