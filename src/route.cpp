#include <set>

#include <lib_log.h>

#include "route.h"

route_t g_rotue;

namespace {
	int parser_db(xmlNodePtr cur, xr::xml_t& xml, db_mgr_t* db_mgr){
		cur = cur->xmlChildrenNode;
		while(NULL != cur){
			if(!xmlStrcmp(cur->name, (const xmlChar*)"db")){
				db_t db;
				xml.get_prop(cur, "ip", db.ip);
				xml.get_prop(cur, "port", db.port);
				db_mgr->db_vec.push_back(service);
			}
			cur = cur->next;
		}
		return 0;
	}
}

int route_t::parser()
{
	xr::xml_t xml;
	int ret = xml.open("./route.xml");
	if (0 != ret){
		return ret;
	}

	xml.move2children_node();
	while(NULL != xml.node_ptr){
		//取出节点中的内容
		if (!xmlStrcmp(xml.node_ptr->name, (const xmlChar*)"cmd")){
			db_mgr_t service_mgr;
			xmlNodePtr cur = xml.node_ptr;

			std::string str_start;
			std::string str_end;

			xml.get_prop(cur, "start", str_start);
			db_mgr.start = ::strtoul(str_start.c_str(), 0, 16);

			xml.get_prop(cur, "end", str_end);
			db_mgr.end = ::strtoul(str_end.c_str(), 0, 16);

			if (db_mgr.start > db_mgr.end){
				assert(0);
				return FAIL;
			}

			parser_ser(cur, xml, &db_mgr);
			g_rotue_t.db_mgr.push_back(db_mgr);
		}
		xml.move2next_node();
	}

	return 0;
}
