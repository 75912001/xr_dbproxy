#pragma once

#include <xr_include.h>
#include <xr_tcp.h>
#include <proto_header.h>

struct dbproxy_t
{
	dbproxy_t();
	int handle_client(xr::tcp_peer_t *peer, const void *data, uint32_t len);
	int handle_server(xr::tcp_peer_t *peer, const void *data, uint32_t len);
	xr_server::proto_head_t head;
	int ret;
};

extern dbproxy_t g_dbproxy;