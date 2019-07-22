#pragma once

#include <lib_include.h>
#include <lib_xmlparser.h>
#include <lib_util.h>
#include <lib_log.h>
#include <common.h>
#include "db_service.h"

struct route_t
{
	std::vector<db_mgr_t> service_mgr_vec;
	int parser();
	inline service_mgr_t* find(CMD_ID cmd){
		FOREACH(this->service_mgr_vec, it){
			service_mgr_t& r = *it;
			if (r.start <= cmd && cmd <= r.end){
				return &r;
			}
		}
		ERROR_LOG("not find cmd:0x%#x", cmd);
		return NULL;
	}
};

extern route_t g_rotue;
