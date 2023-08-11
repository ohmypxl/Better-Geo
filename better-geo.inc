
#if defined _better_geo_included
	#endinput
#endif

#define _better_geo_included

#include <a_samp>
#include <requests>

#include <YSI_Coding\y_inline>

#if !defined MAX_GEO_REQUESTS
	#define MAX_GEO_REQUESTS (128)
#endif

#define INVALID_GEO_RESULTS GeoResults:cellmin

static
	Func:Geo_g_sCallbacks[MAX_GEO_REQUESTS]<i>;

enum GeoCheckOption(<<=1)
{
	OPTION_COUNTRY_NAME = 1, 
	OPTION_COUNTRY_CODE,
	OPTION_REGION_NAME,
	OPTION_REGION_CODE,
	OPTION_CITY_NAME,
	OPTION_ISP_NAME,
	OPTION_IP_RESULTS,
	OPTION_DEFAULT = OPTION_COUNTRY_NAME | OPTION_REGION_NAME | OPTION_CITY_NAME | OPTION_ISP_NAME | OPTION_IP_RESULTS
};

stock bool:Geo_CheckPlayer(playerid, Func:cb<i>, GeoCheckOption:option = OPTION_DEFAULT)
{
	new string:queryBuilder[64];
	GetPlayerIp(playerid, queryBuilder, 16);
	format(queryBuilder, sizeof(queryBuilder), "json/%s/?", queryBuilder);

	if (option & OPTION_COUNTRY_NAME)
	{
		strcat(queryBuilder, "country,");
	}
	if (option & OPTION_COUNTRY_CODE)
	{
		strcat(queryBuilder, "countryCode,");
	}
	if (option & OPTION_REGION_NAME)
	{
		strcat(queryBuilder, "regionName,");
	}
	if (option & OPTION_REGION_CODE)
	{
		strcat(queryBuilder, "region,");
	}
	if (option & OPTION_CITY_NAME)
	{
		strcat(queryBuilder, "city,");
	}
	if (option & OPTION_ISP_NAME)
	{
		strcat(queryBuilder, "isp,");
	}
	if (option & OPTION_IP_RESULTS)
	{
		strcat(queryBuilder, "query,");
	}

	queryBuilder[strlen(queryBuilder) - 1] = EOS; // remove extra comma
	printf("The query builder results: %s", queryBuilder);
	
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

	Indirect_Claim(cb);
	Geo_g_sCallbacks[_:jsonRequest] = cb;
	return true;
}

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

stock bool:Geo_GetISPName(GeoResults:results, string:output[], len = sizeof(output))
{
	if (results == INVALID_GEO_RESULTS)
	{
		return false;
	}

	JsonGetString(Node:_:results, "isp", output, len);
	return true;
}

stock bool:Geo_GetIPAddress(GeoResults:results, string:output[], len = sizeof(output))
{
	if (results == INVALID_GEO_RESULTS)
	{
		return false;
	}

	JsonGetString(Node:_:results, "query", output, len);
	return true;
}

forward Geo_OnResults(Request:id, E_HTTP_STATUS:status, Node:node);
public Geo_OnResults(Request:id, E_HTTP_STATUS:status, Node:node)
{
	new geoStatus[16];
	JsonGetString(node, "status", geoStatus);
	printf("Getting the status code: %s | %d", geoStatus, _:status);

	new Func:cb<i> = Geo_g_sCallbacks[_:id], ret = _:node;
	Geo_g_sCallbacks[_:id] = Func:0<i>;
	print("Prepare the calling...");

	if (strequal(geoStatus, "fail") || status != HTTP_STATUS_OK)
	{
		print("Oh nyo tuns out it's fail/not 200");
		ret = cellmin;
	}

	@.cb(_:ret);
	Indirect_Release(cb);
	return 1;
}
