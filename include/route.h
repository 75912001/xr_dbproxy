#pragma once

#include <xr_include.h>
#include <xr_xml.h>
#include <xr_util.h>
#include <xr_log.h>
#include "db.h"

struct route_t
{
	int parser();
	std::vector<db_mgr_t> db_mgr_vec;
	inline db_mgr_t *find(xr_server::PROTO_CMD cmd)
	{
		FOREACH(this->db_mgr_vec, it)
		{
			db_mgr_t &r = *it;
			if (r.start <= cmd && cmd <= r.end)
			{
				return &r;
			}
		}
		ERROR_LOG("not find cmd:0x%#x", cmd);
		return NULL;
	}
};

extern route_t g_rotue;