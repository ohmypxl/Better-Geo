
#include <a_samp>
#include <requests>

#include <YSI_Coding\y_inline>

#if !defined MAX_GEO_REQUESTS
	#define MAX_GEO_REQUESTS (128)
#endif

#define INVALID_GEO_RESULTS GeoResults:cellmin

enum e_GEO_RESULTS
{
	GEO_RESULTS_COUNTRY[64], 
	GEO_RESULTS_COUNTRY_CODE[3],
	GEO_RESULTS_REGION[32],
	GEO_RESULTS_REGION_NAME[32],
	GEO_RESULTS_CITY[32],
	GEO_RESULTS_ISP[32],
	GEO_RESULTS_IP[32],
};

static
	Func:Geo_g_sCallbacks[MAX_GEO_REQUESTS]<i>,
	string:Geo_g_sResults[MAX_GEO_REQUESTS][e_GEO_RESULTS];

enum GeoCheckOption(<<=1)
{
	GEO_OPTION_CHECK_COUNTRY = 1, 
	GEO_OPTION_CHECK_COUNTRY_CODE,
	GEO_OPTION_CHECK_REGION,
	GEO_OPTION_CHECK_REGION_NAME,
	GEO_OPTION_CHECK_CITY,
	GEO_OPTION_CHECK_ISP,
	GEO_OPTION_CHECK_IP_RESULTS,
	GEO_OPTION_DEFAULT = GEO_OPTION_CHECK_COUNTRY | GEO_OPTION_CHECK_REGION_NAME | GEO_OPTION_CHECK_CITY | GEO_OPTION_CHECK_ISP | GEO_OPTION_CHECK_IP_RESULTS
};

stock bool:Geo_CheckPlayer(playerid, Func:cb<i>, GeoCheckOption:option = GEO_OPTION_DEFAULT)
{`
	new string:queryBuilder[64];
	GetPlayerIp(playerid, queryBuilder, 16);
	format(queryBuilder, sizeof(queryBuilder), "json/%s/?", queryBuilder);

	if (option & OPTION_CHECK_COUNTRY)
	{
		strcat(queryBuilder, "country,");
	}
	if (option & OPTION_CHECK_COUNTRY_CODE)
	{
		strcat(queryBuilder, "countryCode,");
	}
	if (option & OPTION_CHECK_REGION)
	{
		strcat(queryBuilder, "region,");
	}
	if (option & OPTION_CHECK_REGION_NAME)
	{
		strcat(queryBuilder, "regionName,");
	}
	if (option & OPTION_CHECK_CITY)
	{
		strcat(queryBuilder, "city,");
	}
	if (option & OPTION_CHECK_ISP)
	{
		strcat(queryBuilder, "isp,");
	}
	if (option & OPTION_CHECK_IP_RESULTS)
	{
		strcat(queryBuilder, "query,");
	}

	queryBuilder[strlen(queryBuilder) - 1] = EOS; // remove extra comma
	new RequestsClient:client = RequestsClient("http://ip-api.com", RequestHeaders());

	if (!IsValidRequestsClient(client))
	{
		return false;
	}

	new Request:jsonRequest = RequestJson(
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

stock bool:Geo_GetCountryName(GeoResults:results, string:output[], len = sizeof(output))
{
	if (results == INVALID_GEO_RESULTS)
	{
		return false;
	}

	strcopy(output, Geo_g_sResults[_:results][GEO_RESULTS_COUNTRY], len);
	return true;
}

stock bool:Geo_GetCountryCode(GeoResults:results, string:output[], len = sizeof(output))
{
	if (results == INVALID_GEO_RESULTS)
	{
		return false;
	}

	strcopy(output, Geo_g_sResults[_:results][GEO_RESULTS_COUNTRY_CODE], len);
	return true;
}

stock bool:Geo_GetRegionName(GeoResults:results, string:output[], len = sizeof(output))
{
	if (results == INVALID_GEO_RESULTS)
	{
		return false;
	}

	strcopy(output, Geo_g_sResults[_:results][GEO_RESULTS_REGION_NAME], len);
	return true;
}

stock bool:Geo_GetRegionCode(GeoResults:results, string:output[], len = sizeof(output))
{
	if (results == INVALID_GEO_RESULTS)
	{
		return false;
	}

	strcopy(output, Geo_g_sResults[_:results][GEO_RESULTS_REGION], len);
	return true;
}

stock bool:Geo_GetCityName(GeoResults:results, string:output[], len = sizeof(output))
{
	if (results == INVALID_GEO_RESULTS)
	{
		return false;
	}

	strcopy(output, Geo_g_sResults[_:results][GEO_RESULTS_CITY], len);
	return true;
}

stock bool:Geo_GetISPName(GeoResults:results, string:output[], len = sizeof(output))
{
	if (results == INVALID_GEO_RESULTS)
	{
		return false;
	}

	strcopy(output, Geo_g_sResults[_:results][GEO_RESULTS_ISP], len);
	return true;
}

stock bool:Geo_GetIPAddress(GeoResults:results, string:output[], len = sizeof(output))
{
	if (results == INVALID_GEO_RESULTS)
	{
		return false;
	}

	strcopy(output, Geo_g_sResults[_:results][GEO_RESULTS_IP], len);
	return true;
}

forward Geo_OnResults(Request:id, E_HTTP_STATUS:status, Node:node);
public Geo_OnResults(Request:id, E_HTTP_STATUS:status, Node:node)
{
	new geoStatus[16];
	JsonGetString(node, "status", status);

    new Func:cb<iii> = Geo_g_sCallbacks[_:id];
    Geo_g_sCallbacks[_:id] = Func:0<iii>;

	if (strqeual(geoStatus, "fail"))
	{
		@.cb(cellmin);
		Indirect_Release(cb);
		return 0;
	}

	JsonGetString(node, "country", Geo_g_sResults[_:id][GEO_RESULTS_COUNTRY], 32);
	JsonGetString(node, "countryCode", Geo_g_sResults[_:id][GEO_RESULTS_COUNTRY_CODE], 32);
	JsonGetString(node, "region", Geo_g_sResults[_:id][GEO_RESULTS_REGION], 32);
	JsonGetString(node, "regionName", Geo_g_sResults[_:id][GEO_RESULTS_REGION_NAME], 32);
	JsonGetString(node, "city", Geo_g_sResults[_:id][GEO_RESULTS_CITY], 32);
	JsonGetString(node, "isp", Geo_g_sResults[_:id][GEO_RESULTS_ISP], 32);
	JsonGetString(node, "query", Geo_g_sResults[_:id][GEO_RESULTS_IP], 32);

	@.cb(_:id);
	Indirect_Release(cb);
	return 1;
}
