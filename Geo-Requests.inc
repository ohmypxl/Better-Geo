/*
 * @Name: Geo-Requests
 * @Version: 1.0.0
 * @Author: ohmypxl
 * @Date: 13 August, 2023
 * 
 * Copyright (C) 2023  ohmypxl/Kirima
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#if defined _geo_requests_included
	#endinput
#endif

#define _geo_requests_included

#include <a_samp>
#include <requests>

#if !defined MAX_GEO_REQUESTS
	#define MAX_GEO_REQUESTS (128)
#endif

#define INVALID_GEO_RESULTS GeoResults:cellmin

stock bool:IsValidGeoResults(GeoResults:results)
{
	return (results != INVALID_GEO_RESULTS);
}

stock bool:Geo_GetCountryName(GeoResults:results, string:output[], len = sizeof(output))
{
	if (results == INVALID_GEO_RESULTS)
	{
		return false;
	}

	JsonGetString(Node:_:results, "country", output, len);
	return true;
}

stock bool:Geo_GetCountryCode(GeoResults:results, string:output[], len = sizeof(output))
{
	if (results == INVALID_GEO_RESULTS)
	{
		return false;
	}

	JsonGetString(Node:_:results, "countryCode", output, len);
	return true;
}

stock bool:Geo_GetRegionName(GeoResults:results, string:output[], len = sizeof(output))
{
	if (results == INVALID_GEO_RESULTS)
	{
		return false;
	}

	JsonGetString(Node:_:results, "regionName", output, len);
	return true;
}

stock bool:Geo_GetRegionCode(GeoResults:results, string:output[], len = sizeof(output))
{
	if (results == INVALID_GEO_RESULTS)
	{
		return false;
	}

	JsonGetString(Node:_:results, "regionCode", output, len);
	return true;
}

stock bool:Geo_GetCityName(GeoResults:results, string:output[], len = sizeof(output))
{
	if (results == INVALID_GEO_RESULTS)
	{
		return false;
	}

	JsonGetString(Node:_:results, "city", output, len);
	return true;
}

stock bool:Geo_GetIspName(GeoResults:results, string:output[], len = sizeof(output))
{
	if (results == INVALID_GEO_RESULTS)
	{
		return false;
	}

	JsonGetString(Node:_:results, "isp", output, len);
	return true;
}

stock bool:Geo_GetIpResults(GeoResults:results, string:output[], len = sizeof(output))
{
	if (results == INVALID_GEO_RESULTS)
	{
		return false;
	}

	JsonGetString(Node:_:results, "query", output, len);
	return true;
}

stock bool:Geo_IsProxy(GeoResults:results)
{
	if (results == INVALID_GEO_RESULTS)
	{
		return false;
	}

	new bool:ret;
	JsonGetBool(Node:_:results, "proxy", ret);
	return ret;
}

stock bool:Geo_IsMobileConnection(GeoResults:results)
{
	if (results == INVALID_GEO_RESULTS)
	{
		return false;
	}

	new bool:ret;
	JsonGetBool(Node:_:results, "mobile", ret);
	return ret;
}


#if defined _INC_y_inline
static
	Func:Geo_g_sInlineCallbacks[MAX_GEO_REQUESTS]<i> = {Func:0<i>, ...};

stock bool:Geo_InlineCheckIp(const string:ipAddress[], Func:callback<i>)
{
	new string:queryBuilder[256 + 1];
	format(queryBuilder, sizeof(queryBuilder), "/json/%s?fields=status,country,countryCode,regionName,region,city,isp,query", ipAddress);
	new RequestsClient:client = RequestsClient("http://ip-api.com", RequestHeaders());

	if (!IsValidRequestsClient(client))
	{
		return false;
	}

	new Request:jsonRequest = RequestJSON(
		client,
		queryBuilder,
		HTTP_METHOD_GET,
		"Geo_OnInlineResults",
		.headers = RequestHeaders()
	);

	if (!IsValidRequest(jsonRequest) || jsonRequest >= Request:MAX_GEO_REQUESTS)
	{
		return false;
	}

	Indirect_Claim(callback);
	Geo_g_sInlineCallbacks[_:jsonRequest] = callback;
	return true;
}

stock bool:Geo_InlineCheckPlayer(playerid, Func:callback<i>)
{
	new ipAddress[16 + 1];
	GetPlayerIp(playerid, queryBuilder, 16);

	return Geo_InlineCheckIp(ipAddress, callback);
}

forward Geo_OnInlineResults(Request:id, E_HTTP_STATUS:status, Node:node);
public Geo_OnInlineResults(Request:id, E_HTTP_STATUS:status, Node:node)
{
	new geoStatus[16];
	JsonGetString(node, "status", geoStatus);

	new Func:callback<i> = Geo_g_sInlineCallbacks[_:id], ret = _:node;
	Geo_g_sInlineCallbacks[_:id] = Func:0<i>;

	if (strequal(geoStatus, "fail") || status != HTTP_STATUS_OK)
	{
		ret = cellmin;
	}

	@.callback(ret);
	Indirect_Release(callback);
	return 1;
}
#endif

static
	string:Geo_g_sCallbacks[MAX_GEO_REQUESTS][32 + 1];

stock bool:Geo_CheckIp(const string:ipAddress[], const string:callback[])
{
	new string:queryBuilder[256 + 1];
	format(queryBuilder, sizeof(queryBuilder), "/json/%s?fields=status,country,countryCode,regionName,region,city,isp,query", ipAddress);
	new RequestsClient:client = RequestsClient("http://ip-api.com", RequestHeaders());

	if (!IsValidRequestsClient(client))
	{
		return false;
	}

	new Request:jsonRequest = RequestJSON(
		client,
		queryBuilder,
		HTTP_METHOD_GET,
		"Geo_OnResults",
		.headers = RequestHeaders()
	);

	if (!IsValidRequest(jsonRequest) || jsonRequest >= Request:MAX_GEO_REQUESTS)
	{
		return false;
	}

	strcopy(Geo_g_sCallbacks[_:jsonRequest], callback, 32);
	return true;
}

stock bool:Geo_CheckPlayer(playerid, const string:callback[])
{
	new ipAddress[16 + 1];
	GetPlayerIp(playerid, queryBuilder, 16);

	return Geo_CheckIp(ipAddress, callback);
}

forward Geo_OnResults(Request:id, E_HTTP_STATUS:status, Node:node);
public Geo_OnResults(Request:id, E_HTTP_STATUS:status, Node:node)
{
	new geoStatus[16];
	JsonGetString(node, "status", geoStatus);

	new ret = _:node;
	if (strequal(geoStatus, "fail") || status != HTTP_STATUS_OK)
	{
		ret = cellmin;
	}

	CallRemoteFunction(Geo_g_sCallbacks[_:id], "i", _:ret);
	return 1;
}
