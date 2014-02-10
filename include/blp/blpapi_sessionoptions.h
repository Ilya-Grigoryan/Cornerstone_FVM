/* Copyright 2012. Bloomberg Finance L.P.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:  The above copyright notice and this
 * permission notice shall be included in all copies or substantial portions of
 * the Software.  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
 * EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
// blpapi_sessionoptions.h                                            -*-C++-*-
#ifndef INCLUDED_BLPAPI_SESSIONOPTIONS
#define INCLUDED_BLPAPI_SESSIONOPTIONS

//@PURPOSE: A common interface shared between publish and consumer sessions.
//
//@CLASSES:
//  blpapi::SessionOptions: user specified options when creating a session.
//
//@SEE_ALSO: blpapi_abstractsession, blpapi_session, blpapi_providersession
//
//@DESCRIPTION: This file defines a 'SessionOptions' class which is used
// to specify various options during session creation.
//
//
///Usage
///-----
// The following snippet shows to use the SessionOptions when creating a
// 'Session'. 
//..
// #include <session.h>
// SessionOptions sessionOptions;
// sessionOptions.setServerHost("127.0.0.1");
// Session session(sessionOptions);
// if (!session.start()) {
//      std::cout << "Failed to start session." << std::endl;
//      return;
// }
//..

#include <blpapi_defs.h>
#include <blpapi_exception.h>
#include <blpapi_types.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

BLPAPI_EXPORT
blpapi_SessionOptions_t* blpapi_SessionOptions_create();

BLPAPI_EXPORT
blpapi_SessionOptions_t* blpapi_SessionOptions_duplicate(
        const blpapi_SessionOptions_t* parameters);

BLPAPI_EXPORT
void blpapi_SessionOptions_copy(blpapi_SessionOptions_t* lhs,
        const blpapi_SessionOptions_t* rhs);

BLPAPI_EXPORT
void blpapi_SessionOptions_destroy(
        blpapi_SessionOptions_t *parameters);

BLPAPI_EXPORT
int blpapi_SessionOptions_setServerHost(
        blpapi_SessionOptions_t *parameters,
        const char* serverHost);

BLPAPI_EXPORT
int blpapi_SessionOptions_setServerPort(
        blpapi_SessionOptions_t *parameters,
        unsigned short serverPort);

BLPAPI_EXPORT
int blpapi_SessionOptions_setServerAddress(
        blpapi_SessionOptions_t *parameters,
        const char* serverHost,
        unsigned short serverPort,
        size_t index);

BLPAPI_EXPORT
int blpapi_SessionOptions_removeServerAddress(
        blpapi_SessionOptions_t *parameters,
        size_t index);

BLPAPI_EXPORT
int blpapi_SessionOptions_setConnectTimeout(
        blpapi_SessionOptions_t *parameters,
        unsigned int connectTimeoutInSeconds);

BLPAPI_EXPORT
int blpapi_SessionOptions_setDefaultServices(
        blpapi_SessionOptions_t *parameters,
        const char* defaultServices);

BLPAPI_EXPORT
int blpapi_SessionOptions_setDefaultSubscriptionService(
        blpapi_SessionOptions_t *parameters,
        const char* defaultSubscriptionService);

BLPAPI_EXPORT
void blpapi_SessionOptions_setDefaultTopicPrefix(
        blpapi_SessionOptions_t *parameters,
        const char* defaultTopicPrefix);

BLPAPI_EXPORT
void blpapi_SessionOptions_setAllowMultipleCorrelatorsPerMsg(
        blpapi_SessionOptions_t *parameters,
        int allowMultipleCorrelatorsPerMsg);

BLPAPI_EXPORT
void blpapi_SessionOptions_setClientMode(
        blpapi_SessionOptions_t *parameters,
        int clientMode);

BLPAPI_EXPORT
void blpapi_SessionOptions_setMaxPendingRequests(
        blpapi_SessionOptions_t *parameters,
        int maxPendingRequests);

BLPAPI_EXPORT
void blpapi_SessionOptions_setAutoRestartOnDisconnection(
        blpapi_SessionOptions_t *parameters,
        int                      autoRestart);

BLPAPI_EXPORT
void blpapi_SessionOptions_setAutoRestart(
        blpapi_SessionOptions_t *parameters,
        int                     autoRestart);

BLPAPI_EXPORT
void blpapi_SessionOptions_setAuthenticationOptions(
        blpapi_SessionOptions_t *parameters,
        const char* authOptions);

BLPAPI_EXPORT
void blpapi_SessionOptions_setNumStartAttempts(
        blpapi_SessionOptions_t *parameters,
        int numStartAttempts);

BLPAPI_EXPORT
const char* blpapi_SessionOptions_serverHost(
        blpapi_SessionOptions_t *parameters);

BLPAPI_EXPORT
unsigned int blpapi_SessionOptions_serverPort(
        blpapi_SessionOptions_t *parameters);

BLPAPI_EXPORT
int blpapi_SessionOptions_numServerAddresses(
        blpapi_SessionOptions_t *parameters);

BLPAPI_EXPORT
int blpapi_SessionOptions_getServerAddress(
        blpapi_SessionOptions_t *parameters,
        const char** serverHost,
        unsigned short *serverPort,
        size_t index);

BLPAPI_EXPORT
unsigned int blpapi_SessionOptions_connectTimeout(
        blpapi_SessionOptions_t *parameters);

BLPAPI_EXPORT
const char* blpapi_SessionOptions_defaultServices(
        blpapi_SessionOptions_t *parameters);

BLPAPI_EXPORT
const char* blpapi_SessionOptions_defaultSubscriptionService(
        blpapi_SessionOptions_t *parameters);

BLPAPI_EXPORT
const char* blpapi_SessionOptions_defaultTopicPrefix(
        blpapi_SessionOptions_t *parameters);

BLPAPI_EXPORT
int blpapi_SessionOptions_allowMultipleCorrelatorsPerMsg(
        blpapi_SessionOptions_t *parameters);

BLPAPI_EXPORT
int blpapi_SessionOptions_clientMode(
        blpapi_SessionOptions_t *parameters);

BLPAPI_EXPORT
int blpapi_SessionOptions_maxPendingRequests(
        blpapi_SessionOptions_t *parameters);

BLPAPI_EXPORT
int blpapi_SessionOptions_autoRestartOnDisconnection(
        blpapi_SessionOptions_t *parameters);

BLPAPI_EXPORT
int blpapi_SessionOptions_autoRestart(
        blpapi_SessionOptions_t *parameters);

BLPAPI_EXPORT
const char* blpapi_SessionOptions_authenticationOptions(
        blpapi_SessionOptions_t *parameters);

BLPAPI_EXPORT
int blpapi_SessionOptions_numStartAttempts(
        blpapi_SessionOptions_t *parameters);

#ifdef __cplusplus
}

namespace BloombergLP {
namespace blpapi {

class SessionOptions {
    // Contains the options which the user can specify when creating a
    // session.
    //
    // To use non-default options on a Session, create a
    // SessionOptions instance and set the required options and then
    // supply it when creating a Session.

    blpapi_SessionOptions_t *d_handle_p;

  public:

    // The possible options for how to connect to the API

    enum ClientMode {
        AUTO = BLPAPI_CLIENTMODE_AUTO,  // Automatic (desktop if available
                                        // otherwise server)
        DAPI = BLPAPI_CLIENTMODE_DAPI,  // Always connect to the desktop API
        SAPI = BLPAPI_CLIENTMODE_SAPI   // Always connect to the server API
    };

    SessionOptions();
        // Create a SessionOptions with all options set to the
        // standard defaults.

    SessionOptions(const SessionOptions& original);
        // Copy constructor

    ~SessionOptions();
        // Destroy this SessionOptions.

    // MANIUPLATORS

    SessionOptions& operator=(const SessionOptions& rhs);
        // Assign to this object the value of the specified 'rhs' object.

    void setServerHost(const char* host);
        // Set the API server host to connect to when using the server
        // API to the specified 'host'. A hostname or an IPv4 address
        // (that is, a.b.c.d). The default is "127.0.0.1".

    void setServerPort(unsigned short port);
        // Set the port to connect to when using the server API to the
        // specified 'port'. The default is 8194.

    int setServerAddress(
            const char*    serverHost,
            unsigned short serverPort,
            size_t         index);
        // Set the server address at the specified 'index' using the specified 
        // 'serverHost' and 'serverPort'.

    int removeServerAddress(size_t index);
        // Remove the server address at the specified 'index'.

    void setConnectTimeout(unsigned int timeoutMilliSeconds);
        // Set the connection timeout in milliSeconds when connecting to the
        // API. The default is 5000 milliSeconds. Behavior is not defined
        // unless the specified 'timeoutMilliSeconds' is in range of
        // [1 .. 120000] milliSeconds

    void setDefaultServices(const char* defaultServices);
        // Set the default service for the session.

    void setDefaultSubscriptionService(const char* defaultSubscriptionService);
        // Set the default service for subscriptions which do not
        // specify a subscription server to the specified
        // 'defaultSubscriptionService'. The default is
        // "//blp/mktdata".

    void setDefaultTopicPrefix(const char* defaultTopicPrefix);
        // Set the default topic prefix to be used when a subscription
        // does not specify a prefix to the specified
        // 'defaultTopicPrefix'. The default is "ticker"

    void setAllowMultipleCorrelatorsPerMsg(
                                          bool allowMultipleCorrelatorsPerMsg);
        // Set whether the Session is allowed to associate more than
        // one CorrelationId with a Message to the specified
        // 'allowMultipleCorrelatorsPerMsg'. The default is
        // false. This means that if you have multiple subscriptions
        // which overlap (that is a particular Message is relevant to
        // all of them) you will be presented with the same message
        // multiple times when you use the MessageIterator, each time
        // with a different CorrelationId. If you specify true for
        // this then a Message may be presented with multiple
        // CorrelationId's.

    void setClientMode(int clientMode);
        // Set how to connect to the API. The default is AUTO which
        // will try to connect to the desktop API but fall back to the
        // server API if the desktop is not available. DAPI always
        // connects to the desktop API and will fail if it is not
        // available. SAPI always connects to the server API and will
        // fail if it is not available.

    void setMaxPendingRequests(int maxPendingRequests);
        // Set the maximum number of requests which can be pending to
        // the specified 'maxPendingRequests'. The default is 1024.

    void setAuthenticationOptions(const char *authOptions);
        // Set the specified 'authOptions' as authentication option.

    void setNumStartAttempts(int numStartAttempts);
        // Set the maxinum number of attempts to start a session
        // by connecting a server.

    void setAutoRestartOnDisconnection(bool autoRestart);
        // Set whether automatically restarting connection if disconnected.

    // ACCESSORS
    const char* serverHost() const;
        // Return a pointer to the value of the server host option in
        // this SessionOptions instance. The pointer is valid until
        // this SessionOptions is destroyed setServerHost() is
        // called.

    unsigned short serverPort() const;
        // Return the server port that this session connects to. If more than
        // one server addresses are specified, return the port of the first
        // server address.

    size_t numServerAddresses() const;
        // Return the number of server addresses.

    int getServerAddress(
            const char**    serverHost,
            unsigned short* serverPort,
            size_t          index) const;
        // Put the server name and port into 'serverHost' and 'serverPort'
        // indexed by 'index'. Return 0 if succeeded; otherwise, return
        // non-zero.

    unsigned int connectTimeout() const;
        // Return the value of the connection timeout option in this
        // SessionOptions instance in milliseconds.

    const char *defaultServices() const;
        // Return all default services in one string.

    const char *defaultSubscriptionService() const;
        // Return a pointer to the value of the default subscription
        // service option in this SessionOptions instance. The pointer
        // is valid until this SessionOptions is destroyed or
        // setDefaultSubscriptionService() is called.

    const char *defaultTopicPrefix() const;
        // Return a pointer to the value of the default topic prefix
        // option in this SessionOptions instance. The pointer is
        // valid until this SessionOptions is destroyed or
        // setDefaultTopicPrefix() is called.

    bool allowMultipleCorrelatorsPerMsg() const;
        // Return the value of the allow multiple correlators per
        // message option in this SessionOptions instance.

    int clientMode() const;
        // Return the value of the client mode option in this
        // SessionOptions instance.

    int maxPendingRequests() const;
        // Return the value of the maximum pending request option in
        // this SessionOptions instance.

    blpapi_SessionOptions_t *handle() const;
        // Return the handle of the current session

    bool autoRestartOnDisconnection() const;
        // Return whether automatically restarting connection if disconnected.

    const char *authenticationOptions() const;
        // Return authentication options in a string.

    int numStartAttempts() const;
        // Return the maximum number of attempts to start a session
        // by connecting a server.
};

inline
SessionOptions::SessionOptions()
{
    d_handle_p = blpapi_SessionOptions_create();
#if BLPAPI_COMPAT_33X
    blpapi_SessionOptions_setClientMode(
            d_handle_p,
            BLPAPI_CLIENTMODE_AUTO | BLPAPI_CLIENTMODE_COMPAT_33X);
#endif
}

inline
SessionOptions::SessionOptions(const SessionOptions& options)
{
    d_handle_p = blpapi_SessionOptions_duplicate(options.handle());
}

inline
SessionOptions::~SessionOptions()
{
    blpapi_SessionOptions_destroy(d_handle_p);
}

inline
SessionOptions& SessionOptions::operator=(const SessionOptions& rhs)
{
    blpapi_SessionOptions_copy(this->handle(), rhs.handle());
    return *this;
}

inline
void SessionOptions::setServerHost(const char *serverHost)
{
    blpapi_SessionOptions_setServerHost(d_handle_p, serverHost);
}

inline
void SessionOptions::setServerPort(unsigned short serverPort)
{
    blpapi_SessionOptions_setServerPort(d_handle_p, serverPort);
}

inline
int SessionOptions::setServerAddress(const char     *serverHost,
                                     unsigned short  serverPort,
                                     size_t          index)
{
    return blpapi_SessionOptions_setServerAddress(
            d_handle_p, serverHost, serverPort, index);
}

inline
int SessionOptions::removeServerAddress(size_t index)
{
    return blpapi_SessionOptions_removeServerAddress(d_handle_p, index);
}

inline
void SessionOptions::setConnectTimeout(unsigned int timeoutMilliSeconds)
{
    ExceptionUtil::throwOnError(
        blpapi_SessionOptions_setConnectTimeout(
            d_handle_p, timeoutMilliSeconds));
}

inline
void SessionOptions::setDefaultServices(const char* defaultServices)
{
    blpapi_SessionOptions_setDefaultServices(d_handle_p, defaultServices);
}

inline
void SessionOptions::setDefaultSubscriptionService(
                    const char* defaultSubscriptionService)
{
    blpapi_SessionOptions_setDefaultSubscriptionService(
            d_handle_p,
            defaultSubscriptionService);
}

inline
void SessionOptions::setDefaultTopicPrefix(
                        const char* defaultTopicPrefix)
{
    blpapi_SessionOptions_setDefaultTopicPrefix(
            d_handle_p,
            defaultTopicPrefix);
}

inline
void SessionOptions::setAllowMultipleCorrelatorsPerMsg(
                    bool allowMultipleCorrelatorsPerMsg)
{
    blpapi_SessionOptions_setAllowMultipleCorrelatorsPerMsg(
            d_handle_p,
            allowMultipleCorrelatorsPerMsg);
}

inline
void SessionOptions::setClientMode(int clientMode)
{
#if BLPAPI_COMPAT_33X
    clientMode |= BLPAPI_CLIENTMODE_COMPAT_33X;
#endif

    blpapi_SessionOptions_setClientMode(
            d_handle_p,
            clientMode);
}

inline
void SessionOptions::setMaxPendingRequests(int maxPendingRequests)
{
    blpapi_SessionOptions_setMaxPendingRequests(
            d_handle_p,
            maxPendingRequests);
}

inline
void SessionOptions::setAutoRestartOnDisconnection(bool autoRestart)
{
    blpapi_SessionOptions_setAutoRestartOnDisconnection(
            d_handle_p,
            autoRestart? 1: 0);
}

inline
void SessionOptions::setAuthenticationOptions(const char *authOptions)
{
    blpapi_SessionOptions_setAuthenticationOptions(
            d_handle_p, authOptions);
}

inline
void SessionOptions::setNumStartAttempts(int numStartAttempts)
{
    blpapi_SessionOptions_setNumStartAttempts(
            d_handle_p, numStartAttempts);
}

inline
const char* SessionOptions::serverHost() const
{
    return blpapi_SessionOptions_serverHost(d_handle_p);
}

inline
unsigned short SessionOptions::serverPort() const
{
    return blpapi_SessionOptions_serverPort(d_handle_p);
}

inline
size_t SessionOptions::numServerAddresses() const
{
    return blpapi_SessionOptions_numServerAddresses(d_handle_p);
}

inline
int SessionOptions::getServerAddress(
                            const char**    serverHost,
                            unsigned short* serverPort,
                            size_t          index) const
{
    return blpapi_SessionOptions_getServerAddress(
                d_handle_p, serverHost, serverPort, index);
}

inline
unsigned int SessionOptions::connectTimeout() const
{
    return blpapi_SessionOptions_connectTimeout(d_handle_p);
}

inline
const char* SessionOptions::defaultServices() const
{
    return blpapi_SessionOptions_defaultServices(d_handle_p);
}

inline
const char* SessionOptions::defaultSubscriptionService() const
{
    return blpapi_SessionOptions_defaultSubscriptionService(d_handle_p);
}

inline
const char* SessionOptions::defaultTopicPrefix() const
{
    return blpapi_SessionOptions_defaultTopicPrefix(d_handle_p);
}

inline
bool SessionOptions::allowMultipleCorrelatorsPerMsg() const
{
    return blpapi_SessionOptions_allowMultipleCorrelatorsPerMsg(d_handle_p) ?
        true : false;
}

inline
int SessionOptions::clientMode() const
{
    return blpapi_SessionOptions_clientMode(d_handle_p);
}

inline
int SessionOptions::maxPendingRequests() const
{
    return blpapi_SessionOptions_maxPendingRequests(d_handle_p);
}

inline
bool SessionOptions::autoRestartOnDisconnection() const
{
    return blpapi_SessionOptions_autoRestartOnDisconnection(d_handle_p) != 0;
}

inline
const char* SessionOptions::authenticationOptions() const
{
    return blpapi_SessionOptions_authenticationOptions(d_handle_p);
}

inline
int SessionOptions::numStartAttempts() const
{
    return blpapi_SessionOptions_numStartAttempts(d_handle_p);
}

inline
blpapi_SessionOptions_t* SessionOptions::handle() const
{
    return d_handle_p;
}

}  // close namespace blpapi {
}  // close namespace BloombergLP {

#endif // #ifdef __cplusplus
#endif // #ifndef INCLUDED_BLPAPI_ABSTRACTSESSION