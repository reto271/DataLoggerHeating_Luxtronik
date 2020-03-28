#include <iostream>

#include "Common/src/ValueTable_v2.hpp"



ValueTable_v2::ValueTable_v2()
{
}

ValueTable_v2::~ValueTable_v2()
{
}

uint32_t ValueTable_v2::getFileVersion() const
{
    return FILE_Version;
}

uint32_t ValueTable_v2::getSizeOfHeader() const
{
    return FILE_SizeOfHeader;
}

void ValueTable_v2::initValueTable()
{
    // *INDENT-OFF*
    m_entries = {
//  | ID | Identifier                          | Description                                                | Conversion | Unit |
//  |----|-------------------------------------|------------------------------------------------------------|------------|------|
    {  10, /*ID_WEB_Temperatur_TVL*/            "Vorlauftemperatur Heizkreis",                                         10, "°C"},        //, 10 /* 0..102.3 */},
    {  11, /*ID_WEB_Temperatur_TRL*/            "Rücklauftemperatur Heizkreis",                                        10, "°C"},        //, 10 /* 0..102.3 */},
    {  12, /*ID_WEB_Sollwert_TRL_HZ*/           "Rücklauf-Soll Heizkreis",                                             10, "°C"},        //, 10 /* 0..102.3 */},
    {  14, /*ID_WEB_Temperatur_THG*/            "Heisgastemperatur",                                                   10, "°C"},        //, 10 /* 0..102.3 */},
    {  15, /*ID_WEB_Temperatur_TA*/             "Aussentemperatur",                                                    10, "°C"},        //, 10 /* 0..102.3 */},   // was ist mit negativen Werten
    {  16, /*ID_WEB_Mitteltemperatur*/          "Durchschnittstemperatur Aussen über 24 h (Funktion Heizgrenze)",      10, "°C"},        //, 10 /* 0..102.3 */},   // was ist mit negativen Werten
    {  17, /*ID_WEB_Temperatur_TBW*/            "Warmwasser Ist-Temperatur",                                           10, "°C"},        //, 10 /* 0..102.3 */},
    {  18, /*ID_WEB_Einst_BWS_akt*/             "Warmwasser Soll-Temperatur",                                          10, "°C"},        //, 10 /* 0..102.3 */},
    {  19, /*ID_WEB_Temperatur_TWE*/            "Wärmequellen-Eintrittstemperatur",                                    10, "°C"},        //,  8 /* 0.. 25.5 */},
    {  20, /*ID_WEB_Temperatur_TWA*/            "Wärmequellen-Austrittstemperatur",                                    10, "°C"},        //,  8 /* 0.. 25.5 */},
    {  29, /*ID_WEB_ASDin*/                     "Eingang Abtauende / Soledruck / Durchfluss",                           0,  "-"},        //, 1 /* Binary value */ },
    {  30, /*ID_WEB_BWTin_Eingang*/             "Brauchwarmwasserthermostat",                                           0,  "-"},        //, 1 /* Binary value */ },
    {  31, /*ID_WEB_EVUin_Eingang*/             "EVU Sperre",                                                           0,  "-"},        //, 1 /* Binary value */ },
    {  32, /*ID_WEB_HDin_Eingang*/              "Hochdruck Kältekreis",                                                 0,  "-"},        //, 1 /* Binary value */ },
    {  33, /*ID_WEB_MOTin_Eingang*/             "Motorschutz OK",                                                       0,  "-"},        //, 1 /* Binary value */ },
    {  34, /*ID_WEB_NDin_Eingang*/              "Niederdruck",                                                          0,  "-"},        //, 1 /* Binary value */ },
    {  35, /*ID_WEB_PEXin_Eingang*/             "Überwachungskontakt für Potentiostat",                                 0,  "-"},        //, 1 /* Binary value */ },
    {  37, /*ID_WEB_AVout_Ausgang*/             "Abtauventil",                                                          0,  "-"},        //, 1 /* Binary value */ },
    {  38, /*ID_WEB_BUPout_Ausgang*/            "Brauchwasserpumpe/Umstellventil",                                      0,  "-"},        //, 1 /* Binary value */ },
    {  39, /*ID_WEB_HUPout_Ausgang*/            "Heizungsumwälzpumpe",                                                  0,  "-"},        //, 1 /* Binary value */ },
    {  42, /*ID_WEB_VENout_Ausgang*/            "Ventilation (Lüftung)",                                                0,  "-"},        //, 1 /* Binary value */ },
    {  43, /*ID_WEB_VBOout_Ausgang*/            "Solepumpe/Ventilator",                                                 0,  "-"},        //, 1 /* Binary value */ },
    {  44, /*ID_WEB_VD1out_Ausgang*/            "Verdichter 1",                                                         0,  "-"},        //, 1 /* Binary value */ },
    {  46, /*ID_WEB_ZIPout_Ausgang*/            "Zirkulationspumpe",                                                    0,  "-"},        //, 1 /* Binary value */ },
    {  47, /*ID_WEB_ZUPout_Ausgang*/            "Zusatzumwälzpumpe",                                                    0,  "-"},        //, 1 /* Binary value */ },
    {  48, /*ID_WEB_ZW1out_Ausgang*/            "Steuersignal Zusatzheizung v. Heizung",                                0,  "-"},        //, 1 /* Binary value */ },
    {  49, /*ID_WEB_ZW2SSTout_Ausgang*/         "Steuersignal Zusatzheizung/Störsignal",                                0,  "-"},        //, 1 /* Binary value */ },
    {  56, /*ID_WEB_Zaehler_BetrZeitVD1*/       "Betriebsstunden Verdichter 1",                                         0, "Sekunden" },
    {  57, /*ID_WEB_Zaehler_BetrZeitImpVD1*/    "Impulse Verdichter 1",                                                 0, "Impulse" },
    {  61, /*ID_WEB_Zaehler_BetrZeitZWE2*/      "Betriebsstunden Zweiter Wärmeerzeuger 2",                              0, "Sekunden" }, // Heats up to 60 deg
    {  63, /*ID_WEB_Zaehler_BetrZeitWP*/        "Betriebsstunden Wärmepumpe",                                           0, "Sekunden" },
    {  64, /*ID_WEB_Zaehler_BetrZeitHz*/        "Betriebsstunden Heizung",                                              0, "Sekunden" },
    {  65, /*ID_WEB_Zaehler_BetrZeitBW*/        "Betriebsstunden Warmwasser",                                           0, "Sekunden" },
    {  67, /*ID_WEB_Time_WPein_akt*/            "Wärmepumpe läuft seit",                                                0, "Sekunden" },
    {  69, /*ID_WEB_Time_ZWE2_akt*/             "Zweiter Wärmeerzeuger 2 läuft seit",                                   0, "Sekunden" },   // Heats up to 60 deg
    {  70, /*ID_WEB_Timer_EinschVerz*/          "Netzeinschaltverzögerung",                                             0, "Sekunden" },
    {  71, /*ID_WEB_Time_SSPAUS_akt*/           "Schaltspielsperre Aus",                                                0, "Sekunden" },
    {  72, /*ID_WEB_Time_SSPEIN_akt*/           "Schaltspielsperre Ein",                                                0, "Sekunden" },
    {  73, /*ID_WEB_Time_VDStd_akt*/            "Verdichter-Standzeit",                                                 0, "Sekunden" },
    {  74, /*ID_WEB_Time_HRM_akt*/              "Heizungsregler Mehr-Zeit",                                             0, "Sekunden" },
    {  75, /*ID_WEB_Time_HRW_akt*/              "Heizungsregler Weniger-Zeit",                                          0, "Sekunden" },
    {  76, /*ID_WEB_Time_LGS_akt*/              "Thermische Desinfektion läuft seit",                                   0, "Sekunden" },  // Heats up to 60 deg
    {  77, /*ID_WEB_Time_SBW_akt*/              "Sperre Warmwasser",                                                    0, "Sekunden" },  // potentially 0
    {  78, /*ID_WEB_Code_WP_akt*/               "Wärmepumpentyp|0 = ERC|Typenschlüssel",                                0, "-" },  //-> new, potentially  const
    {  79, /*ID_WEB_BIV_Stufe_akt*/             "Bivalenzstufe",                                                        0, "enum" },
    {  80, /*ID_WEB_WP_BZ_akt*/                 "Betriebszustand",                                                      0, "enum" },
    { 151, /*ID_WEB_WMZ_Heizung*/               "Wärmemengenzähler Heizung",                                           10, "kWh"},  //
    { 152, /*ID_WEB_WMZ_Brauchwasser*/          "Wärmemengenzähler Brauchwasser",                                      10, "kWh"},
    { 154, /*ID_WEB_WMZ_Seit*/                  "Wärmemengenzähler Gesamt",                                            10, "kWh"},
    { 155, /*ID_WEB_WMZ_Durchfluss*/            "Wärmemengenzähler Durchfluss",                                         1, "l / h"},
    };
    // *INDENT-ON*
}
