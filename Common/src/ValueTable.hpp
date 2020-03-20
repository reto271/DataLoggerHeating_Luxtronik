#pragma once

typedef struct
{
    uint32_t cmdId;
    // identifier is not used
    std::string description;
    uint32_t conversionDivisor;
    std::string unit;
//    uint32_t nrBits;
} ValueEntry;


//typedef struct
//{
//    std::time_t sampleTime;
//    uint32_t WEB_Temperatur_TVL;            // "Vorlauftemperatur Heizkreis",                                         10, "°C" },
//    uint32_t WEB_Temperatur_TRL;            // "Rücklauftemperatur Heizkreis",                                        10, "°C" },
//    uint32_t WEB_Sollwert_TRL_HZ;           // "Rücklauf-Soll Heizkreis",                                             10, "°C" },
//    // ...
//} TimeValues;

typedef struct
{
    std::time_t sampleTime;
    int32_t data[69];
} TimeDataArray;

typedef union
{
    int32_t rawData[69 + 2];
    TimeDataArray timeData;
//    TimeValues timeValues;
} BufferDataRecord;


// Command Values 3004

// *INDENT-OFF*
const ValueEntry ValueTableDecode[] = {
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
//- 78|ID_WEB_Code_WP_akt|Wärmepumpentyp|0 = ERC|Typenschlüssel
//-  | | | | 1 = SW1
//-  | | | | 2 = SW2
//-  | | | | 3 = WW1
//-  | | | | 4 = WW2
//-  | | | | 5 = L1I
//-  | | | | 6 = L2I
//-  | | | | 7 = L1A
//-  | | | | 8 = L2A
//-  | | | | 9 = KSW
//-  | | | | 10 = KLW
//-  | | | | 11 = SWC
//-  | | | | 12 = LWC
//-  | | | | 13 = L2G
//-  | | | | 14 = WZS
//-  | | | | 15 = L1I407
//-  | | | | 16 = L2I407
//-  | | | | 17 = L1A407
//-  | | | | 18 = L2A407
//-  | | | | 19 = L2G407
//-  | | | | 20 = LWC407
//-  | | | | 21 = L1AREV
//-  | | | | 22 = L2AREV
//-  | | | | 23 = WWC1
//-  | | | | 24 = WWC2
//-  | | | | 25 = L2G404
//-  | | | | 26 = WZW
//-  | | | | 27 = L1S
//-  | | | | 28 = L1H
//-  | | | | 29 = L2H
//-  | | | | 30 = WZWD
//-  | | | | 31 = ERC
//-  | | | | 40 = WWB_20
//-  | | | | 41 = LD5
//-  | | | | 42 = LD7
//-  | | | | 43 = SW 37_45
//-  | | | | 44 = SW 58_69
//-  | | | | 45 = SW 29_56
//-  | | | | 46 = LD5 (230V)
//-  | | | | 47 = LD7 (230 V)
//-  | | | | 48 = LD9
//-  | | | | 49 = LD5 REV
//-  | | | | 50 = LD7 REV
//-  | | | | 51 = LD5 REV 230V
//-  | | | | 52 = LD7 REV 230V
//-  | | | | 53 = LD9 REV 230V
//-  | | | | 54 = SW 291
//-  | | | | 55 = LW SEC
//-  | | | | 56 = HMD 2
//-  | | | | 57 = MSW 4
//-  | | | | 58 = MSW 6
//-  | | | | 59 = MSW 8
//-  | | | | 60 = MSW 10
//-  | | | | 61 = MSW 12
//-  | | | | 62 = MSW 14
//-  | | | | 63 = MSW 17
//-  | | | | 64 = MSW 19
//-  | | | | 65 = MSW 23
//-  | | | | 66 = MSW 26
//-  | | | | 67 = MSW 30
//-  | | | | 68 = MSW 4S
//-  | | | | 69 = MSW 6S
//-  | | | | 70 = MSW 8S
//-  | | | | 71 = MSW 10S
//-  | | | | 72 = MSW 13S
//-  | | | | 73 = MSW 16S
//-  | | | | 74 = MSW2-6S
//-  | | | | 75 = MSW4-16
//- 79|ID_WEB_BIV_Stufe_akt|Bivalenzstufe|1 = ein Verdichter darf laufen|Betriebszustand
//-  | | | | 2 = zwei Verdichter dürfen laufen
//-  | | | | 3 = zusätzlicher Wärmeerzeuger darf mitlaufen
	{  80, /*D_WEB_WP_BZ_akt*/              "Betriebszustand",                                                    0, "enum" },
//-          0 = Heizen
//-          1 = Warmwasser
//-          2 = Schwimmbad / Photovoltaik
//-          3 = EVU Sperre
//-          4 = Abtauen
//-          5 = Keine Anforderung
//-          6 = Heizen ext. Energiequelle
//-          7 = Kühlbetrieb
//- 81|ID_WEB_SoftStand1|Softwarestand|ASCII z.B. 86 = V|ASCII
//- 82|ID_WEB_SoftStand2|Softwarestand|ASCII z.B. 51 = 3|ASCII
//- 83|ID_WEB_SoftStand3|Softwarestand|ASCII z.B. 46 = .|ASCII
//- 84|ID_WEB_SoftStand4|Softwarestand|ASCII z.B. 55 = 7|ASCII
//- 85|ID_WEB_SoftStand5|Softwarestand|ASCII z.B. 56 = 8|ASCII
//- 86|ID_WEB_SoftStand6|Softwarestand|ASCII 0 = Nichts|ASCII
//- 87|ID_WEB_SoftStand7|Softwarestand|ASCII 0 = Nichts|ASCII
//- 88|ID_WEB_SoftStand8|Softwarestand|ASCII 0 = Nichts|ASCII
//- 89|ID_WEB_SoftStand9|Softwarestand|ASCII 0 = Nichts|ASCII
//- 90|ID_WEB_SoftStand10|Softwarestand|ASCII 0 = Nichts|ASCII
//- 91|ID_WEB_AdresseIP_akt|IP Adresse|siehe Beispielcode unten|IP
//- 92|ID_WEB_SubNetMask_akt|Subnetzmaske|siehe Beispielcode unten|IP
//- 93|ID_WEB_Add_Broadcast|Broadcast Adresse|siehe Beispielcode unten|IP
//- 94|ID_WEB_Add_StdGateway|Standard Gateway|siehe Beispielcode unten|IP
//- 95|ID_WEB_ERROR_Time0|Zeitstempel Fehler 0 im Speicher|Keine|Unix Timestamp
//- 96|ID_WEB_ERROR_Time1|Zeitstempel Fehler 1 im Speicher|Keine|Unix Timestamp
//- 97|ID_WEB_ERROR_Time2|Zeitstempel Fehler 2 im Speicher|Keine|Unix Timestamp
//- 98|ID_WEB_ERROR_Time3|Zeitstempel Fehler 3 im Speicher|Keine|Unix Timestamp
//- 99|ID_WEB_ERROR_Time4|Zeitstempel Fehler 4 im Speicher|Keine|Unix Timestamp
//- 100|ID_WEB_ERROR_Nr0|Fehlercode Fehler 0 im Speicher|Keine|Fehlercode
//- 101|ID_WEB_ERROR_Nr1|Fehlercode Fehler 1 im Speicher|Keine|Fehlercode
//- 102|ID_WEB_ERROR_Nr2|Fehlercode Fehler 2 im Speicher|Keine|Fehlercode
//- 103|ID_WEB_ERROR_Nr3|Fehlercode Fehler 3 im Speicher|Keine|Fehlercode
//- 104|ID_WEB_ERROR_Nr4|Fehlercode Fehler 4 im Speicher|Keine|Fehlercode
//- 105|ID_WEB_AnzahlFehlerInSpeicher|Anzahl der Fehler im Speicher|Keine|
//- 106|ID_WEB_Switchoff_file_Nr0|Grund Abschaltung 0 im Speicher|1 = Wärmepumpe Störung|Abschaltcode
//-  | | | | 2 = Anlagen Störung
//-  | | | | 3 = Betriebsart Zweiter Wärmeerzeuger
//-  | | | | 4 = EVU-Sperre
//-  | | | | 5 = Lauftabtau (nur LW-Geräte)
//-  | | | | 6 = Temperatur Einsatzgrenze maximal
//-  | | | | 7 = Temperatur Einsatzgrenze minimal (bei LWD reversibel möglicherweise Abschaltung wegen Frostschutz bei Kühlbetrieb - Verdampfungstemperatur zu lange unter 0°C)
//-  | | | | 8 = Untere Einsatzgrenze
//-  | | | | 9 = Keine Anforderung
//- 107|ID_WEB_Switchoff_file_Nr1|Grund Abschaltung 1 im Speicher|siehe Abschaltcode 0|Abschaltcode
//- 108|ID_WEB_Switchoff_file_Nr2|Grund Abschaltung 2 im Speicher|siehe Abschaltcode 0|Abschaltcode
//- 109|ID_WEB_Switchoff_file_Nr3|Grund Abschaltung 3 im Speicher|siehe Abschaltcode 0|Abschaltcode
//- 110|ID_WEB_Switchoff_file_Nr4|Grund Abschaltung 4 im Speicher|siehe Abschaltcode 0|Abschaltcode
//- 111|ID_WEB_Switchoff_file_Time0|Zeitstempel Abschaltung 0 im Speicher|Keine|Unix Zeitstempel
//- 112|ID_WEB_Switchoff_file_Time1|Zeitstempel Abschaltung 1 im Speicher|Keine|Unix Zeitstempel
//- 113|ID_WEB_Switchoff_file_Time2|Zeitstempel Abschaltung 2 im Speicher|Keine|Unix Zeitstempel
//- 114|ID_WEB_Switchoff_file_Time3|Zeitstempel Abschaltung 3 im Speicher|Keine|Unix Zeitstempel
//- 115|ID_WEB_Switchoff_file_Time4|Zeitstempel Abschaltung 4 im Speicher|Keine|Unix Zeitstempel
//- 116|ID_WEB_Comfort_exists|Comfort Platine installiert|0 = nicht verbaut|Boolean
//-  | | | | 1 = verbaut
//- 117|ID_WEB_HauptMenuStatus_Zeile1|Status Zeile 1|0 = Wärmepumpe läuft|Statuscode
//-  | | | | 1 = Wärmepumpe steht
//-  | | | | 2 = Wärmepumpe kommt
//-  | | | | 3 = Fehlercode Speicherplatz 0
//-  | | | | 4 = Abtauen
//-  | | | | 5 = Warte auf LIN-Verbindung
//-  | | | | 6 = Verdichter heizt auf
//-  | | | | 7 = Pumpenvorlauf
//- 118|ID_WEB_HauptMenuStatus_Zeile2|Status Zeile 2|0 = seit :|Statustext
//-  | | | | 1 = in :
//- 119|ID_WEB_HauptMenuStatus_Zeile3|Status Zeile 3|0 = Heizbetrieb|Statuscode
//-  | | | | 1 = Keine Anforderung
//-  | | | | 2 = Netz-Einschaltverzögerung
//-  | | | | 3 = Schaltspielsperre
//-  | | | | 4 = Sperrzeit
//-  | | | | 5 = Brauchwasser
//-  | | | | 6 = Info Ausheizprogramm
//-  | | | | 7 = Abtauen
//-  | | | | 8 = Pumpenvorlauf
//-  | | | | 9 = Thermische Desinfektion
//-  | | | | 10 = Kühlbetrieb
//-  | | | | 12 = Schwimmbad / Photovoltaik
//-  | | | | 13 = Heizen ext. Energiequelle
//-  | | | | 14 = Brauchwasser ext. Energiequelle
//-  | | | | 16 = Durchflussüberachung
//-  | | | | 17 = Zweiter Wärmeerzeuger 1 Betrieb
//- 120|ID_WEB_HauptMenuStatus_Zeit|Zeit seit / in (in kombination mit #118)|Keine|Sekunden
//- 121|ID_WEB_HauptMenuAHP_Stufe|Stufe Ausheizprogramm|Keine|
//- 122|ID_WEB_HauptMenuAHP_Temp|Temperatur Ausheizprogramm|Wert * 0.1|°C
//- 123|ID_WEB_HauptMenuAHP_Zeit|Laufzeit Ausheizprogramm|Keine|Sekunden
//- 124|ID_WEB_SH_BWW|Brauchwasser aktiv/inaktiv Symbol|0 = inaktiv|Boolean
//-  | | | | 1 = aktiv
//- 125|ID_WEB_SH_HZ|Heizung Symbol|??|??
//- 126|ID_WEB_SH_MK1|Mischkreis 1 Symbol|??|??
//- 127|ID_WEB_SH_MK2|Mischkreis 2 Symbol|??|??
//- 128|ID_WEB_Einst_Kurzrpgramm|Einstellung Kurzprogramm|??|??
//- 129|ID_WEB_StatusSlave_1|Status Slave 1|??|??
//- 130|ID_WEB_StatusSlave_2|Status Slave 2|??|??
//- 131|ID_WEB_StatusSlave_3|Status Slave 3|??|??
//- 132|ID_WEB_StatusSlave_4|Status Slave 4|??|??
//- 133|ID_WEB_StatusSlave_5|Status Slave 5|??|??
//- 134|ID_WEB_AktuelleTimeStamp|Aktuelle Zeit der Wärmepumpe|Keine|Unix Timestamp
//- 135|ID_WEB_SH_MK3|Mischkreis 3 Symbol|??|??
//- 136|ID_WEB_Sollwert_TVL_MK3|Mischkreis 3 Vorlauf-Soll-Temperatur|Wert * 0.1|°C
//- 137|ID_WEB_Temperatur_TFB3|Mischkreis 3 Vorlauftemperatur|Wert * 0.1|°C
//- 138|ID_WEB_MZ3out|Ausgang "Mischkreis 3 Zu"|Keine|Boolean
//- 139|ID_WEB_MA3out|Ausgang "Mischkreis 3 Auf"|Keine|Boolean
//- 140|ID_WEB_FP3out|Pumpe Mischkreis 3|Keine|Boolean
//- 141|ID_WEB_Time_AbtIn|Zeit bis Abtauen|Keine|Sekunden
//- 142|ID_WEB_Temperatur_RFV2|Raumtemperatur Raumstation 2|Wert * 0.1|°C
//- 143|ID_WEB_Temperatur_RFV3|Raumtemperatur Raumstation 3|Wert * 0.1|°C
//- 144|ID_WEB_SH_SW|Schaltuhr Schwimmbad Symbol|??|??
//- 145|ID_WEB_Zaehler_BetrZeitSW|Betriebsstunden Schwimmbad|?? (Wert / 7200 = Stunden)|??
//- 146|ID_WEB_FreigabKuehl|Freigabe Kühlung|Keine|Boolean
//- 147|ID_WEB_AnalogIn|Analoges Eingangssignal|Wert / 100|V
//- 148|ID_WEB_SonderZeichen|??|??|??
//- 149|ID_WEB_SH_ZIP|Zirkulationspumpen Symbol|??|??
//- 150|ID_WEB_WebsrvProgrammWerteBeobarten|??|??|??
//- 151|ID_WEB_WMZ_Heizung|Wärmemengenzähler Heizung|Wert / 10|kWh
//- 152|ID_WEB_WMZ_Brauchwasser|Wärmemengenzähler Brauchwasser|Wert / 10|kWh
//- 153|ID_WEB_WMZ_Schwimmbad|Wärmemengenzähler Schwimmbad|Wert / 10|kWh
//- 154|ID_WEB_WMZ_Seit|Wärmemengenzähler Gesamt|Wert / 10|kWh
//- 155|ID_WEB_WMZ_Durchfluss|Wärmemengenzähler Durchfluss|Keine|l / h
//- 156|ID_WEB_AnalogOut1|Analog Ausgang 1|Wert / 100|V
//- 157|ID_WEB_AnalogOut2|Analog Ausgang 2|Wert / 100|V
//- 158|ID_WEB_Time_Heissgas|Sperre zweiter Verdichter Heissgas|Keine|Sekunden
//- 159|ID_WEB_Temp_Lueftung_Zuluft|Zulufttemperatur|Wert / 10|°C
//- 160|ID_WEB_Temp_Lueftung_Abluft|Ablufttemperatur|Wert / 10|°C
//- 161|ID_WEB_Zaehler_BetrZeitSolar|Betriebstundenzähler Solar  |Keine|Sekunden
//- 162|ID_WEB_AnalogOut3|Analog Ausgang 3|Wert / 100|V
//- 163|ID_WEB_AnalogOut4|Analog Ausgang 4|Wert / 100|V
//- 164|ID_WEB_Out_VZU|Zuluft Ventilator (Abtaufunktion)|Wert / 100|V
//- 165|ID_WEB_Out_VAB|Abluft Ventilator|Wert / 100|V
//- 166|ID_WEB_Out_VSK|Ausgang VSK|Keine|Boolean
//- 167|ID_WEB_Out_FRH|Ausgang FRH|Keine|Boolean
//- 168|ID_WEB_AnalogIn2|Analog Eingang 2|Wert / 100|V
//- 169|ID_WEB_AnalogIn3|Analog Eingang 3|Wert / 100|V
//- 170|ID_WEB_SAXin|Eingang SAX|Keine|Boolean
//- 171|ID_WEB_SPLin|Eingang SPL|Keine|Boolean
//- 172|ID_WEB_Compact_exists|Lüftungsplatine verbaut|0 = nicht verbaut|Boolean
//-  | | | | 1 = verbaut
//- 173|ID_WEB_Durchfluss_WQ|Durchfluss Wärmequelle|Keine|l / h
//- 174|ID_WEB_LIN_exists|LIN BUS verbaut|0 = nicht verbaut|Boolean
//-  | | | | 1 = verbaut
//- 175|ID_WEB_LIN_ANSAUG_VERDAMPFER|Temperatur Ansaug Verdampfer|Wert / 10|°C
//- 176|ID_WEB_LIN_ANSAUG_VERDICHTER|Temperatur Ansaug Verdichter|Wert / 10|°C
//- 177|ID_WEB_LIN_VDH|Temperatur Verdichter Heizung|Wert / 10|°C
//- 178|ID_WEB_LIN_UH|Überhitzung|Wert / 10|K
//- 179|ID_WEB_LIN_UH_Soll|Überhitzung Soll |Wert / 10|K
//- 180|ID_WEB_LIN_HD|Hochdruck|Wert / 100|bar
//- 181|ID_WEB_LIN_ND|Niederdruck|Wert / 100|bar
//- 182|ID_WEB_LIN_VDH_out|Ausgang Verdichterheizung|Keine|Boolean
//- 183|ID_WEB_HZIO_PWM|Steuersignal Umwälzpumpe|Wert / 10|%
//- 184|ID_WEB_HZIO_VEN|Ventilator Drehzahl|Keine|U / min
//- 185|ID_WEB_HZIO_EVU2|EVU 2|??|??
//- 186|ID_WEB_HZIO_STB|Sicherheits-Tempeartur-Begrenzer Fussbodenheizung|Keine|Boolean
//- 187|ID_WEB_SEC_Qh_Soll|Leistung Sollwert|Wert / 100|kWh
//- 188|ID_WEB_SEC_Qh_Ist|Leistung Istwert|Wert / 100|kWh
//- 189|ID_WEB_SEC_TVL_Soll|Temperatur Vorlauf Soll|Wert / 10|°C
//- 190|ID_WEB_SEC_Software|Software Stand SEC Board|??|??
//- 191|ID_WEB_SEC_BZ|Betriebszustand SEC Board|0 = Aus|Betriebszustand
//-  | | | | 1 = Kühlung
//-  | | | | 2 = Heizung
//-  | | | | 3 = Störung
//-  | | | | 4 = Übergang
//-  | | | | 5 = Abtauen
//-  | | | | 6 = Warte
//-  | | | | 7 = Warte
//-  | | | | 8 = Übergang
//-  | | | | 9 = Stop
//-  | | | | 10 = Manuell
//-  | | | | 11 = Simulation Start
//-  | | | | 12 = EVU Sperre
//- 192|ID_WEB_SEC_VWV|Vierwegeventil|??|??
//- 193|ID_WEB_SEC_VD|Verdichterdrehzahl|Keine|U / min
//- 194|ID_WEB_SEC_VerdEVI|Verdichtertemperatur EVI (Enhanced Vapour Injection)|Wert / 10|°C
//- 195|ID_WEB_SEC_AnsEVI|Ansaugtemperatur EVI|Wert / 10|°C
//- 196|ID_WEB_SEC_UEH_EVI|Überhitzung EVI|Wert / 10|K
//- 197|ID_WEB_SEC_UEH_EVI_S|Überhitzung EVI Sollwert|Wert / 10|K
//- 198|ID_WEB_SEC_KondTemp|Kondensationstemperatur|Wert / 10|°C
//- 199|ID_WEB_SEC_FlussigEx|Flüssigtemperatur EEV (elektronisches Expansionsventil)|Wert / 10|°C
//- 200|ID_WEB_SEC_UK_EEV|Unterkühlung EEV|Wert / 10|°C
//- 201|ID_WEB_SEC_EVI_Druck|Druck EVI|Wert / 100|bar
//- 202|ID_WEB_SEC_U_Inv|Spannung Inverter|Wert / 10|V
//- 203|ID_WEB_Temperatur_THG_2|Temperarturfühler Heissgas 2|Wert / 10|°C
//- 204|ID_WEB_Temperatur_TWE_2|Temperaturfühler Wärmequelleneintritt 2|Wert / 10|°C
//- 205|ID_WEB_LIN_ANSAUG_VERDAMPFER_2|Ansaugtemperatur Verdampfer 2|Wert / 10|°C
//- 206|ID_WEB_LIN_ANSAUG_VERDICHTER_2|Ansaugtemperatur Verdichter 2|Wert / 10|°C
//- 207|ID_WEB_LIN_VDH_2|Temperatur Verdichter 2 Heizung|Wert / 10|°C
//- 208|ID_WEB_LIN_UH_2|Überhitzung 2|Wert / 10|K
//- 209|ID_WEB_LIN_UH_Soll_2|Überhitzung Soll 2|Wert / 10|K
//- 210|ID_WEB_LIN_HD_2|Hochdruck 2|Wert / 100|bar
//- 211|ID_WEB_LIN_ND_2|Niederdruck 2|Wert / 100|bar
//- 212|ID_WEB_HDin_2|Eingang Druckschalter Hochdruck 2|Keine|Boolean
//- 213|ID_WEB_AVout_2|Ausgang Abtauventil 2|Keine|Boolean
//- 214|ID_WEB_VBOout_2|Ausgang Solepumpe/Ventilator 2|Keine|Boolean
//- 215|ID_WEB_VD1out_2|Ausgang Verdichter 1 / 2|Keine|Boolean
//- 216|ID_WEB_LIN_VDH_out_2|Ausgang Verdichter Heizung 2|Keine|Boolean
//- 217|ID_WEB_Switchoff2_file_Nr0|Grund Abschaltung 0 im Speicher|1 = Wärmepumpe Störung|Abschaltcode
//-  | | | | 2 = Anlagen Störung
//-  | | | | 3 = Betriebsart Zweiter Wärmeerzeuger
//-  | | | | 4 = EVU-Sperre
//-  | | | | 5 = Lauftabtau (nur LW-Geräte)
//-  | | | | 6 = Temperatur Einsatzgrenze maximal
//-  | | | | 7 = Temperatur Einsatzgrenze minimal (bei LWD reversibel möglicherweise Abschaltung wegen Frostschutz bei Kühlbetrieb - Verdampfungstemperatur zu lange unter 0°C)
//-  | | | | 8 = Untere Einsatzgrenze
//-  | | | | 9 = Keine Anforderung
//- 218|ID_WEB_Switchoff2_file2_Nr1|Grund Abschaltung 1 im Speicher|siehe Abschaltcode 0|Abschaltcode
//- 219|ID_WEB_Switchoff2_file2_Nr2|Grund Abschaltung 2 im Speicher|siehe Abschaltcode 0|Abschaltcode
//- 220|ID_WEB_Switchoff2_file2_Nr3|Grund Abschaltung 3 im Speicher|siehe Abschaltcode 0|Abschaltcode
//- 221|ID_WEB_Switchoff2_file2_Nr4|Grund Abschaltung 4 im Speicher|siehe Abschaltcode 0|Abschaltcode
//- 222|ID_WEB_Switchoff2_file_Time0|Zeitstempel Abschaltung 0 im Speicher|Keine|Unix Zeitstempel
//- 223|ID_WEB_Switchoff2_file_Time1|Zeitstempel Abschaltung 1 im Speicher|Keine|Unix Zeitstempel
//- 224|ID_WEB_Switchoff2_file_Time2|Zeitstempel Abschaltung 2 im Speicher|Keine|Unix Zeitstempel
//- 225|ID_WEB_Switchoff2_file_Time3|Zeitstempel Abschaltung 3 im Speicher|Keine|Unix Zeitstempel
//- 226|ID_WEB_Switchoff2_file_Time4|Zeitstempel Abschaltung 4 im Speicher|Keine|Unix Zeitstempel
//- 227|ID_WEB_RBE_RT_Ist|Raumtemperatur Istwert|Wert / 10|°C
//- 228|ID_WEB_RBE_RT_Soll|Raumtemperatur Sollwert|Wert / 10|°C
//- 229|ID_WEB_Temperatur_BW_oben|Temperatur Brauchwasser Oben|Wert / 10|°C
//- 230|ID_WEB_Code_WP_akt_2|Wärmepumpen Typ 2|0 = ERC|Typenschlüssel
//-  | | | | 1 = SW1
//-  | | | | 2 = SW2
//-  | | | | 3 = WW1
//-  | | | | 4 = WW2
//-  | | | | 5 = L1I
//-  | | | | 6 = L2I
//-  | | | | 7 = L1A
//-  | | | | 8 = L2A
//-  | | | | 9 = KSW
//-  | | | | 10 = KLW
//-  | | | | 11 = SWC
//-  | | | | 12 = LWC
//-  | | | | 13 = L2G
//-  | | | | 14 = WZS
//-  | | | | 15 = L1I407
//-  | | | | 16 = L2I407
//-  | | | | 17 = L1A407
//-  | | | | 18 = L2A407
//-  | | | | 19 = L2G407
//-  | | | | 20 = LWC407
//-  | | | | 21 = L1AREV
//-  | | | | 22 = L2AREV
//-  | | | | 23 = WWC1
//-  | | | | 24 = WWC2
//-  | | | | 25 = L2G404
//-  | | | | 26 = WZW
//-  | | | | 27 = L1S
//-  | | | | 28 = L1H
//-  | | | | 29 = L2H
//-  | | | | 30 = WZWD
//-  | | | | 31 = ERC
//-  | | | | 40 = WWB_20
//-  | | | | 41 = LD5
//-  | | | | 42 = LD7
//-  | | | | 43 = SW 37_45
//-  | | | | 44 = SW 58_69
//-  | | | | 45 = SW 29_56
//-  | | | | 46 = LD5 (230V)
//-  | | | | 47 = LD7 (230 V)
//-  | | | | 48 = LD9
//-  | | | | 49 = LD5 REV
//-  | | | | 50 = LD7 REV
//-  | | | | 51 = LD5 REV 230V
//-  | | | | 52 = LD7 REV 230V
//-  | | | | 53 = LD9 REV 230V
//-  | | | | 54 = SW 291
//-  | | | | 55 = LW SEC
//-  | | | | 56 = HMD 2
//-  | | | | 57 = MSW 4
//-  | | | | 58 = MSW 6
//-  | | | | 59 = MSW 8
//-  | | | | 60 = MSW 10
//-  | | | | 61 = MSW 12
//-  | | | | 62 = MSW 14
//-  | | | | 63 = MSW 17
//-  | | | | 64 = MSW 19
//-  | | | | 65 = MSW 23
//-  | | | | 66 = MSW 26
//-  | | | | 67 = MSW 30
//-  | | | | 68 = MSW 4S
//-  | | | | 69 = MSW 6S
//-  | | | | 70 = MSW 8S
//-  | | | | 71 = MSW 10S
//-  | | | | 72 = MSW 13S
//-  | | | | 73 = MSW 16S
//-  | | | | 74 = MSW2-6S
//-  | | | | 75 = MSW4-16
//- 231|ID_WEB_Freq_VD|Verdichterfrequenz|Keine|Hz
};
// *INDENT-ON*
