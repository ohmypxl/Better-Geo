# Better-Geo

[![sampctl](https://img.shields.io/badge/sampctl-Geo--Requests-2f2f2f.svg?style=for-the-badge)](https://github.com/ohmypxl/Geo-Requests)

This is the Reinventing-Wheel geoip but using requests, that's it.

Why? because unlike certain someone i feel like using plugins like requests is a must!, it's also for me to better understand about how to work with requests.

***Note:** For now we only support http connection, if you know how to make ssl connection with example keys, feel free to make PR*

## Installation

Simply install to your project:

```bash
sampctl package install ohmypxl/Geo-Requests
```

Include in your code and begin using the library:

```pawn
#include <Geo-Requests>
```

## Usage

Simple usage with inline:
```pawn
#include <YSI_Coding\y_inline>
#include <Geo-Requests>

public OnPlayerConnect(playerid)
{
	inline OnGeoResults(GeoResults:result)
	{
		if (!IsValidGeoResults(result))
		{
			printf("Unable to give results to %d", playerid);
			return 0;
		}

		new string:countryName[32], string:ispName[32];
		Geo_GetCountryName(result, countryName);
		Geo_GetIspName(result, ispName);

		va_SendClientMessage(playerid, 0xFFFFFFAA, "LOGIN: %s is logged-in to the server [%s | %s]", ReturnPlayerName(playerid), countryName, ispName);
	}

	Geo_InlineCheckPlayer(playerid, using inline OnGeoResults);
	return 1;
}
```

Without Inline:
```pawn
forward OnGeoResults(GeoResult:result);
public OnGeoResults(GeoResult:result)
{
	if (!IsValidGeoResults(result))
	{
		printf("Unable to give results to %d", playerid);
		return 0;
	}

	new string:countryName[32], string:ispName[32];
	Geo_GetCountryName(result, countryName);
	Geo_GetIspName(result, ispName);

	va_SendClientMessage(playerid, 0xFFFFFFAA, "LOGIN: %s is logged-in to the server [%s | %s]", ReturnPlayerName(playerid), countryName, ispName);
}

public OnPlayerConnect(playerid)
{
	Geo_CheckPlayer(playerid, "OnGeoResults");
	return 1;
}
```

***Note:** You can still use `Geo_CheckPlayer` if you're using inline, but it's discouraged since you can just do `using public OnGeoResults<i>`*

Function Info:
```pawn
// This will initiate geo checking based on playerid IP's
forward bool:Geo_InlineCheckPlayer(playerid, Func:callback<i>);
forward bool:Geo_CheckPlayer(playerid, const string:callback[]);

// Same as above but based on input IP's
forward bool:Geo_InlineCheckIp(const string:ipAddress[], Func:callback<i>);
forward bool:Geo_CheckIp(const string:ipAddress[], const string:callback[]);

// Check if the inline result is legit or not
forward bool:Geo_IsValidGeoResults(GeoResults:result);

// Get data from GeoResults
forward bool:Geo_GetCountryName(GeoResults:result, string:output[], len = sizeof(output));
forward bool:Geo_GetCountryCode(GeoResults:result, string:output[], len = sizeof(output));
forward bool:Geo_GetRegionName(GeoResults:result, string:output[], len = sizeof(output));
forward bool:Geo_GetRegionCode(GeoResults:result, string:output[], len = sizeof(output));
forward bool:Geo_GetCityName(GeoResults:result, string:output[], len = sizeof(output));
forward bool:Geo_GetIspName(GeoResults:result, string:output[], len = sizeof(output));
forward bool:Geo_GetIpResults(GeoResults:result, string:output[], len = sizeof(output));

forward bool:Geo_IsProxy(GeoResults:result);
forward bool:Geo_IsMobileConnection(GeoResults:result);
```

## Testing


To test, simply run the package:

```bash
sampctl package run
```
