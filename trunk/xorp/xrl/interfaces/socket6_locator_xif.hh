/*
 * Copyright (c) 2001-2007 International Computer Science Institute
 * See LICENSE file for licensing, conditions, and warranties on use.
 *
 * DO NOT EDIT THIS FILE - IT IS PROGRAMMATICALLY GENERATED
 *
 * Generated by 'clnt-gen'.
 *
 * $XORP: xorp/xrl/interfaces/socket6_locator_xif.hh,v 1.7 2007/02/16 22:47:44 pavlin Exp $
 */

#ifndef __XRL_INTERFACES_SOCKET6_LOCATOR_XIF_HH__
#define __XRL_INTERFACES_SOCKET6_LOCATOR_XIF_HH__

#undef XORP_LIBRARY_NAME
#define XORP_LIBRARY_NAME "XifSocket6Locator"

#include "libxorp/xlog.h"
#include "libxorp/callback.hh"

#include "libxipc/xrl.hh"
#include "libxipc/xrl_error.hh"
#include "libxipc/xrl_sender.hh"


class XrlSocket6LocatorV0p1Client {
public:
    XrlSocket6LocatorV0p1Client(XrlSender* s) : _sender(s) {}
    virtual ~XrlSocket6LocatorV0p1Client() {}

    typedef XorpCallback2<void, const XrlError&, const string*>::RefPtr FindSocketServerForAddrCB;
    /**
     *  Send Xrl intended to:
     *
     *  Find suitable socket serving Xrl Target for a particular interface
     *  address.
     *
     *  @param dst_xrl_target_name the Xrl target name of the destination.
     */
    bool send_find_socket_server_for_addr(
	const char*	dst_xrl_target_name,
	const IPv6&	local_addr,
	const FindSocketServerForAddrCB&	cb
    );

protected:
    XrlSender* _sender;

private:
    void unmarshall_find_socket_server_for_addr(
	const XrlError&	e,
	XrlArgs*	a,
	FindSocketServerForAddrCB		cb
    );

};

#endif /* __XRL_INTERFACES_SOCKET6_LOCATOR_XIF_HH__ */
