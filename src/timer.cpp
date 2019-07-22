
#include "timer.h"
#include "wait_db.h"

dbproxy_timer_t* g_dbproxy_timer;

int dbproxy_timer_t::clear( void* owner, void* data )
{
	//处理超时协议返回
	g_wait_db.client_time_out();
	g_dbproxy_timer->add_sec(dbproxy_timer_t::clear, NULL, xr::g_timer->now_sec() + 3);
	return 0;
}

dbproxy_timer_t::dbproxy_timer_t() : xr::timer_event_t(xr::g_timer)
{
}