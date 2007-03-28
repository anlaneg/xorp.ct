/*
 * Copyright (c) 2001-2007 International Computer Science Institute
 * See LICENSE file for licensing, conditions, and warranties on use.
 *
 * DO NOT EDIT THIS FILE - IT IS PROGRAMMATICALLY GENERATED
 *
 * Generated by 'clnt-gen'.
 *
 * $XORP: xorp/xrl/interfaces/policy_redist6_xif.hh,v 1.6 2006/03/27 00:09:18 pavlin Exp $
 */

#ifndef __XRL_INTERFACES_POLICY_REDIST6_XIF_HH__
#define __XRL_INTERFACES_POLICY_REDIST6_XIF_HH__

#undef XORP_LIBRARY_NAME
#define XORP_LIBRARY_NAME "XifPolicyRedist6"

#include "libxorp/xlog.h"
#include "libxorp/callback.hh"

#include "libxipc/xrl.hh"
#include "libxipc/xrl_error.hh"
#include "libxipc/xrl_sender.hh"


class XrlPolicyRedist6V0p1Client {
public:
    XrlPolicyRedist6V0p1Client(XrlSender* s) : _sender(s) {}
    virtual ~XrlPolicyRedist6V0p1Client() {}

    typedef XorpCallback1<void, const XrlError&>::RefPtr AddRoute6CB;
    /**
     *  Send Xrl intended to:
     *
     *  Start route redistribution for an IPv6 route.
     *
     *  @param dst_xrl_target_name the Xrl target name of the destination.
     *
     *  @param network the route to advertise.
     *
     *  @param unicast whether the route is unicast.
     *
     *  @param multicast whether the route is multicast.
     *
     *  @param nexthop the nexthop of the route.
     *
     *  @param metric the metric of the route.
     *
     *  @param policytags the set of policy-tags associated with the route.
     */
    bool send_add_route6(
	const char*	dst_xrl_target_name,
	const IPv6Net&	network,
	const bool&	unicast,
	const bool&	multicast,
	const IPv6&	nexthop,
	const uint32_t&	metric,
	const XrlAtomList&	policytags,
	const AddRoute6CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr DeleteRoute6CB;
    /**
     *  Send Xrl intended to:
     *
     *  Terminate route redistribution for an IPv6 route.
     *
     *  @param dst_xrl_target_name the Xrl target name of the destination.
     *
     *  @param network the route for which advertisements should cease.
     *
     *  @param unicast whether the route is unicast.
     *
     *  @param multicast whether the route is multicast.
     */
    bool send_delete_route6(
	const char*	dst_xrl_target_name,
	const IPv6Net&	network,
	const bool&	unicast,
	const bool&	multicast,
	const DeleteRoute6CB&	cb
    );

protected:
    XrlSender* _sender;

private:
    void unmarshall_add_route6(
	const XrlError&	e,
	XrlArgs*	a,
	AddRoute6CB		cb
    );

    void unmarshall_delete_route6(
	const XrlError&	e,
	XrlArgs*	a,
	DeleteRoute6CB		cb
    );

};

#endif /* __XRL_INTERFACES_POLICY_REDIST6_XIF_HH__ */