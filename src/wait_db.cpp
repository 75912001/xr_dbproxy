#include "wait_db.h"

wait_db_t g_wait_db;

PROTO_SEQ wait_db_t::gen_save_key( proto_head_t& proto_head, xr::tcp_info_t* peer )
{
	this->key++;

	client_t client;
	client.head = proto_head;
	client.peer = peer;

	if(!m_proto_head_map.insert(std::make_pair(this->key, client)).second){
		ERROR_LOG("[len:%u, cmd:%#x, seq:%u, ret:%u, key:%u]",
			proto_head.len, proto_head.cmd, proto_head.seq, 
			proto_head.ret, this->key);
		return 0;
	}

	return this->key;
}

bool wait_db_t::find( PROTO_SEQ seq, client_t& rci )
{
	auto it = m_proto_head_map.find(seq);
	if(m_proto_head_map.end() == it){
		ERROR_LOG("can not find [seq:%u]", seq);
		return false;
	}
	rci = it->second;
	m_proto_head_map.erase(seq);
	return true;
}

void wait_db_t::clear_client( int fd )
{
	for (auto it = this->m_proto_head_map.begin(); this->m_proto_head_map.end() != it;){
		if (it->second.peer->fd == fd){
			this->m_proto_head_map.erase(it++);
		} else {
			++it;
		}
	}
}

void wait_db_t::client_time_out()
{
	uint32_t now_sec = g_timer->now_sec();
	for (auto it = this->m_proto_head_map.begin(); this->m_proto_head_map.end() != it;){
		if (now_sec >= it->second.add_time + this->time_out_sec){
			proto_head_t& proto_head = it->second.head;
			ERROR_LOG("[len:%u, cmd:%#x, seq:%u, ret:%u]",
				proto_head.len, proto_head.cmd, proto_head.seq, 
				proto_head.ret);
			proto_head.len = proto_head_t::PROTO_HEAD_LEN;
			proto_head.ret = xr::ECODE_SYS::TIME_OUT;

			g_send_ret(it->second.peer, proto_head);
			this->m_proto_head_map.erase(it++);
		} else {
			++it;
		}
	}
}
