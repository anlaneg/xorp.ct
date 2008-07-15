// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-

// Copyright (c) 2007-2008 International Computer Science Institute
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software")
// to deal in the Software without restriction, subject to the conditions
// listed in the XORP LICENSE file. These conditions include: you must
// preserve this copyright notice, and you cannot mention the copyright
// holders in advertising related to the Software without their permission.
// The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
// notice is a summary of the XORP LICENSE file; the license in that file is
// legally binding.

#ident "$XORP: xorp/fea/xrl_fea_node.cc,v 1.17 2008/04/11 00:19:33 pavlin Exp $"


//
// FEA (Forwarding Engine Abstraction) with XRL front-end node implementation.
//


#include "fea_module.h"

#include "libxorp/xorp.h"
#include "libxorp/xlog.h"
#include "libxorp/debug.h"

#include "xrl_fea_node.hh"



#include "libxipc/xrl_std_router.hh"

#include "cli/xrl_cli_node.hh"

#include "fibconfig.hh"
#include "ifconfig.hh"
#include "libfeaclient_bridge.hh"
#include "xrl_mfea_node.hh"


XrlFeaNode::XrlFeaNode(EventLoop& eventloop, const string& xrl_fea_targetname,
		       const string& xrl_finder_targetname,
		       const string& finder_hostname, uint16_t finder_port,
		       bool is_dummy)
    : _eventloop(eventloop),
      _xrl_router(eventloop, xrl_fea_targetname.c_str(),
		  finder_hostname.c_str(), finder_port),
      _xrl_fea_io(eventloop, _xrl_router, xrl_finder_targetname),
      _fea_node(eventloop, _xrl_fea_io, is_dummy),
      _lib_fea_client_bridge(_xrl_router, _fea_node.ifconfig().ifconfig_update_replicator()),
      _xrl_fib_client_manager(_fea_node.fibconfig(), _xrl_router),
      _xrl_io_link_manager(_fea_node.io_link_manager(), _xrl_router),
      _xrl_io_ip_manager(_fea_node.io_ip_manager(), _xrl_router),
      _xrl_io_tcpudp_manager(_fea_node.io_tcpudp_manager(), _xrl_router),
      _cli_node4(AF_INET, XORP_MODULE_CLI, _eventloop),
      _xrl_cli_node(_eventloop, _cli_node4.module_name(), finder_hostname,
		    finder_port,
		    xrl_finder_targetname,
		    _cli_node4),
      _xrl_mfea_node4(_fea_node, AF_INET, XORP_MODULE_MFEA, _eventloop,
		      xorp_module_name(AF_INET, XORP_MODULE_MFEA),
		      finder_hostname, finder_port,
		      xrl_finder_targetname),
#ifdef HAVE_IPV6_MULTICAST
      _xrl_mfea_node6(_fea_node, AF_INET6, XORP_MODULE_MFEA, _eventloop,
		      xorp_module_name(AF_INET6, XORP_MODULE_MFEA),
		      finder_hostname, finder_port,
		      xrl_finder_targetname),
#endif
      _xrl_fea_target(_eventloop, _fea_node, _xrl_router, _fea_node.profile(),
		      _xrl_fib_client_manager, _lib_fea_client_bridge),
      _xrl_finder_targetname(xrl_finder_targetname)
{
    _cli_node4.set_cli_port(0);		// XXX: disable CLI telnet access
}

XrlFeaNode::~XrlFeaNode()
{
    shutdown();
}

int
XrlFeaNode::startup()
{
    wait_until_xrl_router_is_ready(eventloop(), xrl_router());

    if (! fea_node().is_dummy()) {
	// XXX: The multicast-related code doesn't have dummy mode (yet)
	wait_until_xrl_router_is_ready(eventloop(),
				       _xrl_cli_node.xrl_router());
	wait_until_xrl_router_is_ready(eventloop(),
				       _xrl_mfea_node4.xrl_router());
#ifdef HAVE_IPV6_MULTICAST
	wait_until_xrl_router_is_ready(eventloop(),
				       _xrl_mfea_node6.xrl_router());
#endif
    }

    xrl_fea_io().startup();
    fea_node().startup();
    xrl_fea_target().startup();

    if (! fea_node().is_dummy()) {
	// XXX: temporary enable the built-in CLI access
	_xrl_cli_node.enable_cli();
	_xrl_cli_node.start_cli();
	_xrl_mfea_node4.enable_mfea();
	// _xrl_mfea_node4.startup();
	_xrl_mfea_node4.enable_cli();
	_xrl_mfea_node4.start_cli();
#ifdef HAVE_IPV6_MULTICAST
	_xrl_mfea_node6.enable_mfea();
	// _xrl_mfea_node6.startup();
	_xrl_mfea_node6.enable_cli();
	_xrl_mfea_node6.start_cli();
#endif
    }

    return (XORP_OK);
}

int
XrlFeaNode::shutdown()
{
    xrl_fea_io().shutdown();
    fea_node().shutdown();
    xrl_fea_target().shutdown();

    if (! fea_node().is_dummy()) {
	_xrl_mfea_node4.shutdown();
#ifdef HAVE_IPV6_MULTICAST
	_xrl_mfea_node6.shutdown();
#endif
    }

    return (XORP_OK);
}

bool
XrlFeaNode::is_running() const
{
    if (_xrl_fea_io.is_running())
	return (true);
    if (_fea_node.is_running())
	return (true);
    if (_xrl_fea_target.is_running())
	return (true);

    if (! _fea_node.is_dummy()) {
	if (! _xrl_mfea_node4.MfeaNode::is_down())
	    return (true);
#ifdef HAVE_IPV6_MULTICAST
	if (! _xrl_mfea_node6.MfeaNode::is_down())
	    return (true);
#endif
    }

    //
    // Test whether all XRL operations have completed
    //

    if (! _fea_node.is_dummy()) {
	if (_xrl_cli_node.xrl_router().pending())
	    return (true);
	if (_xrl_mfea_node4.xrl_router().pending())
	    return (true);
#ifdef HAVE_IPV6_MULTICAST
	if (_xrl_mfea_node6.xrl_router().pending())
	    return (true);
#endif
    }

    if (_xrl_router.pending())
	return (true);

    return (false);
}

bool
XrlFeaNode::is_shutdown_received() const
{
    return (_xrl_fea_target.is_shutdown_received());
}