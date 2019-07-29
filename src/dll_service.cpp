#include <xr_include.h>
#include <xr_log.h>
#include <xr_util.h>
#include <xr_timer.h>
#include <config.h>
#include <xr_tcp.h>
#include "route.h"
#include "timer.h"


#include "wait_db.h"
#include "dbproxy.h"

extern "C" int on_init()
{
	if(xr_server::is_parent()){
		DEBUG_LOG("======parent start======");
	}else{
		DEBUG_LOG("======child start======");
		if (0 != g_rotue.parser()){
			return FAIL;
		}
		std::string time_out_sec = xr_server::g_config->get_val_str("db", "time_out_sec");
		if (!time_out_sec.empty()){
			uint32_t time_out = 0;
			xr::string2T(time_out_sec, time_out);
			g_wait_db.time_out_sec = time_out;
		}
		g_dbproxy_timer = new dbproxy_timer_t;
		g_dbproxy_timer->add_sec(dbproxy_timer_t::clear, NULL, xr::g_timer->now_sec() + 3);
	}
	return 0;
}

extern "C" int on_fini()
{
	if (xr_server::is_parent()) {
		DEBUG_LOG("======parent done======");
	}else{
		DEBUG_LOG("======child done======");
		SAFE_DELETE(g_dbproxy_timer);
	}
	return 0;
}

extern "C" void on_events()
{
	if (!xr_server::is_parent()){
		xr::g_timer->scan();
	}
}

extern "C" void on_cli_conn(xr::tcp_peer_t* peer)
{
    DEBUG_LOG("[fd:%d]", peer->fd);
}

extern "C" int on_cli_pkg(xr::tcp_peer_t* peer, const void* data, uint32_t len)
{
	g_dbproxy.handle_client(peer, data, len);
	return 0;
}

extern "C" void on_srv_pkg(xr::tcp_peer_t* peer, const void* data, uint32_t len)
{
	g_dbproxy.handle_server(peer, data, len);
}

extern "C" void on_cli_conn_closed(int fd)
{
	g_wait_db.clear_client(fd);
}

extern "C" void on_svr_conn_closed(int fd)
{
	FOREACH(g_rotue.db_mgr_vec, it){
		db_mgr_t& db_mgr = *it;
		FOREACH(db_mgr.db_vec, it_2){
			db_t& db = *it_2;
			if (NULL == db.peer){
				continue;
			}
			
			if (db.peer->fd == fd){
				db.peer = NULL;
				break;
			}
		}
	}
}

extern "C" int on_get_pkg_len(xr::tcp_peer_t* peer,
        const void* data, uint32_t len)
{
    if (len < xr_server::proto_head_t::PROTO_HEAD_LEN){
        return 0;
    }

	char* c = (char*)data;
	xr_server::PROTO_LEN pkg_len = (xr_server::PROTO_LEN)(*(xr_server::PROTO_LEN*)c);

    if (pkg_len < xr_server::proto_head_t::PROTO_HEAD_LEN || pkg_len >= xr_server::g_config->page_size_max){
        ERROR_LOG("pkg len error |%u", pkg_len);
        return xr::ECODE::DISCONNECT_PEER;
    }

    if (len < pkg_len){
        return 0;
    }

	return pkg_len;
}

extern "C" void	on_mcast_pkg(const void* data, int len)
{
}

extern "C"  void on_addr_mcast_pkg(uint32_t id, const char* name, const char* ip, uint16_t port, const char* data, int flag)
{
}

extern "C" void on_udp_pkg(int fd, const void* data, int len ,struct sockaddr_in* from, socklen_t fromlen)
{
}