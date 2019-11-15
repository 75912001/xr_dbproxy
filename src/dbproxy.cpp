
#include "dbproxy.h"
#include "db.h"
#include "route.h"

dbproxy_t g_dbproxy;

int dbproxy_t::handle_client( xr::tcp_peer_t* peer, const void* data, uint32_t len )
{
    this->head.unpack(data);

    db_mgr_t* db_mgr = g_rotue.find(this->head.cmd);
	if (NULL == db_mgr){
		return xr_server::send_ret(peer, this->head, xr::ERROR::UNDEFINED_CMD);
	}
	this->ret = db_mgr->send(peer, this->head.uid, data, len);
	if (this->ret < 0){
		return xr_server::send_ret(peer, this->head, this->ret);
	}
	return SUCC;
}

int dbproxy_t::handle_server( xr::tcp_peer_t* peer, const void* data, uint32_t len )
{
    this->head.unpack(data);

	client_t client;
    if (!g_wait_db.find(this->head.seq, client)){
        ERROR_LOG("seq can not find [cmd:%#x, seq:%u]", this->head.cmd, this->head.seq);
        return FAIL;
    } else {
        ::memcpy((char*)data + sizeof(xr_server::PROTO_LEN) + sizeof(xr_server::PROTO_CMD) + sizeof(xr_server::PROTO_UID), &client.head.seq, sizeof(xr_server::PROTO_SEQ));
        xr_server::s2peer(client.peer, data, len);		
    }
	
	return SUCC;
}

dbproxy_t::dbproxy_t()
{
	this->ret = 0;
}