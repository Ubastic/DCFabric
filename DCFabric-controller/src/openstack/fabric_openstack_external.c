/*
 * DCFabric GPL Source Code
 * Copyright (C) 2015, BNC <DCFabric-admin@bnc.org.cn>
 *
 * This file is part of the DCFabric SDN Controller. DCFabric SDN
 * Controller is a free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, , see <http://www.gnu.org/licenses/>.
 */

/*
 * fabric_openstack_external.c
 *
 *  Created on: sep 9, 2015
 *  Author: BNC administrator
 *  E-mail: DCFabric-admin@bnc.org.cn
 *
 *  Modified on: sep 9, 2015
 */
#include "gnflush-types.h"
#include "fabric_openstack_external.h"
#include "mem_pool.h"
#include "../restful-svr/openstack-server.h"
#include "../fabric/fabric_flows.h"


void *g_openstack_external_id = NULL;
void *g_openstack_floating_id = NULL;
void *g_nat_icmp_iden_id=NULL;
void *g_openstack_external_node_id = NULL;
openstack_external_node_p g_openstack_external_list = NULL;
openstack_external_node_p g_openstack_floating_list = NULL;
openstack_external_node_p g_nat_icmp_iden_list=NULL;

UINT1 g_openstack_external_init_flag = 0;

extern void find_openstack_network_by_floating_ip(UINT4 floating_ip,char* network_id);
extern UINT4 g_openstack_on;

external_port_p create_external_port(
        UINT4 external_gateway_ip,
		UINT1* external_gateway_mac,
        UINT4 external_outer_interface_ip,
		UINT1* external_outer_interface_mac,
		UINT8 external_dpid,
		UINT4 external_port,
		char* network_id);
external_floating_ip_p create_floating_ip_p(
		UINT4 fixed_ip,
		UINT4 floating_ip,
		char* port_id,
		char* router_id);
nat_icmp_iden_p create_nat_icmp_iden(
		UINT2 identifier,
		UINT4 host_ip,
		UINT1* host_mac,
		UINT8 sw_dpid,
		UINT4 inport);
void add_openstack_external_to_list(external_port_p epp);
void add_openstack_floating_to_list(external_floating_ip_p efp);
void add_nat_icmp_iden_to_list(nat_icmp_iden_p nii);
openstack_external_node_p create_openstack_external_node(UINT1* data);
void update_external_config(
		UINT4 external_gateway_ip,
		UINT1* external_gateway_mac,
		UINT4 external_outer_interface_ip,
		UINT1* external_outer_interface_mac,
		UINT8 external_dpid,
		UINT4 external_port,
		char* network_id);
void update_external_port(
        UINT4 external_gateway_ip,
		UINT1* external_gateway_mac,
        UINT4 external_outer_interface_ip,
		UINT1* external_outer_interface_mac,
		UINT8 external_dpid,
		UINT4 external_port,
		char* network_id);
void update_floating_ip_list(
		UINT4 fixed_ip,
		UINT4 floating_ip,
		char* port_id,
		char* router_id);
nat_icmp_iden_p update_nat_icmp_iden(
		UINT2 identifier,
		UINT4 host_ip,
		UINT1* host_mac,
		UINT8 sw_dpid,
		UINT4 inport);
void destory_openstack_external();
external_port_p find_openstack_external_by_outer_ip(UINT4 external_outer_interface_ip);
external_floating_ip_p find_external_floating_ip_by_fixed_ip(UINT4 fixed_ip);
external_port_p find_openstack_external_by_gatway_ip(UINT4 external_gateway_ip);
external_port_p find_openstack_external_by_floating_ip(UINT4 external_floating_ip);
external_floating_ip_p find_external_floating_ip_by_floating_ip(UINT4 floating_ip);
external_port_p find_openstack_external_by_outer_mac(UINT1* external_gateway_mac);
nat_icmp_iden_p find_nat_icmp_iden_by_host_ip(UINT4 host_ip);
nat_icmp_iden_p find_nat_icmp_iden_by_host_mac(UINT1* host_mac);
nat_icmp_iden_p find_nat_icmp_iden_by_identifier(UINT2 identifier);
void get_sw_from_dpid(UINT8 dpid,gn_switch_t **sw);
void test(UINT1 type);

void init_openstack_external(){
	if(g_openstack_external_id != NULL){
		mem_destroy(g_openstack_external_id);
	}
	g_openstack_external_id = mem_create(sizeof(external_port_t), OPENSTACK_EXTERNAL_MAX_NUM);

	if(g_openstack_floating_id != NULL){
		mem_destroy(g_openstack_floating_id);
	}
	g_openstack_floating_id = mem_create(sizeof(external_floating_ip), OPENSTACK_FLOATING_MAX_NUM);

	if(g_nat_icmp_iden_id != NULL){
		mem_destroy(g_nat_icmp_iden_id);
	}
	g_nat_icmp_iden_id = mem_create(sizeof(nat_icmp_iden), OPENSTACK_NAT_ICMP_MAX_NUM);

	if(g_openstack_external_node_id != NULL){
		mem_destroy(g_openstack_external_node_id);
	}
	g_openstack_external_node_id = mem_create(sizeof(openstack_external_node), OPENSTACK_EXTERNAL_NODE_MAX_NUM);
	g_openstack_external_list=NULL;
	g_openstack_floating_list=NULL;
	g_nat_icmp_iden_list=NULL;
	return;
}
external_port_p get_external_port_by_out_interface_ip(UINT4 external_outer_interface_ip){
    return find_openstack_external_by_outer_ip(external_outer_interface_ip);
}
external_port_p get_external_port_by_floatip(UINT4 external_floatip){
    return find_openstack_external_by_floating_ip(external_floatip);
}

external_floating_ip_p get_external_floating_ip_by_fixed_ip(UINT4 fixed_ip){
	return find_external_floating_ip_by_fixed_ip(fixed_ip);
}
external_floating_ip_p get_external_floating_ip_by_floating_ip(UINT4 floating_ip){
	return find_external_floating_ip_by_floating_ip(floating_ip);
}

nat_icmp_iden_p get_nat_icmp_iden_by_host_ip(UINT4 host_ip){
	return find_nat_icmp_iden_by_host_ip(host_ip);
}
nat_icmp_iden_p get_nat_icmp_iden_by_host_mac(UINT1* host_mac){
	return find_nat_icmp_iden_by_host_mac(host_mac);
}
nat_icmp_iden_p get_nat_icmp_iden_by_identifier(UINT2 identifier){
	return find_nat_icmp_iden_by_identifier(identifier);
}


void create_external_port_by_rest(
		UINT4 external_gateway_ip,
		UINT1* external_gateway_mac,
        UINT4 external_outer_interface_ip,
		UINT1* external_outer_interface_mac,
		UINT8 external_dpid,
		UINT4 external_port,
		char* network_id){
	update_external_port(
	        external_gateway_ip,
			external_gateway_mac,
	        external_outer_interface_ip,
			external_outer_interface_mac,
	        external_dpid,
	        external_port,
			network_id);
	gn_switch_t *sw = NULL;
	get_sw_from_dpid(external_dpid,&sw);
	if(sw){
		install_fabric_openstack_external_output_flow(sw,external_port,external_gateway_mac,external_outer_interface_ip,1);
		install_fabric_openstack_external_output_flow(sw,external_port,external_gateway_mac,external_outer_interface_ip,2);
	}else{
		LOG_PROC("ERROR", "can not find any sw match external_dpid:[%llu]!\n", external_dpid);
	}
	update_external_config(external_gateway_ip, external_gateway_mac, external_outer_interface_ip,external_outer_interface_mac,
			external_dpid, external_port, network_id);
}

void init_external_flows()
{
	// if openstack on
	if (0 == g_openstack_on) {
		// return
		return;
	}

	// judge whether external flow initialized
	if (0 != g_openstack_external_init_flag) {
		LOG_PROC("TEST", "External:external flow has been initialized!");
		return;
	}

	// define the pointer
	external_port_p epp = NULL;

	// get the external object
	epp = get_external_port();
	gn_switch_t *sw = NULL;

	// if pointer is NULL
	if (NULL == epp) {
		LOG_PROC("ERROR", "External: init exteranl flow failed, external port not exist");
		return ;
	}

	// get sw info
	get_sw_from_dpid(epp->external_dpid,&sw);

	// install the flows
	if (sw) {
		install_fabric_openstack_external_output_flow(sw, epp->external_port, epp->external_gateway_mac, epp->external_outer_interface_ip, 1);
		install_fabric_openstack_external_output_flow(sw, epp->external_port, epp->external_gateway_mac, epp->external_outer_interface_ip, 2);
	}
	else {
		LOG_PROC("ERROR", "can not find any sw match external_dpid:[%llu]!\n", epp->external_dpid);
		return;
	}

	LOG_PROC("INFO", "External: Finish installing external flow");

	// set the flag
	g_openstack_external_init_flag = 1;
}

void update_external_config(
		UINT4 external_gateway_ip,
		UINT1* external_gateway_mac,
		UINT4 external_outer_interface_ip,
		UINT1* external_outer_interface_mac,
		UINT8 external_dpid,
		UINT4 external_port,
		char* network_id){
	// config
	set_value_ip(g_controller_configure, "[openvstack_conf]", "external_gateway_ip",external_gateway_ip);
	set_value_mac(g_controller_configure, "[openvstack_conf]", "external_gateway_mac", external_gateway_mac);
	set_value_ip(g_controller_configure, "[openvstack_conf]", "external_outer_interface_ip",external_outer_interface_ip);
	set_value_mac(g_controller_configure, "[openvstack_conf]", "external_outer_interface_mac",external_outer_interface_mac);
	set_value_int(g_controller_configure, "[openvstack_conf]", "external_dpid",external_dpid);
	set_value_int(g_controller_configure, "[openvstack_conf]", "external_port",external_port);
	set_value(g_controller_configure, "[openvstack_conf]", "external_network_id",network_id);
	g_controller_configure = save_ini(g_controller_configure,CONFIGURE_FILE);
}
void create_floatting_ip_by_rest(
		UINT4 fixed_ip,
        UINT4 floating_ip,
		char* port_id,
        char* router_id){
	update_floating_ip_list(
			fixed_ip,
			floating_ip,
			port_id,
			router_id);
}

nat_icmp_iden_p create_nat_imcp_iden_p(
		UINT2 identifier,
		UINT4 host_ip,
		UINT1* host_mac,
		UINT8 sw_dpid,
		UINT4 inport){
	return update_nat_icmp_iden(
			identifier,
			host_ip,
			host_mac,
			sw_dpid,
			inport);
}
external_port_p create_external_port(
        UINT4 external_gateway_ip,
		UINT1* external_gateway_mac,
        UINT4 external_outer_interface_ip,
		UINT1* external_outer_interface_mac,
		UINT8 external_dpid,
		UINT4 external_port,
		char* network_id){
    external_port_p epp = NULL;
    epp = (external_port_p)mem_get(g_openstack_external_id);
    memset(epp,0,sizeof(external_port_t));
    if(external_gateway_ip && external_gateway_ip!=0){
    	epp->external_gateway_ip=external_gateway_ip;
    }
    if(external_outer_interface_ip && external_outer_interface_ip!=0){
    	epp->external_outer_interface_ip=external_outer_interface_ip;
    }
    if(external_gateway_mac){
    	memcpy(epp->external_gateway_mac, external_gateway_mac, 6);
    }
    if(external_outer_interface_mac){
    	memcpy(epp->external_outer_interface_mac, external_outer_interface_mac, 6);
    }
    if(external_dpid && external_dpid!=0){
    	epp->external_dpid=external_dpid;
    }
    if(external_port && external_port!=0){
    	epp->external_port=external_port;
    }
    if(network_id){
		strcpy(epp->network_id,network_id);
	}
    add_openstack_external_to_list(epp);
    test(1);
    return epp;
}
nat_icmp_iden_p create_nat_icmp_iden(
		UINT2 identifier,
		UINT4 host_ip,
		UINT1* host_mac,
		UINT8 sw_dpid,
		UINT4 inport
		){
	nat_icmp_iden_p nii = NULL;
	nii = (nat_icmp_iden_p)mem_get(g_nat_icmp_iden_id);
	memset(nii,0,sizeof(nat_icmp_iden));
	if(identifier){
		nii->identifier=identifier;
	}
	if(host_ip){
		nii->host_ip=host_ip;
	}
	if(host_mac){
		memcpy(nii->host_mac, host_mac, 6);
	}
	if (sw_dpid) {
		nii->sw_dpid = sw_dpid;
	}
	if (inport) {
		nii->inport = inport;
	}
	add_nat_icmp_iden_to_list(nii);
	test(3);
	return nii;
}

void add_openstack_external_to_list(external_port_p epp){
	external_port_p epp_p = NULL;
    openstack_external_node_p node_p = NULL;
    if(epp == NULL){
       return;
    }
    epp_p = find_openstack_external_by_outer_ip(epp->external_outer_interface_ip);
    if(epp_p != NULL){
       return;
    }
    node_p = create_openstack_external_node((UINT1*)epp);
    node_p->next = g_openstack_external_list;
    g_openstack_external_list = node_p;
}
openstack_external_node_p create_openstack_external_node(UINT1* data){
	openstack_external_node_p ret = NULL;
	ret = (openstack_external_node_p)mem_get(g_openstack_external_node_id);
	memset(ret,0,sizeof(openstack_external_node));
	ret->data = data;
	return ret;
};

external_floating_ip_p create_floating_ip_p(
		UINT4 fixed_ip,
		UINT4 floating_ip,
		char* port_id,
		char* router_id){
	external_floating_ip_p efp = NULL;
	efp = (external_floating_ip_p)mem_get(g_openstack_floating_id);
    memset(efp,0,sizeof(external_floating_ip));
    if(fixed_ip && fixed_ip!=0){
    	efp->fixed_ip = fixed_ip;
    }
    if(floating_ip && floating_ip!=0){
    	efp->floating_ip = floating_ip;
    }
    if(port_id){
    	strcpy(efp->port_id,port_id);
    }
    if(router_id){
    	strcpy(efp->router_id,router_id);
    }
    add_openstack_floating_to_list(efp);
    test(2);
    return efp;
}
void add_openstack_floating_to_list(external_floating_ip_p efp){
	external_floating_ip_p efp_p = NULL;
	openstack_external_node_p node_p = NULL;
	if(efp == NULL){
	   return;
	}
	efp_p = find_external_floating_ip_by_floating_ip(efp->floating_ip);
	if(efp_p != NULL){
	   return;
	}
	node_p = create_openstack_external_node((UINT1*)efp);
	node_p->next = g_openstack_floating_list;
	g_openstack_floating_list = node_p;
}
void add_nat_icmp_iden_to_list(nat_icmp_iden_p nii){
	nat_icmp_iden_p nii_p = NULL;
	openstack_external_node_p node_p = NULL;
	if(nii == NULL){
	   return;
	}
    nii_p = find_nat_icmp_iden_by_host_ip(nii->host_ip);
	if(nii_p != NULL){
	   return;
	}
	node_p = create_openstack_external_node((UINT1*)nii);
	node_p->next = g_nat_icmp_iden_list;
	g_nat_icmp_iden_list = node_p;
}

void update_external_port(
        UINT4 external_gateway_ip,
		UINT1* external_gateway_mac,
	    UINT4 external_outer_interface_ip,
		UINT1* external_outer_interface_mac,
		UINT8 external_dpid,
		UINT4 external_port,
		char* network_id){
    external_port_p epp = NULL;
    epp = find_openstack_external_by_outer_ip(external_outer_interface_ip);
    if(epp == NULL){
        epp = create_external_port(external_gateway_ip,external_gateway_mac,external_outer_interface_ip,
                external_outer_interface_mac,external_dpid,external_port,network_id);
    }else{
        if(external_gateway_ip && external_gateway_ip!=0){
        	epp->external_gateway_ip=external_gateway_ip;
        }
        if(external_outer_interface_ip && external_outer_interface_ip!=0){
        	epp->external_outer_interface_ip=external_outer_interface_ip;
        }
        if(external_gateway_mac){
        	memcpy(epp->external_gateway_mac, external_gateway_mac, 6);
		}
        if(external_outer_interface_mac){
        	memcpy(epp->external_outer_interface_mac, external_outer_interface_mac, 6);
        }
        if(external_dpid && external_dpid!=0){
        	epp->external_dpid=external_dpid;
        }
        if(external_port && external_port!=0){
        	epp->external_port=external_port;
        }
        if(network_id){
			strcpy(epp->network_id,network_id);
		}
    }
}
void update_floating_ip_list(
		UINT4 fixed_ip,
		UINT4 floating_ip,
		char* port_id,
		char* router_id){
	external_floating_ip_p efp = NULL;
	efp = find_external_floating_ip_by_floating_ip(floating_ip);
	if(efp == NULL){
		efp = create_floating_ip_p(fixed_ip,floating_ip,port_id,router_id);
	}else{
		if(fixed_ip && fixed_ip!=0){
			efp->fixed_ip = fixed_ip;
		}
		if(floating_ip && floating_ip!=0){
			efp->floating_ip = floating_ip;
		}
		if(port_id){
			strcpy(efp->port_id,port_id);
		}
		if(router_id){
			strcpy(efp->router_id,router_id);
		}
	}
}
nat_icmp_iden_p update_nat_icmp_iden(
		UINT2 identifier,
		UINT4 host_ip,
		UINT1* host_mac,
		UINT8 sw_dpid,
		UINT4 inport){
	nat_icmp_iden_p nii=NULL;
	nii = find_nat_icmp_iden_by_host_ip(host_ip);
	if(nii == NULL){
		nii=create_nat_icmp_iden(identifier,host_ip,host_mac,sw_dpid,inport);
	}else{
		if(identifier){
			nii->identifier=identifier;
		}
		if(host_ip){
			nii->host_ip=host_ip;
		}
		if(host_mac){
			memcpy(nii->host_mac, host_mac, 6);
		}
		if (sw_dpid) {
			nii->sw_dpid = sw_dpid;
		}
		if (inport) {
			nii->inport = inport;
		}
	}
	return nii;
}
void destory_openstack_external(){
    if(g_openstack_external_id != NULL){
        mem_destroy(g_openstack_external_id);
        g_openstack_external_id = NULL;
    }
    if(g_openstack_floating_id !=NULL){
        mem_destroy(g_openstack_floating_id);
        g_openstack_floating_id = NULL;
    }
    if(g_nat_icmp_iden_id !=NULL){
		mem_destroy(g_nat_icmp_iden_id);
		g_nat_icmp_iden_id = NULL;
	}
    if(g_openstack_external_node_id != NULL){
		mem_destroy(g_openstack_external_node_id);
		g_openstack_external_node_id = NULL;
	}
    g_openstack_external_list = NULL;
    g_openstack_floating_list = NULL;
    g_nat_icmp_iden_list=NULL;
}

external_port_p find_openstack_external_by_outer_ip(UINT4 external_outer_interface_ip){
    external_port_p epp = NULL;
    openstack_external_node_p node_p = g_openstack_external_list;
    while(node_p != NULL){
        epp = (external_port_p)node_p->data;
        if(epp==NULL){
        	return NULL;
        }
        if(epp->external_outer_interface_ip == external_outer_interface_ip){
            return epp;
        }
        node_p = node_p->next;
    }
    return NULL;
};

external_port_p get_external_port_by_host_mac(UINT1* host_mac){
	external_port_p epp = NULL;
	openstack_external_node_p node_p = g_openstack_external_list;

	while(node_p != NULL ){
		epp = (external_port_p)node_p->data;
		if(epp->external_dpid && epp->external_port){
			return epp;
		}
		node_p=node_p->next;
	}
	return NULL;
}

external_port_p get_external_port(){
	external_port_p epp = NULL;
	openstack_external_node_p node_p = g_openstack_external_list;
	while(node_p != NULL ){
		epp = (external_port_p)node_p->data;
		if(epp->external_dpid && epp->external_port){
			return epp;
		}
		node_p=node_p->next;
	}
	return NULL;
}

external_floating_ip_p find_external_floating_ip_by_fixed_ip(UINT4 fixed_ip){
	external_floating_ip_p epp = NULL;
    openstack_external_node_p node_p = g_openstack_floating_list;
    while(node_p != NULL){
        epp = (external_floating_ip_p)node_p->data;
        if(epp==NULL){
        	return NULL;
        }
        if(epp->fixed_ip == fixed_ip){
            return epp;
        }
        node_p = node_p->next;
    }
    return NULL;
};

external_port_p find_openstack_external_by_floating_ip(UINT4 external_floating_ip){
	external_port_p epp = NULL;
	openstack_external_node_p node_p = g_openstack_external_list;
    char network_id[48];
    find_openstack_network_by_floating_ip(external_floating_ip,network_id);
	while(node_p != NULL ){
		epp = (external_port_p)node_p->data;
		if(epp->external_dpid  && epp->external_port){
			if((0 != strlen(network_id)) && (0 == strcmp(epp->network_id,network_id))){
				return epp;
			}
		}
		node_p=node_p->next;
	}
	return NULL;
};
external_port_p find_openstack_external_by_gatway_ip(UINT4 external_gateway_ip){
    external_port_p epp = NULL;
    openstack_external_node_p node_p = g_openstack_external_list;
    while(node_p != NULL){
        epp = (external_port_p)node_p->data;
        if(epp==NULL){
			return NULL;
		}
        if(epp->external_gateway_ip==external_gateway_ip){
            return epp;
        }
        node_p = node_p->next;
    }
    return NULL;
};
external_floating_ip_p find_external_floating_ip_by_floating_ip(UINT4 floating_ip){
	external_floating_ip_p efp = NULL;
    openstack_external_node_p node_p = g_openstack_floating_list;
    while(node_p != NULL){
    	efp = (external_floating_ip_p)node_p->data;
        if(efp==NULL){
        	return NULL;
        }
        if(efp->floating_ip == floating_ip){
            return efp;
        }
        node_p = node_p->next;
    }
    return NULL;
}
external_port_p find_openstack_external_by_outer_mac(UINT1* external_gateway_mac){
    external_port_p epp = NULL;
    openstack_external_node_p node_p = g_openstack_external_list;
    while(node_p != NULL){
        epp = (external_port_p)node_p->data;
        if(epp==NULL){
			return NULL;
		}
        //if(epp->external_gateway_mac==external_gateway_mac){
            return epp;
       // }
        node_p = node_p->next;
    }
    return NULL;
};

nat_icmp_iden_p find_nat_icmp_iden_by_host_ip(UINT4 host_ip){
	nat_icmp_iden_p nii = NULL;
	openstack_external_node_p node_p = g_nat_icmp_iden_list;
	while(node_p != NULL){
		nii = (nat_icmp_iden_p)node_p->data;
		if(nii==NULL){
			return NULL;
		}
		if(nii->host_ip==host_ip){
			return nii;
	    }
		node_p = node_p->next;
	}
	return NULL;
}
nat_icmp_iden_p find_nat_icmp_iden_by_host_mac(UINT1* host_mac){
	nat_icmp_iden_p nii = NULL;
	openstack_external_node_p node_p = g_nat_icmp_iden_list;
	while(node_p != NULL){
		nii = (nat_icmp_iden_p)node_p->data;
		if(nii==NULL){
			return NULL;
		}
		node_p = node_p->next;
	}
	return NULL;
}
nat_icmp_iden_p find_nat_icmp_iden_by_identifier(UINT2 identifier){
	nat_icmp_iden_p nii = NULL;
	openstack_external_node_p node_p = g_nat_icmp_iden_list;
	while(node_p != NULL){
		nii = (nat_icmp_iden_p)node_p->data;
		if(nii==NULL){
			return NULL;
		}
		if(nii->identifier==identifier){
			return nii;
	    }
		node_p = node_p->next;
	}
	return NULL;
}


void test(UINT1 type){
	openstack_external_node_p node = NULL;
	UINT4 num = 0;
	if(type==1){
		node = g_openstack_external_list;
		while(node != NULL){
			external_port_p p = (external_port_p)(node->data);
			if(NULL!=p){
				node = node->next;
				num++;
			}else{
	            printf("node data: %s \n",node->data);
	            node = node->next;
			}
		}
	}else if(type==2){
		node = g_openstack_floating_list;
		while(node != NULL){
			external_floating_ip_p p = (external_floating_ip_p)(node->data);
			if(NULL!=p){
				node = node->next;
				num++;
			}else{
	            printf("node data: %s \n",node->data);
	            node = node->next;
			}
		}
	}else if(type==3){
		node = g_nat_icmp_iden_list;
		while(node != NULL){
			nat_icmp_iden_p p = (nat_icmp_iden_p)(node->data);
			if(NULL!=p){
				node = node->next;
				printf("g_nat_icmp_iden_list : id:[%d]  | ip:[%d]  | \n",p ->identifier,p->host_ip);
				num++;
			}else{
				printf("node data: %s \n",node->data);
				node = node->next;
			}
		}
	}

	LOG_PROC("INFO", "external number: %d \n",num);
	return;
}

void update_floating_ip_mem_info(){
	updateOpenstackFloating();
}
void get_sw_from_dpid(UINT8 dpid,gn_switch_t **sw){
	UINT2 i = 0;
	for(i = 0; i < g_server.max_switch; i++){
		if (g_server.switches[i].dpid==dpid){
			*sw =   &g_server.switches[i];
			return;
		}
	}
}

void read_external_port_config()
{
	// LOG_PROC("INFO", "read external port config");
	// read the configure
	char network_id[48];
	UINT4 external_gateway_ip = 0;
	UINT4 external_outer_interface_ip = 0;
	UINT1 external_outer_interface_mac[6];
	UINT1 external_gateway_mac[6];
	UINT8 external_dpid = 0;
	UINT4 external_port = 0;

	// get configure
	INT1 *value = NULL;
	value = get_value(g_controller_configure, "[openvstack_conf]", "external_network_id");
	while (NULL != value) {
		memset(network_id, 0, 48);
		memcpy(network_id, value, 48);

		value = get_value(g_controller_configure, "[openvstack_conf]", "external_gateway_ip");
		external_gateway_ip = ip2number(value);

		value = get_value(g_controller_configure, "[openvstack_conf]", "external_outer_interface_ip");
		external_outer_interface_ip = ip2number(value);

		value = get_value(g_controller_configure, "[openvstack_conf]", "external_gateway_mac");
		memset(external_gateway_mac, 0, 6);
		macstr2hex(value, external_gateway_mac);

		value = get_value(g_controller_configure, "[openvstack_conf]", "external_outer_interface_mac");
		memset(external_outer_interface_mac, 0, 6);
		macstr2hex(value, external_outer_interface_mac);

		value = get_value(g_controller_configure, "[openvstack_conf]", "external_dpid");
		external_dpid = atoll(value);

		value = get_value(g_controller_configure, "[openvstack_conf]", "external_port");
		external_port = atoi(value);

		update_external_port(external_gateway_ip, external_gateway_mac, external_outer_interface_ip, external_outer_interface_mac,
				external_dpid, external_port, network_id);

		break;
	}
	if (NULL == value) {
		LOG_PROC("INFO", "External: configure is empty!");
	}


}



