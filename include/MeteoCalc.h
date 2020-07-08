//
//  MetroCalc.h
//  Calc
//
//  Created by David Braun on 08.07.20.
//  Copyright © 2020 David Braun. All rights reserved.
//

#ifndef METEOCALC_H
#define METEOCALC_H

#include <cmath>

namespace CMetroCalc
{
  class CWind
  {
  public:
      enum enWindUnit { enWindUnitMpH, enWindUnitKnots, enWindUnitMpS, enWindUnitFpS, enWindUnitKMh };
      CWind( double p_fWind, enWindUnit p_enUnit ) { m_dWind = p_fWind; m_enUnit = p_enUnit; }
      ~CWind() = default;

      double getMpH()   { return convert( enWindUnitMpH   ); }
      double getKnots() { return convert( enWindUnitKnots ); }
      double getMpS()   { return convert( enWindUnitMpS   ); }
      double getFpS()   { return convert( enWindUnitFpS   ); }
      double getKMh()   { return convert( enWindUnitKMh   ); }

      double getWind     () const { return m_dWind ; }
      enWindUnit getUnit() const { return m_enUnit; }

  protected:
      double convert( double p_dWindSpeed, enWindUnit p_enUnitFrom, enWindUnit p_enUnitTo )
      {
        if( p_enUnitFrom == p_enUnitTo || p_dWindSpeed < 0 )
          return p_dWindSpeed;

        if( p_enUnitFrom == enWindUnitKnots )
          return convertToSpeed( p_dWindSpeed, p_enUnitTo );
        else if( p_enUnitTo == enWindUnitKnots )
          return convertToKnots( p_dWindSpeed, p_enUnitFrom );
        else
        {
          double dMid = convertToKnots( p_dWindSpeed, p_enUnitFrom );
          return convertToSpeed( dMid, p_enUnitTo );
        }
      }

      double convertToKnots( double p_dWindSpeed, enWindUnit p_enUnit )
      {
        if( p_dWindSpeed < 0 )
          return p_dWindSpeed;

        double dKnots;

        switch( p_enUnit )
        {
          case enWindUnitMpH:
            dKnots = p_dWindSpeed * 0.8689762;
            break;

          case enWindUnitMpS:
            dKnots = p_dWindSpeed * 1.9438445;
            break;

          case enWindUnitFpS:
            dKnots = p_dWindSpeed * 0.5924838;
            break;

          case enWindUnitKMh:
            dKnots = p_dWindSpeed * 0.539593;
            break;

          case enWindUnitKnots:
          default:
            dKnots = p_dWindSpeed;
            break;
        }

        return dKnots;
      }

      double convertToSpeed( double p_dKnots, enWindUnit p_enUnit )
      {
        if( p_dKnots < 0 )
          return p_dKnots;

        double dSpeed;

        switch( p_enUnit )
        {
          case enWindUnitMpH:
            dSpeed = p_dKnots * 1.1507794;
            break;

          case enWindUnitMpS:
            dSpeed = p_dKnots * 0.5144444;
            break;

          case enWindUnitFpS:
            dSpeed = p_dKnots * 1.6878099;
            break;

          case enWindUnitKMh:
            dSpeed = p_dKnots * 1.85325;
            break;

          case enWindUnitKnots:
          default:
            dSpeed = p_dKnots;
            break;
        }

        return dSpeed;
      }
  private:
      double convert( enWindUnit p_enUnit ) { return convert( m_dWind, m_enUnit, p_enUnit ); }
      double m_dWind = 0.0;
      enWindUnit m_enUnit = enWindUnitMpH;
  };

  class CTemp
  {
  public:
      enum enTempUnit { enTempUnitCelcius, enTempUnitFahrenheit, enTempUnitKelvin };
      CTemp(double p_dTemp, enTempUnit p_enUnit ) { m_dTemp = p_dTemp; m_enUnit = p_enUnit; }
      ~CTemp() {}

      double getC() { return convert( enTempUnitCelcius    ); }
      double getF() { return convert( enTempUnitFahrenheit ); }
      double getK() { return convert( enTempUnitKelvin     ); }

      double getTemp    () const { return m_dTemp ; }
      enTempUnit getUnit() const { return m_enUnit; }

      CTemp getDewPoint(double p_dHumidity )
      {
        if( p_dHumidity < 1.f or p_dHumidity > 100 )
          return *this;

        double fTemp = getC();
        double dConstrantB = 17.368;
        double dConstrantC = 238.88;
        if( fTemp > 0 )
        {
          dConstrantB = 17.966;
          dConstrantC = 247.15;
        }

        double fPA = p_dHumidity / 100. * exp( dConstrantB * fTemp / ( dConstrantC + fTemp ) );
        double fDP = dConstrantC * log( fPA ) / ( dConstrantB - log( fPA ) );

        return CTemp(fDP, enTempUnitCelcius );
      }

      CTemp getHeatIndex(double p_dHumidity )
      {
        double fTemp = getF();

        double fHI = 0.5 * ( fTemp + 61.0 + ( ( fTemp - 68.0 ) * 1.2 ) + ( p_dHumidity * 0.094 ) );

        if( fHI >= 80 )
        {
          fHI = -42.379 + 2.04901523 * fTemp + 10.14333127 * p_dHumidity - 0.22475541 * fTemp * p_dHumidity - 6.83783E-3 * fTemp * fTemp - 5.481717E-2 * p_dHumidity * p_dHumidity + 1.22874E-3 * fTemp * fTemp * p_dHumidity + 8.5282E-4 * fTemp * p_dHumidity * p_dHumidity - 1.99E-6 * fTemp * fTemp * p_dHumidity * p_dHumidity;
        }
        return CTemp(fHI, enTempUnitFahrenheit );
      }

      CTemp getWindChill( CWind& p_cWindSpeed )
      {
        double fTemp = getF();
        double fWindSpeed = p_cWindSpeed.getMpH();

        if( fTemp > 50 or fWindSpeed <= 3 )
          return *this;

        double fWindChill = 35.74 + ( 0.6215 * fTemp ) - 35.75 * ( pow ( fWindSpeed, 0.16 ) ) +  ( ( 0.4275 * fTemp ) * ( pow ( fWindSpeed, 0.16 ) ) );

        return CTemp(fWindChill, enTempUnitFahrenheit );
      }

      CTemp getFeelsLike( double p_dHumidity, CWind& p_cWindSpeed )
      {
        double fTemp = getF();
        double fWindSpeed = p_cWindSpeed.getMpH();

        if( fTemp <= 50 && fWindSpeed > 3 )
          return getWindChill( p_cWindSpeed );
        else if( fTemp >= 80 )
          return getHeatIndex( p_dHumidity );
        else
          return *this;
      }

  protected:
      double convert( double p_dTemp, enTempUnit p_enUnitFrom, enTempUnit p_enUnitTo )
      {
        if( p_enUnitFrom == p_enUnitTo )
          return p_dTemp;

        double fResult = p_dTemp;

        if( p_enUnitFrom != enTempUnitCelcius )
        {
          if( p_enUnitFrom == enTempUnitKelvin )
            fResult = p_dTemp - 273.15;
          else if ( p_enUnitFrom == enTempUnitFahrenheit )
            fResult = (p_dTemp - 32) * 5 / 9.0;

          if( p_enUnitTo == enTempUnitCelcius )
            return fResult;
        }

        if( p_enUnitTo == enTempUnitKelvin )
          fResult = fResult + 273.15;
        else if( p_enUnitTo == enTempUnitFahrenheit )
          fResult = fResult * 9 / 5.0 + 32;

        return fResult;
      }

  private:
      double convert( enTempUnit p_enUnit ) { return convert( m_dTemp, m_enUnit, p_enUnit ); }
      double m_dTemp = 0.0;
      enTempUnit m_enUnit = enTempUnitCelcius;
  };
}

#endif /* METEOCALC_H */
