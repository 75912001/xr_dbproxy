
#include "timer.h"
#include "wait_db.h"

timer_t* g_dbproxy_timer;

int timer_t::clear( void* owner, void* data )
{
	//处理超时协议返回
	g_wait_db.clis_req_time_out();
	g_dbproxy_timer->add_sec(timer_t::clear, NULL, g_dbproxy_timer->now_sec() + 3);
	return 0;
}

timer_t::timer_t() : xr::timer_event_t(g_timer)
{
}
