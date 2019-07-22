//定时器

#pragma once

#include <xr_timer.h>

struct dbproxy_timer_t : public xr::timer_event_t
{
	dbproxy_timer_t();
	//定时处理超时协议
	static int clear(void* owner, void* data);
};

extern dbproxy_timer_t* g_dbproxy_timer;