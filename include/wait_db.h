//等待db的数据
#pragma once

#include <xr_timer.h>
#include <proto_header.h>

struct client_t 
{
    proto_head_t head;      //客户端协议包头
	xr::tcp_peer_t* peer; 	//客户端链接信息
	time_t add_time;        
	client_t(){
		this->peer_info = NULL;
        this->add_time = g_timer->now_sec();
	}
	inline int send_ret(xr::tcp_peer_t* peer, proto_head_t& proto_head, PROTO_RET ret){
    	proto_head_t err_out = proto_head;
		err_out.ret = ret;
    	return xr_server::s2peer(peer, (void*)err_out.len, proto_head_t::PROTO_HEAD_LEN);
	}	
};

struct wait_db_t
{
public:
	PROTO_SEQ key;
	time_t time_out_sec;
	wait_db_t(){
		this->key = 0;
		this->time_out_sec = 30;
	}

    //生成并保存KEY
    PROTO_SEQ gen_save_key(proto_head_t& proto_head, xr::tcp_peer_t* peer);
	bool find(PROTO_SEQ seq, client_t& rci);

	//客户端断开连接时清理对应未返回的等待协议
	void clear_client(int fd);
	//客户端请求超时,返回超时
	void client_time_out();
private:
	std::map<PROTO_SEQ, client_t> m_proto_head_map;//key:新的序号.val:原seq
};

extern wait_db_t g_wait_db;
