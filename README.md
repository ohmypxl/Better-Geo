# Better-Geo

[![sampctl](https://img.shields.io/badge/sampctl-Better--Geo-2f2f2f.svg?style=for-the-badge)](https://github.com/ohmypxl/Better-Geo)

This is the Reinventing-Wheel geoip but using plugins and using Inline callbacks, that's it.

## Installation

Simply install to your project:

```bash
sampctl package install ohmypxl/Better-Geo
```

Include in your code and begin using the library:

```pawn
#include <Better-Geo>
```

## Usage

Simple usage:
```pawn
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
		Geo_GetISPName(result, ispName);

		va_SendClientMessage(playerid, 0xFFFFFFAA, "LOGIN: %s is logged-in to the server [%s | %s]", ReturnPlayerName(playerid), countryName, ispName);
	}

	Geo_CheckPlayer(
		playerid, 
		using inline OnGeoResults, 
		OPTION_IP_RESULTS | OPTION_COUNTRY_NAME | OPTION_ISP_NAME
	);
	return 1;
}
```

Function Info:
```
// This will initiate geo checking based on playerid IP's
forward bool:Geo_CheckPlayer(playerid, inline callback, GeoCheckOption:option);

// Same as above but based on input IP's
forward bool:Geo_CheckIpAddress(const string:ipAddress[], inline callback, GeoCheckOption:option);

// Check if the inline result is legit or not
forward bool:Geo_IsValidGeoResults(GeoResults:result);

// Get data from GeoResults
forward bool:Geo_GetCountryName(GeoResults:result, string:output[], len = sizeof(output));
forward bool:Geo_GetCountryCode(GeoResults:result, string:output[], len = sizeof(output));
forward bool:Geo_GetRegionName(GeoResults:result, string:output[], len = sizeof(output));
forward bool:Geo_GetRegionCode(GeoResults:result, string:output[], len = sizeof(output));
forward bool:Geo_GetCityName(GeoResults:result, string:output[], len = sizeof(output));
forward bool:Geo_GetISPName(GeoResults:result, string:output[], len = sizeof(output));
forward bool:Geo_GetIpResults(GeoResults:result, string:output[], len = sizeof(output));
```

## Testing


To test, simply run the package:

```bash
sampctl package run
```
