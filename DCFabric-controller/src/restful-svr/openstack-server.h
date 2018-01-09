/*
 * GNFlush SDN Controller GPL Source Code
 * Copyright (C) 2015, Greenet <greenet@greenet.net.cn>
 *
 * This file is part of the GNFlush SDN Controller. GNFlush SDN
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

/******************************************************************************
*                                                                             *
*   File Name   : openstack-erver.h           *
*   Author      : yanglei           *
*   Create Date : 2015-6-18           *
*   Version     : 1.0           *
*   Function    : .           *
*                                                                             *
******************************************************************************/



#ifndef INC_OPENSTACK_SERVER_H_
#define INC_OPENSTACK_SERVER_H_


enum EOpenStack_GetType
{
  EOPENSTACK_GET_NORMAL,
  EOPENSTACK_GET_LOADBALANCE,
  EOPENSTACK_GET_PORTFORWARD,
  EOPENSTACK_GET_CLBLOADBALANCE,
  EOPENSTACK_GET_QOS
};

typedef INT4 (*openstack_dataparse_cb)(char *string, void* param);

typedef struct openstack_parse
{
	char *stringType;
	char eGetType;
	openstack_dataparse_cb callback_func;
}stopenstack_parse, *pstopenstack_parse;

int getNewTokenId(char *ip,char *tenantName,char *username,char *password);
INT4 getOpenstackInfo(char *ip,char *url,int port,char *string, void* param, enum EOpenStack_GetType getType);
void initOpenstackFabric();
INT4 updateOpenstackFloating();
INT4 reload_security_group_info();
INT4 reoad_lbaas_info();
INT4 reload_net_info();
void reload_routers();
INT4 reload_clbs_info();
void reload_port_forward();

#endif