#pragma once

typedef struct {
    uint32_t cmdId;
    // identifier is not used
    std::string description;
    uint32_t conversionDivisor;
    std::string unit;
} ValueEntry_v1;


// Command Values 3004

// *INDENT-OFF*
const ValueEntry_v1 ValueTableDecode_v1[] = {
//  | ID | Identifier                          | Description                                                | Conversion | Unit |
//  |----|-------------------------------------|------------------------------------------------------------|------------|------|
    {  10, /*ID_WEB_Temperatur_TVL*/            "Vorlauftemperatur Heizkreis",                                         10, "°C"},        //, 10 /* 0..102.3 */},
    {  11, /*ID_WEB_Temperatur_TRL*/            "Rücklauftemperatur Heizkreis",                                        10, "°C"},        //, 10 /* 0..102.3 */},
    {  12, /*ID_WEB_Sollwert_TRL_HZ*/           "Rücklauf-Soll Heizkreis",                                             10, "°C"},        //, 10 /* 0..102.3 */},
    {  13, /*ID_WEB_Temperatur_TRL_ext*/        "Rücklauftemperatur im Trennspeicher",                                 10, "°C"},        //, 10 /* ??? War soeben 50 */ },
    {  14, /*ID_WEB_Temperatur_THG*/            "Heisgastemperatur",                                                   10, "°C"},        //, 10 /* 0..102.3 */},
    {  15, /*ID_WEB_Temperatur_TA*/             "Aussentemperatur",                                                    10, "°C"},        //, 10 /* 0..102.3 */},   // was ist mit negativen Werten
    {  16, /*ID_WEB_Mitteltemperatur*/          "Durchschnittstemperatur Aussen über 24 h (Funktion Heizgrenze)",      10, "°C"},        //, 10 /* 0..102.3 */},   // was ist mit negativen Werten
    {  17, /*ID_WEB_Temperatur_TBW*/            "Warmwasser Ist-Temperatur",                                           10, "°C"},        //, 10 /* 0..102.3 */},
    {  18, /*ID_WEB_Einst_BWS_akt*/             "Warmwasser Soll-Temperatur",                                          10, "°C"},        //, 10 /* 0..102.3 */},
    {  19, /*ID_WEB_Temperatur_TWE*/            "Wärmequellen-Eintrittstemperatur",                                    10, "°C"},        //,  8 /* 0.. 25.5 */},
    {  20, /*ID_WEB_Temperatur_TWA*/            "Wärmequellen-Austrittstemperatur",                                    10, "°C"},        //,  8 /* 0.. 25.5 */},
    {  21, /*ID_WEB_Temperatur_TFB1*/           "Mischkreis 1 Vorlauftemperatur",                                      10, "°C"},        //, 10 /* 0..102.3 */},   // Was ist das für ein Wert?
    {  22, /*ID_WEB_Sollwert_TVL_MK1*/          "Mischkreis 1 Vorlauf-Soll-Temperatur",                                10, "°C"},        //, 10 /* 0..102.3 */},   // Was ist das für ein Wert?
    {  23, /*ID_WEB_Temperatur_RFV*/            "Raumtemperatur Raumstation 1",                                        10, "°C"},        //, 10 /* 0..102.3 */},   // Was ist das für ein Wert?
    {  24, /*ID_WEB_Temperatur_TFB2*/           "Mischkreis 2 Vorlauftemperatur",                                      10, "°C"},        //, 10 /* 0..102.3 */},   // Was ist das für ein Wert?
    {  25, /*ID_WEB_Sollwert_TVL_MK2*/          "Mischkreis 2 Vorlauf-Soll-Temperatur",                                10, "°C"},        //, 10 /* 0..102.3 */},   // Was ist das für ein Wert?
    {  26, /*ID_WEB_Temperatur_TSK*/            "Fühler Solarkollektor",                                               10, "°C"},        //, 10 /* 0..102.3 */},   // Was ist das für ein Wert?
    {  27, /*ID_WEB_Temperatur_TSS*/            "Fühler Solarspeicher",                                                10, "°C"},        //, 10 /* 0..102.3 */},   // Was ist das für ein Wert?
    {  28, /*ID_WEB_Temperatur_TEE*/            "Fühler externe Energiequelle",                                        10, "°C"},        //, 10 /* 0..102.3 */},   // Was ist das für ein Wert?
    {  29, /*ID_WEB_ASDin*/                     "Eingang Abtauende / Soledruck / Durchfluss",                           0,  "-"},        //, 1 /* Binary value */ },
	{  30, /*ID_WEB_BWTin_Eingang*/             "Brauchwarmwasserthermostat",                                           0,  "-"},        //, 1 /* Binary value */ },
	{  31, /*ID_WEB_EVUin_Eingang*/             "EVU Sperre",                                                           0,  "-"},        //, 1 /* Binary value */ },
	{  32, /*ID_WEB_HDin_Eingang*/              "Hochdruck Kältekreis",                                                 0,  "-"},        //, 1 /* Binary value */ },
	{  33, /*ID_WEB_MOTin_Eingang*/             "Motorschutz OK",                                                       0,  "-"},        //, 1 /* Binary value */ },
	{  34, /*ID_WEB_NDin_Eingang*/              "Niederdruck",                                                          0,  "-"},        //, 1 /* Binary value */ },
	{  35, /*ID_WEB_PEXin_Eingang*/             "Überwachungskontakt für Potentiostat",                                 0,  "-"},        //, 1 /* Binary value */ },
	{  36, /*ID_WEB_SWTin_Eingang*/             "Schwimmbadthermostat",                                                 0,  "-"},        //, 1 /* Binary value */ },
	{  37, /*ID_WEB_AVout_Ausgang*/             "Abtauventil",                                                          0,  "-"},        //, 1 /* Binary value */ },
	{  38, /*ID_WEB_BUPout_Ausgang*/            "Brauchwasserpumpe/Umstellventil",                                      0,  "-"},        //, 1 /* Binary value */ },
	{  39, /*ID_WEB_HUPout_Ausgang*/            "Heizungsumwälzpumpe",                                                  0,  "-"},        //, 1 /* Binary value */ },
	{  40, /*ID_WEB_MA1out_Ausgang*/            "Mischkreis 1 Auf",                                                     0,  "-"},        //, 1 /* Binary value */ },
	{  41, /*ID_WEB_MZ1out_Ausgang*/            "Mischkreis 1 Zu",                                                      0,  "-"},        //, 1 /* Binary value */ },
	{  42, /*ID_WEB_VENout_Ausgang*/            "Ventilation (Lüftung)",                                                0,  "-"},        //, 1 /* Binary value */ },
	{  43, /*ID_WEB_VBOout_Ausgang*/            "Solepumpe/Ventilator",                                                 0,  "-"},        //, 1 /* Binary value */ },
	{  44, /*ID_WEB_VD1out_Ausgang*/            "Verdichter 1",                                                         0,  "-"},        //, 1 /* Binary value */ },
	{  45, /*ID_WEB_VD2out_Ausgang*/            "Verdichter 2",                                                         0,  "-"},        //, 1 /* Binary value */ },
	{  46, /*ID_WEB_ZIPout_Ausgang*/            "Zirkulationspumpe",                                                    0,  "-"},        //, 1 /* Binary value */ },
	{  47, /*ID_WEB_ZUPout_Ausgang*/            "Zusatzumwälzpumpe",                                                    0,  "-"},        //, 1 /* Binary value */ },
	{  48, /*ID_WEB_ZW1out_Ausgang*/            "Steuersignal Zusatzheizung v. Heizung",                                0,  "-"},        //, 1 /* Binary value */ },
	{  49, /*ID_WEB_ZW2SSTout_Ausgang*/         "Steuersignal Zusatzheizung/Störsignal",                                0,  "-"},        //, 1 /* Binary value */ },
	{  50, /*ID_WEB_ZW3SSTout_Ausgang*/         "Zusatzheizung 3",                                                      0,  "-"},        //, 1 /* Binary value */ },
	{  51, /*ID_WEB_FP2out_Ausgang*/            "Pumpe Mischkreis 2",                                                   0,  "-"},        //, 1 /* Binary value */ },
	{  52, /*ID_WEB_SLPout_Ausgang*/            "Solarladepumpe",                                                       0,  "-"},        //, 1 /* Binary value */ },
	{  53, /*ID_WEB_SUPout_Ausgang*/            "Schwimmbadpumpe",                                                      0,  "-"},        //, 1 /* Binary value */ },
	{  54, /*ID_WEB_MZ2out_Ausgang*/            "Mischkreis 2 Zu",                                                      0,  "-"},        //, 1 /* Binary value */ },
	{  55, /*ID_WEB_MA2out_Ausgang*/            "Mischkreis 2 Auf",                                                     0,  "-"},        //, 1 /* Binary value */ },
	{  56, /*ID_WEB_Zaehler_BetrZeitVD1*/       "Betriebsstunden Verdichter 1",                                         0, "Sekunden" },
	{  57, /*ID_WEB_Zaehler_BetrZeitImpVD1*/    "Impulse Verdichter 1",                                                 0, "Impulse" },
	{  58, /*ID_WEB_Zaehler_BetrZeitVD2*/       "Betriebsstunden Verdichter 2",                                         0, "Sekunden" },
	{  59, /*ID_WEB_Zaehler_BetrZeitImpVD2*/    "Impulse Verdichter 2",                                                 0, "Impulse" },
	{  60, /*ID_WEB_Zaehler_BetrZeitZWE1*/      "Betriebsstunden Zweiter Wärmeerzeuger 1",                              0, "Sekunden" },
	{  61, /*ID_WEB_Zaehler_BetrZeitZWE2*/      "Betriebsstunden Zweiter Wärmeerzeuger 2",                              0, "Sekunden" },
	{  62, /*ID_WEB_Zaehler_BetrZeitZWE3*/      "Betriebsstunden Zweiter Wärmeerzeuger 3",                              0, "Sekunden" },      // |?? (Wert / 7200 = Stunden)|??
	{  63, /*ID_WEB_Zaehler_BetrZeitWP*/        "Betriebsstunden Wärmepumpe",                                           0, "Sekunden" },
	{  64, /*ID_WEB_Zaehler_BetrZeitHz*/        "Betriebsstunden Heizung",                                              0, "Sekunden" },
	{  65, /*ID_WEB_Zaehler_BetrZeitBW*/        "Betriebsstunden Warmwasser",                                           0, "Sekunden" },
	{  66, /*ID_WEB_Zaehler_BetrZeitKue*/       "Betriebsstunden Kühlung",                                              0, "Sekunden" },
	{  67, /*ID_WEB_Time_WPein_akt*/            "Wärmepumpe läuft seit",                                                0, "Sekunden" },
	{  68, /*ID_WEB_Time_ZWE1_akt*/             "Zweiter Wärmeerzeuger 1 läuft seit",                                   0, "Sekunden" },
	{  69, /*ID_WEB_Time_ZWE2_akt*/             "Zweiter Wärmeerzeuger 2 läuft seit",                                   0, "Sekunden" },
	{  70, /*ID_WEB_Timer_EinschVerz*/          "Netzeinschaltverzögerung",                                             0, "Sekunden" },
	{  71, /*ID_WEB_Time_SSPAUS_akt*/           "Schaltspielsperre Aus",                                                0, "Sekunden" },
	{  72, /*ID_WEB_Time_SSPEIN_akt*/           "Schaltspielsperre Ein",                                                0, "Sekunden" },
	{  73, /*ID_WEB_Time_VDStd_akt*/            "Verdichter-Standzeit",                                                 0, "Sekunden" },
	{  74, /*ID_WEB_Time_HRM_akt*/              "Heizungsregler Mehr-Zeit",                                             0, "Sekunden" },
	{  75, /*ID_WEB_Time_HRW_akt*/              "Heizungsregler Weniger-Zeit",                                          0, "Sekunden" },
	{  76, /*ID_WEB_Time_LGS_akt*/              "Thermische Desinfektion läuft seit",                                   0, "Sekunden" },
	{  77, /*ID_WEB_Time_SBW_akt*/              "Sperre Warmwasser",                                                    0, "Sekunden" },
	{  80, /*D_WEB_WP_BZ_akt*/                  "Betriebszustand",                                                      0, "enum" },
};
// *INDENT-ON*

const uint32_t FILE_Version_v1 = 1;
const uint32_t FILE_SizeOfHeader_v1 = 3;
const uint32_t FILE_NrDataEntries_v1 = sizeof(ValueTableDecode_v1) / sizeof(ValueEntry_v1);
