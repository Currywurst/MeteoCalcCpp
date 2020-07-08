# MeteoCalcCpp
Based on the implementation from https://github.com/malexer/meteocalc  - but just for c++.

Several functions for calculation of meteorological variables.
Calculations for wind and temperature are based on publicly available formulas.

## Example
    #include <iostream>

    #include "MeteoCalc.h"


    int main() {
      // create input temperature in different units
      CMetroCalc::CTemp cTemp( 20, CMetroCalc::CTemp::enTempUnitCelcius );
      CMetroCalc::CTemp cTemp2( 60, CMetroCalc::CTemp::enTempUnitFahrenheit );

      // create input wind in different units
      CMetroCalc::CWind cWind( 25, CMetroCalc::CWind::enWindUnitKMh );
      CMetroCalc::CWind cWind2( 80, CMetroCalc::CWind::enWindUnitMpH );

      // calculate Dew Point
      cTemp.getDewPoint( 56 );

      // calculate Heat Index
      cTemp.getHeatIndex( 42 );

      // calculate Wind Chill
      CMetroCalc::CTemp cTempChill( -5, CMetroCalc::CTemp::enTempUnitCelcius );
      CMetroCalc::CTemp cTempChillResult = cTemp.getWindChill( cWind );

      // calculate Feels Like temperature
      CMetroCalc::CTemp cTempFeelsLike = cTemp.getFeelsLike( 40, cWind );


      std::cout << "Feels like: " << cTempFeelsLike.getC() << "°C" << std::endl;
      return 0;
    }


###### works for ESP32 ;)
