//定时器

#pragma once

#include <xr_timer.h>

class timer_t  : public xr::timer_event_t
{
public:
	timer_t();
	//定时处理超时协议
	static int clear(void* owner, void* data);
};

extern timer_t* g_dbproxy_timer;
