#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdint.h>

#include <iostream>
#include <string>

#include "DataDecoder/src/FileDataReader.hpp"
#include "DataDecoder/src/IFileDataWriterCSV.hpp"

namespace
{
using ::testing::StrictMock;
using ::testing::Return;
// using ::testing::Exactly;
using ::testing::_;
// using ::testing::HasSubstr;

class MockFileDataWriterCSV : public IFileDataWriterCSV
{
public:
    MOCK_METHOD(bool, writeHeader, (std::string headerLine), (override));
    MOCK_METHOD(bool, writeData, (std::vector<std::uint32_t> dataVector, const uint32_t nrColumnExclTimeStamp), (override));
};

// class Test_FileDataReader : public testing::Test, public BitBuffer
class Test_FileDataReader : public testing::Test
{
protected:
    void SetUp() override
    {
        srand(time(NULL));
        csvWriter = std::make_shared<MockFileDataWriterCSV>();
    }

    void TearDown() override
    {
        // Dump Trace buffer
    }

    std::shared_ptr<MockFileDataWriterCSV> csvWriter;
    FileDataReader_SPtr fileDataReader;
};



TEST_F(Test_FileDataReader, fileVersion1)
{
    bool feedback;
    fileDataReader = std::make_shared<FileDataReader>("Test/testData/fileVersion_01.dat", csvWriter); // real test
//    fileDataReader = std::make_shared<FileDataReader>("../testData/fileVersion_01.dat", csvWriter); //debugger
    feedback = fileDataReader->readHeaderData();
    EXPECT_TRUE(feedback);

    std::string expectedHeaderLine("Time,Vorlauftemperatur Heizkreis,Rücklauftemperatur Heizkreis,Rücklauf-Soll Heizkreis,"
                                   "Rücklauftemperatur im Trennspeicher,Heisgastemperatur,Aussentemperatur,"
                                   "Durchschnittstemperatur Aussen über 24 h (Funktion Heizgrenze),Warmwasser Ist-Temperatur,"
                                   "Warmwasser Soll-Temperatur,Wärmequellen-Eintrittstemperatur,Wärmequellen-Austrittstemperatur,"
                                   "Mischkreis 1 Vorlauftemperatur,Mischkreis 1 Vorlauf-Soll-Temperatur,Raumtemperatur Raumstation 1,"
                                   "Mischkreis 2 Vorlauftemperatur,Mischkreis 2 Vorlauf-Soll-Temperatur,Fühler Solarkollektor,"
                                   "Fühler Solarspeicher,Fühler externe Energiequelle,Eingang Abtauende / Soledruck / Durchfluss,"
                                   "Brauchwarmwasserthermostat,EVU Sperre,Hochdruck Kältekreis,Motorschutz OK,Niederdruck,"
                                   "Überwachungskontakt für Potentiostat,Schwimmbadthermostat,Abtauventil,Brauchwasserpumpe/Umstellventil,"
                                   "Heizungsumwälzpumpe,Mischkreis 1 Auf,Mischkreis 1 Zu,Ventilation (Lüftung),Solepumpe/Ventilator,"
                                   "Verdichter 1,Verdichter 2,Zirkulationspumpe,Zusatzumwälzpumpe,Steuersignal Zusatzheizung v. Heizung,"
                                   "Steuersignal Zusatzheizung/Störsignal,Zusatzheizung 3,Pumpe Mischkreis 2,Solarladepumpe,"
                                   "Schwimmbadpumpe,Mischkreis 2 Zu,Mischkreis 2 Auf,Betriebsstunden Verdichter 1,Impulse Verdichter 1,"
                                   "Betriebsstunden Verdichter 2,Impulse Verdichter 2,Betriebsstunden Zweiter Wärmeerzeuger 1,"
                                   "Betriebsstunden Zweiter Wärmeerzeuger 2,Betriebsstunden Zweiter Wärmeerzeuger 3,"
                                   "Betriebsstunden Wärmepumpe,Betriebsstunden Heizung,Betriebsstunden Warmwasser,"
                                   "Betriebsstunden Kühlung,Wärmepumpe läuft seit,Zweiter Wärmeerzeuger 1 läuft seit,"
                                   "Zweiter Wärmeerzeuger 2 läuft seit,Netzeinschaltverzögerung,Schaltspielsperre Aus,"
                                   "Schaltspielsperre Ein,Verdichter-Standzeit,Heizungsregler Mehr-Zeit,Heizungsregler Weniger-Zeit,"
                                   "Thermische Desinfektion läuft seit,Sperre Warmwasser,Betriebszustand");

    uint32_t expecedNrColExclusiveTimeStamp = 69;

    std::vector<uint32_t> expectedVectorCSV(2 * (expecedNrColExclusiveTimeStamp + 2));
    expectedVectorCSV.at(  0) = 0x5e6677B4;       // B4 77 66 5E
    expectedVectorCSV.at(  1) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(  2) = 0x00000110;       // 10 01 00 00
    expectedVectorCSV.at(  3) = 0x00000110;       // 10 01 00 00
    expectedVectorCSV.at(  4) = 0x00000116;       // 16 01 00 00
    expectedVectorCSV.at(  5) = 0x00000032;       // 32 00 00 00
    expectedVectorCSV.at(  6) = 0x0000018C;       // 8C 01 00 00
    expectedVectorCSV.at(  7) = 0x0000001C;       // 1C 00 00 00
    expectedVectorCSV.at(  8) = 0x00000028;       // 28 00 00 00
    expectedVectorCSV.at(  9) = 0x000001EB;       // EB 01 00 00
    expectedVectorCSV.at( 10) = 0x000001F4;       // F4 01 00 00
    expectedVectorCSV.at( 11) = 0x00000058;       // 58 00 00 00
    expectedVectorCSV.at( 12) = 0x00000062;       // 62 00 00 00
    expectedVectorCSV.at( 13) = 0x000002EE;       // EE 02 00 00
    expectedVectorCSV.at( 14) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 15) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 16) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 17) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 18) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 19) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 20) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 21) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at( 22) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 23) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 24) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 25) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at( 26) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at( 27) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 28) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 29) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 30) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 31) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at( 32) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 33) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at( 34) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 35) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 36) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 37) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 38) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 39) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 40) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 41) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 42) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 43) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 44) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 45) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 46) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 47) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 48) = 0x01644F2E;       // 2E 4F 64 01
    expectedVectorCSV.at( 49) = 0x00002EF3;       // F3 2E 00 00
    expectedVectorCSV.at( 50) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 51) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 52) = 0x002E47DF;       // DF 47 2E 00
    expectedVectorCSV.at( 53) = 0x003613A4;       // A4 13 36 00
    expectedVectorCSV.at( 54) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 55) = 0x01644F2E;       // 2E 4F 64 01
    expectedVectorCSV.at( 56) = 0x010F9930;       // 30 99 0F 01
    expectedVectorCSV.at( 57) = 0x0054B5D7;       // D7 B5 54 00
    expectedVectorCSV.at( 58) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 59) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at( 60) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 61) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 62) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 63) = 0x00000002;       // 02 00 00 00
    expectedVectorCSV.at( 64) = 0x000000DF;       // DF 00 00 00
    expectedVectorCSV.at( 65) = 0x00000129;       // 29 01 00 00
    expectedVectorCSV.at( 66) = 0x00000020;       // 20 00 00 00
    expectedVectorCSV.at( 67) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 68) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 69) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 70) = 0x00000003;       // 03 00 00 00
    expectedVectorCSV.at( 71) = 0x5E6677F0;       // F0 77 66 5E
    expectedVectorCSV.at( 72) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 73) = 0x0000010E;       // 0E 01 00 00
    expectedVectorCSV.at( 74) = 0x0000010E;       // 0E 01 00 00
    expectedVectorCSV.at( 75) = 0x00000116;       // 16 01 00 00
    expectedVectorCSV.at( 76) = 0x00000032;       // 32 00 00 00
    expectedVectorCSV.at( 77) = 0x0000017C;       // 7C 01 00 00
    expectedVectorCSV.at( 78) = 0x0000001B;       // 1B 00 00 00
    expectedVectorCSV.at( 79) = 0x00000028;       // 28 00 00 00
    expectedVectorCSV.at( 80) = 0x000001EB;       // EB 01 00 00
    expectedVectorCSV.at( 81) = 0x000001F4;       // F4 01 00 00
    expectedVectorCSV.at( 82) = 0x00000059;       // 59 00 00 00
    expectedVectorCSV.at( 83) = 0x00000063;       // 63 00 00 00
    expectedVectorCSV.at( 84) = 0x000002EE;       // EE 02 00 00
    expectedVectorCSV.at( 85) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 86) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 87) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 88) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 89) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 90) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 91) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 92) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at( 93) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 94) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 95) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 96) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at( 97) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at( 98) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at( 99) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(100) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(101) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(102) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(103) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(104) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(105) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(106) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(107) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(108) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(109) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(110) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(111) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(112) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(113) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(114) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(115) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(116) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(117) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(118) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(119) = 0x01644F2E;       // 2E 4F 64 01
    expectedVectorCSV.at(120) = 0x00002EF3;       // F3 2E 00 00
    expectedVectorCSV.at(121) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(122) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(123) = 0x002E47DF;       // DF 47 2E 00
    expectedVectorCSV.at(124) = 0x003613A4;       // A4 13 36 00
    expectedVectorCSV.at(125) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(126) = 0x01644F2E;       // 2E 4F 64 01
    expectedVectorCSV.at(127) = 0x010F9930;       // 30 99 0F 01
    expectedVectorCSV.at(128) = 0x0054B5D7;       // D7 B5 54 00
    expectedVectorCSV.at(129) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(130) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(131) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(132) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(133) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(134) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(135) = 0x000000A3;       // A3 00 00 00
    expectedVectorCSV.at(136) = 0x00000165;       // 65 01 00 00
    expectedVectorCSV.at(137) = 0x00000020;       // 20 00 00 00
    expectedVectorCSV.at(138) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(139) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(140) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(141) = 0x00000003;       // 03 00 00 00

    EXPECT_CALL(*csvWriter, writeHeader(expectedHeaderLine)).WillOnce(Return(true));
    EXPECT_CALL(*csvWriter, writeData(expectedVectorCSV, expecedNrColExclusiveTimeStamp)).WillOnce(Return(true));
    feedback = fileDataReader->decodeData();
    EXPECT_TRUE(feedback);
}

TEST_F(Test_FileDataReader, fileVersion2)
{
    bool feedback;
    fileDataReader = std::make_shared<FileDataReader>("Test/testData/fileVersion_02.dat", csvWriter); // real test
    feedback = fileDataReader->readHeaderData();
    EXPECT_TRUE(feedback);

    std::string expectedHeaderLine("Time," "Vorlauftemperatur Heizkreis," "Rücklauftemperatur Heizkreis," "Rücklauf-Soll Heizkreis," "Heisgastemperatur,"
                                   "Aussentemperatur," "Durchschnittstemperatur Aussen über 24 h (Funktion Heizgrenze),"
                                   "Warmwasser Ist-Temperatur," "Warmwasser Soll-Temperatur," "Wärmequellen-Eintrittstemperatur,"
                                   "Wärmequellen-Austrittstemperatur," "Eingang Abtauende / Soledruck / Durchfluss," "Brauchwarmwasserthermostat,"
                                   "EVU Sperre," "Hochdruck Kältekreis," "Motorschutz OK," "Niederdruck," "Überwachungskontakt für Potentiostat,"
                                   "Abtauventil," "Brauchwasserpumpe/Umstellventil," "Heizungsumwälzpumpe," "Ventilation (Lüftung)," "Solepumpe/Ventilator,"
                                   "Verdichter 1," "Zirkulationspumpe," "Zusatzumwälzpumpe," "Steuersignal Zusatzheizung v. Heizung,"
                                   "Steuersignal Zusatzheizung/Störsignal," "Betriebsstunden Verdichter 1," "Impulse Verdichter 1,"
                                   "Betriebsstunden Zweiter Wärmeerzeuger 2," "Betriebsstunden Wärmepumpe," "Betriebsstunden Heizung,"
                                   "Betriebsstunden Warmwasser," "Wärmepumpe läuft seit," "Zweiter Wärmeerzeuger 2 läuft seit," "Netzeinschaltverzögerung,"
                                   "Schaltspielsperre Aus," "Schaltspielsperre Ein," "Verdichter-Standzeit," "Heizungsregler Mehr-Zeit,"
                                   "Heizungsregler Weniger-Zeit," "Thermische Desinfektion läuft seit," "Sperre Warmwasser,"
                                   "Wärmepumpentyp|0 = ERC|Typenschlüssel," "Bivalenzstufe," "Betriebszustand," "Wärmemengenzähler Heizung,"
                                   "Wärmemengenzähler Brauchwasser," "Wärmemengenzähler Gesamt," "Wärmemengenzähler Durchfluss");

    uint32_t expecedNrColExclusiveTimeStamp = 50;

    std::vector<uint32_t> expectedVectorCSV(3 * (expecedNrColExclusiveTimeStamp + 2));
    expectedVectorCSV.at(  0 ) = 0x5E7A90F0;     // F0 90 7A 5E
    expectedVectorCSV.at(  1 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at(  2 ) = 0x00000173;     // 73 01 00 00
    expectedVectorCSV.at(  3 ) = 0x0000013E;     // 3E 01 00 00
    expectedVectorCSV.at(  4 ) = 0x00000123;     // 23 01 00 00
    expectedVectorCSV.at(  5 ) = 0x0000027F;     // 7F 02 00 00
    expectedVectorCSV.at(  6 ) = 0xFFFFFFEF;     // EF FF FF FF
    expectedVectorCSV.at(  7 ) = 0xFFFFFFFF;     // FF FF FF FF
    expectedVectorCSV.at(  8 ) = 0x000001D8;     // D8 01 00 00
    expectedVectorCSV.at(  9 ) = 0x000001F4;     // F4 01 00 00
    expectedVectorCSV.at( 10 ) = 0x00000042;     // 42 00 00 00
    expectedVectorCSV.at( 11 ) = 0x00000024;     // 24 00 00 00
    expectedVectorCSV.at( 12 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at( 13 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 14 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at( 15 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 16 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at( 17 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at( 18 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 19 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 20 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 21 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at( 22 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 23 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at( 24 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at( 25 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 26 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at( 27 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 28 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 29 ) = 0x0168BC84;     // 84 BC 68 01
    expectedVectorCSV.at( 30 ) = 0x00002F51;     // 51 2F 00 00
    expectedVectorCSV.at( 31 ) = 0x003693FF;     // FF 93 36 00
    expectedVectorCSV.at( 32 ) = 0x0168BC84;     // 84 BC 68 01
    expectedVectorCSV.at( 33 ) = 0x011357EB;     // EB 57 13 01
    expectedVectorCSV.at( 34 ) = 0x00556472;     // 72 64 55 00
    expectedVectorCSV.at( 35 ) = 0x00000C17;     // 17 0C 00 00
    expectedVectorCSV.at( 36 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 37 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 38 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 39 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 40 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 41 ) = 0x00000118;     // 18 01 00 00
    expectedVectorCSV.at( 42 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 43 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 44 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 45 ) = 0x0000003C;     // 3C 00 00 00
    expectedVectorCSV.at( 46 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at( 47 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 48 ) = 0x0008E4D0;     // D0 E4 08 00
    expectedVectorCSV.at( 49 ) = 0x0002AD4E;     // 4E AD 02 00
    expectedVectorCSV.at( 50 ) = 0x000B9631;     // 31 96 0B 00
    expectedVectorCSV.at( 51 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 52 ) = 0x5E7A912C;     // 2C 91 7A 5E
    expectedVectorCSV.at( 53 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 54 ) = 0x00000173;     // 73 01 00 00
    expectedVectorCSV.at( 55 ) = 0x0000013E;     // 3E 01 00 00
    expectedVectorCSV.at( 56 ) = 0x00000123;     // 23 01 00 00
    expectedVectorCSV.at( 57 ) = 0x00000280;     // 80 02 00 00
    expectedVectorCSV.at( 58 ) = 0xFFFFFFEE;     // EE FF FF FF
    expectedVectorCSV.at( 59 ) = 0xFFFFFFFF;     // FF FF FF FF
    expectedVectorCSV.at( 60 ) = 0x000001D7;     // D7 01 00 00
    expectedVectorCSV.at( 61 ) = 0x000001F4;     // F4 01 00 00
    expectedVectorCSV.at( 62 ) = 0x00000042;     // 42 00 00 00
    expectedVectorCSV.at( 63 ) = 0x00000025;     // 25 00 00 00
    expectedVectorCSV.at( 64 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at( 65 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 66 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at( 67 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 68 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at( 69 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at( 70 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 71 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 72 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 73 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at( 74 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 75 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at( 76 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at( 77 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 78 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at( 79 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 80 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 81 ) = 0x0168BCC0;     // C0 BC 68 01
    expectedVectorCSV.at( 82 ) = 0x00002F51;     // 51 2F 00 00
    expectedVectorCSV.at( 83 ) = 0x003693FF;     // FF 93 36 00
    expectedVectorCSV.at( 84 ) = 0x0168BCC0;     // C0 BC 68 01
    expectedVectorCSV.at( 85 ) = 0x01135827;     // 27 58 13 01
    expectedVectorCSV.at( 86 ) = 0x00556472;     // 72 64 55 00
    expectedVectorCSV.at( 87 ) = 0x00000C53;     // 53 0C 00 00
    expectedVectorCSV.at( 88 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 89 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 90 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 91 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 92 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 93 ) = 0x00000118;     // 18 01 00 00
    expectedVectorCSV.at( 94 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 95 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 96 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at( 97 ) = 0x0000003C;     // 3C 00 00 00
    expectedVectorCSV.at( 98 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at( 99 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at(100 ) = 0x0008E4D2;     // D2 E4 08 00
    expectedVectorCSV.at(101 ) = 0x0002AD4E;     // 4E AD 02 00
    expectedVectorCSV.at(102 ) = 0x000B9633;     // 33 96 0B 00
    expectedVectorCSV.at(103 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at(104 ) = 0x5E7A9168;     // 68 91 7A 5E
    expectedVectorCSV.at(105 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at(106 ) = 0x00000173;     // 73 01 00 00
    expectedVectorCSV.at(107 ) = 0x0000013F;     // 3F 01 00 00
    expectedVectorCSV.at(108 ) = 0x00000123;     // 23 01 00 00
    expectedVectorCSV.at(109 ) = 0x00000281;     // 81 02 00 00
    expectedVectorCSV.at(110 ) = 0xFFFFFFEF;     // EF FF FF FF
    expectedVectorCSV.at(111 ) = 0xFFFFFFFF;     // FF FF FF FF
    expectedVectorCSV.at(112 ) = 0x000001D8;     // D8 01 00 00
    expectedVectorCSV.at(113 ) = 0x000001F4;     // F4 01 00 00
    expectedVectorCSV.at(114 ) = 0x00000042;     // 42 00 00 00
    expectedVectorCSV.at(115 ) = 0x00000024;     // 24 00 00 00
    expectedVectorCSV.at(116 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at(117 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at(118 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at(119 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at(120 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at(121 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at(122 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at(123 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at(124 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at(125 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at(126 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at(127 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at(128 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at(129 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at(130 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at(131 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at(132 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at(133 ) = 0x0168BCFC;     // FC BC 68 01
    expectedVectorCSV.at(134 ) = 0x00002F51;     // 51 2F 00 00
    expectedVectorCSV.at(135 ) = 0x003693FF;     // FF 93 36 00
    expectedVectorCSV.at(136 ) = 0x0168BCFC;     // FC BC 68 01
    expectedVectorCSV.at(137 ) = 0x01135863;     // 63 58 13 01
    expectedVectorCSV.at(138 ) = 0x00556472;     // 72 64 55 00
    expectedVectorCSV.at(139 ) = 0x00000C8F;     // 8F 0C 00 00
    expectedVectorCSV.at(140 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at(141 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at(142 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at(143 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at(144 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at(145 ) = 0x00000118;     // 18 01 00 00
    expectedVectorCSV.at(146 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at(147 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at(148 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at(149 ) = 0x0000003C;     // 3C 00 00 00
    expectedVectorCSV.at(150 ) = 0x00000001;     // 01 00 00 00
    expectedVectorCSV.at(151 ) = 0x00000000;     // 00 00 00 00
    expectedVectorCSV.at(152 ) = 0x0008E4D4;     // D4 E4 08 00
    expectedVectorCSV.at(153 ) = 0x0002AD4E;     // 4E AD 02 00
    expectedVectorCSV.at(154 ) = 0x000B9635;     // 35 96 0B 00
    expectedVectorCSV.at(155 ) = 0x00000000;     // 00 00 00 00

    EXPECT_CALL(*csvWriter, writeHeader(expectedHeaderLine)).WillOnce(Return(true));
    EXPECT_CALL(*csvWriter, writeData(expectedVectorCSV, expecedNrColExclusiveTimeStamp)).WillOnce(Return(true));
    feedback = fileDataReader->decodeData();
    EXPECT_TRUE(feedback);
}

}  // unnamed namespace
