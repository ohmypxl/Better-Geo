
#include <a_samp>

#define YSI_NO_HEAP_MALLOC

#include <YSI_Coding\y_inline>
#include "Geo-Requests.inc"

main() 
{
	inline OnGeoResults(GeoResults:result)
	{
		if (!IsValidGeoResults(result))
		{
			print("Unable to give the results");
			return;
		}

		new 
			string:ipResults[16 + 1],
			string:countryName[32], 
			string:ispName[32];

		Geo_GetIpResults(result, ipResults);
		Geo_GetCountryName(result, countryName);
		Geo_GetIspName(result, ispName);

		printf("This %s is from %s using %s ISP's", ipResults, countryName, ispName);
	}

	Geo_CheckIp("24.48.0.1", using inline OnGeoResults);
}

