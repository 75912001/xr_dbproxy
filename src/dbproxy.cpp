
#include "dbproxy.h"


dbproxy_t g_dbproxy;

int dbproxy_t::handle_client( xr::tcp_peer_t* peer, const void* data, uint32_t len )
{
    this->head.unpack(data);

    service_mgr_t* service_mgr = g_rotue.find(head.cmd);
	if (NULL == service_mgr){
		return client_t::send_ret(peer, this->head, xr::ECODE_SYS::UNDEFINED_CMD);
	}
	this->ret = service_mgr->send(peer, this->head.uid, data, len);
	if (this->ret < 0){
		return client_t::send_ret(peer, this->head, this->ret);
	}
	return SUCC;
}

int dbproxy_t::handle_server( xr::tcp_peer_t* peer, const void* data, uint32_t len )
{
    this->head.unpack(data);

	//处理DB的返回包
	client_t client;
    if (!g_wait_db.find(this->head.seq, client)){
        ERROR_LOG("seq can not find [cmd:%#x, seq:%u]", this->head.cmd, this->head.seq);
        return FAIL;
    } else {
        ::memcpy((char*)data + sizeof(PROTO_LEN), &client.head.seq, sizeof(PROTO_SEQ));
        xr_server::s2peer(client.peer, data, len);		
    }
	
	return SUCC;
}

dbproxy_t::dbproxy_t()
{
	this->ret = 0;
}

