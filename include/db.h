//db

#pragma once

#include <xr_tcp.h>
#include <if.h>
#include <xr_ecode.h>
#include "dbproxy.h"
#include "wait_db.h"

//服务数据	
struct db_t
{
	std::string ip;
	uint16_t port;
	xr::tcp_peer_t* peer;
	db_t(){
		this->init();
	}
	void init(){
		this->peer = NULL;
		this->ip.clear();
		this->port = 0;
	}
	inline int send(xr::tcp_peer_t* cb_peer, const void* data, uint32_t len){
		if (NULL == this->peer){
			//连接
			this->peer = xr_server::connect(this->ip, this->port);
			if (NULL == this->peer){
				return xr::ECODE_SYS::CONNECT_FAIL;
			}
		}
		if (1 == g_dbproxy.head.cmd%2){//偶数消息不需要回包,奇数消息需要回包
			//创建新KEY&&保存
			PROTO_SEQ key = g_wait_db.gen_save_key(g_dbproxy.head, cb_peer);

			//修改头&&发送
			::memcpy((char*)data + sizeof(PROTO_LEN), &key, sizeof(key));
		}
		return xr_server::s2peer(this->peer, data, len);
	}
};

//db管理器	
struct db_mgr_t
{
	PROTO_CMD start;
	PROTO_CMD end;
	service_mgr_t(){
		this->start = 0;
		this->end = 0;
	}

	inline int send(xr::tcp_peer_t* peer, PROTO_UID uid, const void* data, uint32_t len){
		uint32_t s_cnt = this->db_vec.size();
		uint32_t idx = uid%s_cnt;
		return this->db_vec[idx].send(peer, data, len);
	}
	std::vector<db_t> db_vec;
};