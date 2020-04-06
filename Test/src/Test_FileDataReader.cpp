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
    MOCK_METHOD(bool, writeData, (std::vector<IFileDataWriterCSV::DataEntryCSV> dataVector, const uint32_t nrColumnExclTimeStamp), (override));
};

class Test_FileDataReader : public testing::Test
{
protected:
    void SetUp() override
    {
        m_enableLog = false;
        srand(time(NULL));
        csvWriter = std::make_shared<MockFileDataWriterCSV>();
        expectedHeaderLine_v1 = std::make_shared<std::string>("Time,Vorlauftemperatur Heizkreis,Rücklauftemperatur Heizkreis,Rücklauf-Soll Heizkreis,"
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

        expectedHeaderLine_v2_v3 = std::make_shared<std::string>("Time," "Vorlauftemperatur Heizkreis," "Rücklauftemperatur Heizkreis," "Rücklauf-Soll Heizkreis," "Heisgastemperatur,"
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

        expectedUnitLine_v1 = std::make_shared<std::string>("-,°C,°C,°C,°C,°C,°C,°C,°C,°C,°C,°C,°C,°C,°C,°C,°C,°C,°C,°C,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,"
                                                            "-,-,-,-,-,-,Sekunden,Impulse,Sekunden,Impulse,Sekunden,Sekunden,Sekunden,Sekunden,Sekunden,Sekunden,"
                                                            "Sekunden,Sekunden,Sekunden,Sekunden,Sekunden,Sekunden,Sekunden,Sekunden,Sekunden,Sekunden,Sekunden,"
                                                            "Sekunden,enum");


        expectedUnitLine_v2_v3 = std::make_shared<std::string>("-,°C,°C,°C,°C,°C,°C,°C,°C,°C,°C,bool,bool,bool,bool,bool,bool,bool,bool,bool,bool,bool,bool,bool,"
                                                               "bool,bool,bool,bool,Sekunden,Impulse,Sekunden,"
                                                               "Sekunden,Sekunden,Sekunden,Sekunden,Sekunden,Sekunden,Sekunden,Sekunden,Sekunden,Sekunden,"
                                                               "Sekunden,Sekunden,Sekunden,enum,enum,enum,kWh,kWh,kWh,l/h");

        std::vector<uint32_t> v1{10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        expectedDivisor_v1 = v1;
        EXPECT_EQ(69, expectedDivisor_v1.size());

        std::vector<uint32_t> v2{10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                 1, 1, 1, 1, 1, 1, 1, 1, 10, 10, 10, 1};

        expectedDivisor_v2_v3 = v2;
        EXPECT_EQ(50, expectedDivisor_v2_v3.size());
    }

    void TearDown() override
    {
        // Dump Trace buffer
    }

    std::shared_ptr<MockFileDataWriterCSV> csvWriter;
    FileDataReader_SPtr fileDataReader;
    std::shared_ptr<std::string> expectedHeaderLine_v1;
    std::shared_ptr<std::string> expectedHeaderLine_v2_v3;
    std::shared_ptr<std::string> expectedUnitLine_v1;
    std::shared_ptr<std::string> expectedUnitLine_v2_v3;
    std::vector<uint32_t> expectedDivisor_v1;
    std::vector<uint32_t> expectedDivisor_v2_v3;
    std::string m_filePath = "Test/testData/";  // real test
//    std::string m_filePath("../testData/"); //debugger
    bool m_enableLog;
};

MATCHER_P (compareVectors, expVec, "")
{
    bool feedback = true;
    if(expVec.size() != arg.size()) {
        std::cout << "Exp len: " << expVec.size() << " length is not act len: " << arg.size() << std::endl;
        feedback = false;
    } else {
        for(uint32_t cnt = 0; cnt < arg.size(); cnt++) {
            // Compare value if unsigned
            {
                auto* uExp = std::get_if<uint32_t>(&expVec.at(cnt).uiVal);
                auto* uAct = std::get_if<uint32_t>(&arg.at(cnt).uiVal);
                if(nullptr == uExp) {
                    if(nullptr != uAct) {
                        std::cout << "ERROR (" << cnt << ") ln: " << __LINE__ << std::endl;
                        feedback = false;
                    }
                } else {
                    if(nullptr == uAct) {
                        std::cout << "ERROR ln: " << __LINE__ << std::endl;
                        feedback = false;
                    } else {
                        // Compare values
                        if(*uExp != *uAct) {
                            std::cout << "expect value(" << cnt << ") is " << *uExp << " != actual(" << cnt << ") is " << *uAct << std::endl;
                            feedback = false;
                        }
                    }
                }
            }


            // Compare value if signed
            {
                auto* sExp = std::get_if<int32_t>(&expVec.at(cnt).uiVal);
                auto* sAct = std::get_if<int32_t>(&arg.at(cnt).uiVal);
                if(nullptr == sExp) {
                    if(nullptr != sAct) {
                        std::cout << "ERROR (" << cnt << ") ln: " << __LINE__ << std::endl;
                        feedback = false;
                    }
                } else {
                    if(nullptr == sAct) {
                        std::cout << "ERROR ln: " << __LINE__ << std::endl;
                        feedback = false;
                    } else {
                        // Compare values
                        if(*sExp != *sAct) {
                            std::cout << "expect value(" << cnt << ") is " << *sExp << " != actual(" << cnt << ") is " << *sAct << std::endl;
                            feedback = false;
                        }
                    }
                }
            }

            // Compare divisor
            if(expVec.at(cnt).divisor != arg.at(cnt).divisor) {
                std::cout << "expect divisor(" << cnt << ") is " << expVec.at(cnt).divisor << " != actual(" << cnt << ") is " << arg.at(cnt).divisor << std::endl;
                feedback = false;
            }
        }
    }
    return feedback;
}

TEST_F(Test_FileDataReader, fileVersion1)
{
    bool feedback;
    fileDataReader = std::make_shared<FileDataReader>(m_filePath + "fileVersion_01.dat", csvWriter, m_enableLog);
    feedback = fileDataReader->readHeaderData();
    EXPECT_TRUE(feedback);

    uint32_t expecedNrColExclusiveTimeStamp = 69;

    std::vector<IFileDataWriterCSV::DataEntryCSV> expectedVectorCSV(2 * (expecedNrColExclusiveTimeStamp + 2));
    expectedVectorCSV.at(0).uiVal = static_cast<uint32_t>(0x5e6677B4);         // B4 77 66 5E
    expectedVectorCSV.at(1).uiVal = static_cast<uint32_t>(0x00000000);         // 00 00 00 00
    expectedVectorCSV.at(2).uiVal = static_cast<uint32_t>(0x00000110);         // 10 01 00 00
    expectedVectorCSV.at(3).uiVal = static_cast<uint32_t>(0x00000110);         // 10 01 00 00
    expectedVectorCSV.at(4).uiVal = static_cast<uint32_t>(0x00000116);         // 16 01 00 00
    expectedVectorCSV.at(5).uiVal = static_cast<uint32_t>(0x00000032);         // 32 00 00 00
    expectedVectorCSV.at(6).uiVal = static_cast<uint32_t>(0x0000018C);         // 8C 01 00 00
    expectedVectorCSV.at(7).uiVal = static_cast<uint32_t>(0x0000001C);         // 1C 00 00 00
    expectedVectorCSV.at(8).uiVal = static_cast<uint32_t>(0x00000028);         // 28 00 00 00
    expectedVectorCSV.at(9).uiVal = static_cast<uint32_t>(0x000001EB);         // EB 01 00 00
    expectedVectorCSV.at(10).uiVal = static_cast<uint32_t>(0x000001F4);        // F4 01 00 00
    expectedVectorCSV.at(11).uiVal = static_cast<uint32_t>(0x00000058);        // 58 00 00 00
    expectedVectorCSV.at(12).uiVal = static_cast<uint32_t>(0x00000062);        // 62 00 00 00
    expectedVectorCSV.at(13).uiVal = static_cast<uint32_t>(0x000002EE);        // EE 02 00 00
    expectedVectorCSV.at(14).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(15).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(16).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(17).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(18).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(19).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(20).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(21).uiVal = static_cast<uint32_t>(0x00000001);        // 01 00 00 00
    expectedVectorCSV.at(22).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(23).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(24).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(25).uiVal = static_cast<uint32_t>(0x00000001);        // 01 00 00 00
    expectedVectorCSV.at(26).uiVal = static_cast<uint32_t>(0x00000001);        // 01 00 00 00
    expectedVectorCSV.at(27).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(28).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(29).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(30).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(31).uiVal = static_cast<uint32_t>(0x00000001);        // 01 00 00 00
    expectedVectorCSV.at(32).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(33).uiVal = static_cast<uint32_t>(0x00000001);        // 01 00 00 00
    expectedVectorCSV.at(34).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(35).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(36).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(37).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(38).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(39).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(40).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(41).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(42).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(43).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(44).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(45).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(46).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(47).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(48).uiVal = static_cast<uint32_t>(0x01644F2E);        // 2E 4F 64 01
    expectedVectorCSV.at(49).uiVal = static_cast<uint32_t>(0x00002EF3);        // F3 2E 00 00
    expectedVectorCSV.at(50).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(51).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(52).uiVal = static_cast<uint32_t>(0x002E47DF);        // DF 47 2E 00
    expectedVectorCSV.at(53).uiVal = static_cast<uint32_t>(0x003613A4);        // A4 13 36 00
    expectedVectorCSV.at(54).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(55).uiVal = static_cast<uint32_t>(0x01644F2E);        // 2E 4F 64 01
    expectedVectorCSV.at(56).uiVal = static_cast<uint32_t>(0x010F9930);        // 30 99 0F 01
    expectedVectorCSV.at(57).uiVal = static_cast<uint32_t>(0x0054B5D7);        // D7 B5 54 00
    expectedVectorCSV.at(58).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(59).uiVal = static_cast<uint32_t>(0x00000001);        // 01 00 00 00
    expectedVectorCSV.at(60).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(61).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(62).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(63).uiVal = static_cast<uint32_t>(0x00000002);        // 02 00 00 00
    expectedVectorCSV.at(64).uiVal = static_cast<uint32_t>(0x000000DF);        // DF 00 00 00
    expectedVectorCSV.at(65).uiVal = static_cast<uint32_t>(0x00000129);        // 29 01 00 00
    expectedVectorCSV.at(66).uiVal = static_cast<uint32_t>(0x00000020);        // 20 00 00 00
    expectedVectorCSV.at(67).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(68).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(69).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(70).uiVal = static_cast<uint32_t>(0x00000003);        // 03 00 00 00
    expectedVectorCSV.at(71).uiVal = static_cast<uint32_t>(0x5E6677F0);        // F0 77 66 5E
    expectedVectorCSV.at(72).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(73).uiVal = static_cast<uint32_t>(0x0000010E);        // 0E 01 00 00
    expectedVectorCSV.at(74).uiVal = static_cast<uint32_t>(0x0000010E);        // 0E 01 00 00
    expectedVectorCSV.at(75).uiVal = static_cast<uint32_t>(0x00000116);        // 16 01 00 00
    expectedVectorCSV.at(76).uiVal = static_cast<uint32_t>(0x00000032);        // 32 00 00 00
    expectedVectorCSV.at(77).uiVal = static_cast<uint32_t>(0x0000017C);        // 7C 01 00 00
    expectedVectorCSV.at(78).uiVal = static_cast<uint32_t>(0x0000001B);        // 1B 00 00 00
    expectedVectorCSV.at(79).uiVal = static_cast<uint32_t>(0x00000028);        // 28 00 00 00
    expectedVectorCSV.at(80).uiVal = static_cast<uint32_t>(0x000001EB);        // EB 01 00 00
    expectedVectorCSV.at(81).uiVal = static_cast<uint32_t>(0x000001F4);        // F4 01 00 00
    expectedVectorCSV.at(82).uiVal = static_cast<uint32_t>(0x00000059);        // 59 00 00 00
    expectedVectorCSV.at(83).uiVal = static_cast<uint32_t>(0x00000063);        // 63 00 00 00
    expectedVectorCSV.at(84).uiVal = static_cast<uint32_t>(0x000002EE);        // EE 02 00 00
    expectedVectorCSV.at(85).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(86).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(87).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(88).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(89).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(90).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(91).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(92).uiVal = static_cast<uint32_t>(0x00000001);        // 01 00 00 00
    expectedVectorCSV.at(93).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(94).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(95).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(96).uiVal = static_cast<uint32_t>(0x00000001);        // 01 00 00 00
    expectedVectorCSV.at(97).uiVal = static_cast<uint32_t>(0x00000001);        // 01 00 00 00
    expectedVectorCSV.at(98).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(99).uiVal = static_cast<uint32_t>(0x00000000);        // 00 00 00 00
    expectedVectorCSV.at(100).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(101).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(102).uiVal = static_cast<uint32_t>(0x00000001);       // 01 00 00 00
    expectedVectorCSV.at(103).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(104).uiVal = static_cast<uint32_t>(0x00000001);       // 01 00 00 00
    expectedVectorCSV.at(105).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(106).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(107).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(108).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(109).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(110).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(111).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(112).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(113).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(114).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(115).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(116).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(117).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(118).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(119).uiVal = static_cast<uint32_t>(0x01644F2E);       // 2E 4F 64 01
    expectedVectorCSV.at(120).uiVal = static_cast<uint32_t>(0x00002EF3);       // F3 2E 00 00
    expectedVectorCSV.at(121).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(122).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(123).uiVal = static_cast<uint32_t>(0x002E47DF);       // DF 47 2E 00
    expectedVectorCSV.at(124).uiVal = static_cast<uint32_t>(0x003613A4);       // A4 13 36 00
    expectedVectorCSV.at(125).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(126).uiVal = static_cast<uint32_t>(0x01644F2E);       // 2E 4F 64 01
    expectedVectorCSV.at(127).uiVal = static_cast<uint32_t>(0x010F9930);       // 30 99 0F 01
    expectedVectorCSV.at(128).uiVal = static_cast<uint32_t>(0x0054B5D7);       // D7 B5 54 00
    expectedVectorCSV.at(129).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(130).uiVal = static_cast<uint32_t>(0x00000001);       // 01 00 00 00
    expectedVectorCSV.at(131).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(132).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(133).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(134).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(135).uiVal = static_cast<uint32_t>(0x000000A3);       // A3 00 00 00
    expectedVectorCSV.at(136).uiVal = static_cast<uint32_t>(0x00000165);       // 65 01 00 00
    expectedVectorCSV.at(137).uiVal = static_cast<uint32_t>(0x00000020);       // 20 00 00 00
    expectedVectorCSV.at(138).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(139).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(140).uiVal = static_cast<uint32_t>(0x00000000);       // 00 00 00 00
    expectedVectorCSV.at(141).uiVal = static_cast<uint32_t>(0x00000003);       // 03 00 00 00

    expectedVectorCSV.at(0).divisor = 1;
    expectedVectorCSV.at(1).divisor = 1;
    expectedVectorCSV.at(71).divisor = 1;
    expectedVectorCSV.at(72).divisor = 1;
    for(uint32_t cnt = 0; cnt < 69; cnt++) {
        expectedVectorCSV.at(2 + cnt).divisor = expectedDivisor_v1.at(cnt);
        expectedVectorCSV.at(73 + cnt).divisor = expectedDivisor_v1.at(cnt);
    }

    EXPECT_CALL(*csvWriter, writeHeader(*expectedHeaderLine_v1)).WillOnce(Return(true));
    EXPECT_CALL(*csvWriter, writeHeader(*expectedUnitLine_v1)).WillOnce(Return(true));
    EXPECT_CALL(*csvWriter, writeData(compareVectors(expectedVectorCSV), expecedNrColExclusiveTimeStamp)).WillOnce(Return(true));
    feedback = fileDataReader->decodeData();
    EXPECT_TRUE(feedback);
}

TEST_F(Test_FileDataReader, DISABLED_fileVersion2)
{
    bool feedback;
    fileDataReader = std::make_shared<FileDataReader>(m_filePath + "fileVersion_02.dat", csvWriter, m_enableLog);
    feedback = fileDataReader->readHeaderData();
    EXPECT_TRUE(feedback);

    uint32_t expecedNrColExclusiveTimeStamp = 50;

    std::vector<IFileDataWriterCSV::DataEntryCSV> expectedVectorCSV(3 * (expecedNrColExclusiveTimeStamp + 2));
    expectedVectorCSV.at(0).uiVal = 0x5E7A90F0;        // F0 90 7A 5E
    expectedVectorCSV.at(1).uiVal = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(2).uiVal = 0x00000173;        // 73 01 00 00
    expectedVectorCSV.at(3).uiVal = 0x0000013E;        // 3E 01 00 00
    expectedVectorCSV.at(4).uiVal = 0x00000123;        // 23 01 00 00
    expectedVectorCSV.at(5).uiVal = 0x0000027F;        // 7F 02 00 00
    expectedVectorCSV.at(6).uiVal = 0xFFFFFFEF;        // EF FF FF FF
    expectedVectorCSV.at(7).uiVal = 0xFFFFFFFF;        // FF FF FF FF
    expectedVectorCSV.at(8).uiVal = 0x000001D8;        // D8 01 00 00
    expectedVectorCSV.at(9).uiVal = 0x000001F4;        // F4 01 00 00
    expectedVectorCSV.at(10).uiVal = 0x00000042;       // 42 00 00 00
    expectedVectorCSV.at(11).uiVal = 0x00000024;       // 24 00 00 00
    expectedVectorCSV.at(12).uiVal = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(13).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(14).uiVal = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(15).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(16).uiVal = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(17).uiVal = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(18).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(19).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(20).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(21).uiVal = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(22).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(23).uiVal = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(24).uiVal = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(25).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(26).uiVal = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(27).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(28).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(29).uiVal = 0x0168BC84;       // 84 BC 68 01
    expectedVectorCSV.at(30).uiVal = 0x00002F51;       // 51 2F 00 00
    expectedVectorCSV.at(31).uiVal = 0x003693FF;       // FF 93 36 00
    expectedVectorCSV.at(32).uiVal = 0x0168BC84;       // 84 BC 68 01
    expectedVectorCSV.at(33).uiVal = 0x011357EB;       // EB 57 13 01
    expectedVectorCSV.at(34).uiVal = 0x00556472;       // 72 64 55 00
    expectedVectorCSV.at(35).uiVal = 0x00000C17;       // 17 0C 00 00
    expectedVectorCSV.at(36).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(37).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(38).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(39).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(40).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(41).uiVal = 0x00000118;       // 18 01 00 00
    expectedVectorCSV.at(42).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(43).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(44).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(45).uiVal = 0x0000003C;       // 3C 00 00 00
    expectedVectorCSV.at(46).uiVal = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(47).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(48).uiVal = 0x0008E4D0;       // D0 E4 08 00
    expectedVectorCSV.at(49).uiVal = 0x0002AD4E;       // 4E AD 02 00
    expectedVectorCSV.at(50).uiVal = 0x000B9631;       // 31 96 0B 00
    expectedVectorCSV.at(51).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(52).uiVal = 0x5E7A912C;       // 2C 91 7A 5E
    expectedVectorCSV.at(53).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(54).uiVal = 0x00000173;       // 73 01 00 00
    expectedVectorCSV.at(55).uiVal = 0x0000013E;       // 3E 01 00 00
    expectedVectorCSV.at(56).uiVal = 0x00000123;       // 23 01 00 00
    expectedVectorCSV.at(57).uiVal = 0x00000280;       // 80 02 00 00
    expectedVectorCSV.at(58).uiVal = 0xFFFFFFEE;       // EE FF FF FF
    expectedVectorCSV.at(59).uiVal = 0xFFFFFFFF;       // FF FF FF FF
    expectedVectorCSV.at(60).uiVal = 0x000001D7;       // D7 01 00 00
    expectedVectorCSV.at(61).uiVal = 0x000001F4;       // F4 01 00 00
    expectedVectorCSV.at(62).uiVal = 0x00000042;       // 42 00 00 00
    expectedVectorCSV.at(63).uiVal = 0x00000025;       // 25 00 00 00
    expectedVectorCSV.at(64).uiVal = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(65).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(66).uiVal = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(67).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(68).uiVal = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(69).uiVal = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(70).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(71).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(72).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(73).uiVal = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(74).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(75).uiVal = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(76).uiVal = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(77).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(78).uiVal = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(79).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(80).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(81).uiVal = 0x0168BCC0;       // C0 BC 68 01
    expectedVectorCSV.at(82).uiVal = 0x00002F51;       // 51 2F 00 00
    expectedVectorCSV.at(83).uiVal = 0x003693FF;       // FF 93 36 00
    expectedVectorCSV.at(84).uiVal = 0x0168BCC0;       // C0 BC 68 01
    expectedVectorCSV.at(85).uiVal = 0x01135827;       // 27 58 13 01
    expectedVectorCSV.at(86).uiVal = 0x00556472;       // 72 64 55 00
    expectedVectorCSV.at(87).uiVal = 0x00000C53;       // 53 0C 00 00
    expectedVectorCSV.at(88).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(89).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(90).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(91).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(92).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(93).uiVal = 0x00000118;       // 18 01 00 00
    expectedVectorCSV.at(94).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(95).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(96).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(97).uiVal = 0x0000003C;       // 3C 00 00 00
    expectedVectorCSV.at(98).uiVal = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(99).uiVal = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(100).uiVal = 0x0008E4D2;      // D2 E4 08 00
    expectedVectorCSV.at(101).uiVal = 0x0002AD4E;      // 4E AD 02 00
    expectedVectorCSV.at(102).uiVal = 0x000B9633;      // 33 96 0B 00
    expectedVectorCSV.at(103).uiVal = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(104).uiVal = 0x5E7A9168;      // 68 91 7A 5E
    expectedVectorCSV.at(105).uiVal = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(106).uiVal = 0x00000173;      // 73 01 00 00
    expectedVectorCSV.at(107).uiVal = 0x0000013F;      // 3F 01 00 00
    expectedVectorCSV.at(108).uiVal = 0x00000123;      // 23 01 00 00
    expectedVectorCSV.at(109).uiVal = 0x00000281;      // 81 02 00 00
    expectedVectorCSV.at(110).uiVal = 0xFFFFFFEF;      // EF FF FF FF
    expectedVectorCSV.at(111).uiVal = 0xFFFFFFFF;      // FF FF FF FF
    expectedVectorCSV.at(112).uiVal = 0x000001D8;      // D8 01 00 00
    expectedVectorCSV.at(113).uiVal = 0x000001F4;      // F4 01 00 00
    expectedVectorCSV.at(114).uiVal = 0x00000042;      // 42 00 00 00
    expectedVectorCSV.at(115).uiVal = 0x00000024;      // 24 00 00 00
    expectedVectorCSV.at(116).uiVal = 0x00000001;      // 01 00 00 00
    expectedVectorCSV.at(117).uiVal = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(118).uiVal = 0x00000001;      // 01 00 00 00
    expectedVectorCSV.at(119).uiVal = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(120).uiVal = 0x00000001;      // 01 00 00 00
    expectedVectorCSV.at(121).uiVal = 0x00000001;      // 01 00 00 00
    expectedVectorCSV.at(122).uiVal = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(123).uiVal = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(124).uiVal = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(125).uiVal = 0x00000001;      // 01 00 00 00
    expectedVectorCSV.at(126).uiVal = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(127).uiVal = 0x00000001;      // 01 00 00 00
    expectedVectorCSV.at(128).uiVal = 0x00000001;      // 01 00 00 00
    expectedVectorCSV.at(129).uiVal = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(130).uiVal = 0x00000001;      // 01 00 00 00
    expectedVectorCSV.at(131).uiVal = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(132).uiVal = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(133).uiVal = 0x0168BCFC;      // FC BC 68 01
    expectedVectorCSV.at(134).uiVal = 0x00002F51;      // 51 2F 00 00
    expectedVectorCSV.at(135).uiVal = 0x003693FF;      // FF 93 36 00
    expectedVectorCSV.at(136).uiVal = 0x0168BCFC;      // FC BC 68 01
    expectedVectorCSV.at(137).uiVal = 0x01135863;      // 63 58 13 01
    expectedVectorCSV.at(138).uiVal = 0x00556472;      // 72 64 55 00
    expectedVectorCSV.at(139).uiVal = 0x00000C8F;      // 8F 0C 00 00
    expectedVectorCSV.at(140).uiVal = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(141).uiVal = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(142).uiVal = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(143).uiVal = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(144).uiVal = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(145).uiVal = 0x00000118;      // 18 01 00 00
    expectedVectorCSV.at(146).uiVal = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(147).uiVal = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(148).uiVal = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(149).uiVal = 0x0000003C;      // 3C 00 00 00
    expectedVectorCSV.at(150).uiVal = 0x00000001;      // 01 00 00 00
    expectedVectorCSV.at(151).uiVal = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(152).uiVal = 0x0008E4D4;      // D4 E4 08 00
    expectedVectorCSV.at(153).uiVal = 0x0002AD4E;      // 4E AD 02 00
    expectedVectorCSV.at(154).uiVal = 0x000B9635;      // 35 96 0B 00
    expectedVectorCSV.at(155).uiVal = 0x00000000;      // 00 00 00 00

    expectedVectorCSV.at(0).divisor = 1;
    expectedVectorCSV.at(1).divisor = 1;
    expectedVectorCSV.at(52).divisor = 1;
    expectedVectorCSV.at(53).divisor = 1;
    expectedVectorCSV.at(104).divisor = 1;
    expectedVectorCSV.at(105).divisor = 1;
    for(uint32_t cnt = 0; cnt < 50; cnt++) {
        expectedVectorCSV.at(2 + cnt).divisor = expectedDivisor_v2_v3.at(cnt);
        expectedVectorCSV.at(54 + cnt).divisor = expectedDivisor_v2_v3.at(cnt);
        expectedVectorCSV.at(106 + cnt).divisor = expectedDivisor_v2_v3.at(cnt);
    }
    EXPECT_CALL(*csvWriter, writeHeader(*expectedHeaderLine_v2_v3)).WillOnce(Return(true));
    EXPECT_CALL(*csvWriter, writeHeader(*expectedUnitLine_v2_v3)).WillOnce(Return(true));
    EXPECT_CALL(*csvWriter, writeData(compareVectors(expectedVectorCSV), expecedNrColExclusiveTimeStamp)).WillOnce(Return(true));
    feedback = fileDataReader->decodeData();
    EXPECT_TRUE(feedback);
}

TEST_F(Test_FileDataReader, DISABLED_unsupportedFileVersion)
{
    bool feedback;
    fileDataReader = std::make_shared<FileDataReader>(m_filePath + "fileVersion_NotSupported.dat", csvWriter, m_enableLog);
    feedback = fileDataReader->readHeaderData();
    EXPECT_FALSE(feedback);
}

TEST_F(Test_FileDataReader, DISABLED_fileVersion03_zeroValues)
{
    bool feedback;
    fileDataReader = std::make_shared<FileDataReader>(m_filePath + "fileVersion_03_zeroValues.dat", csvWriter, m_enableLog);
    feedback = fileDataReader->readHeaderData();
    EXPECT_TRUE(feedback);

    uint32_t expecedNrColExclusiveTimeStamp = 50;

    std::vector<IFileDataWriterCSV::DataEntryCSV> expectedVectorCSV(1 * (expecedNrColExclusiveTimeStamp + 2), {0, 1});

    expectedVectorCSV.at(0).divisor = 1;
    expectedVectorCSV.at(1).divisor = 1;
    for(uint32_t cnt = 0; cnt < expecedNrColExclusiveTimeStamp; cnt++) {
        expectedVectorCSV.at(2 + cnt).divisor = expectedDivisor_v2_v3.at(cnt);
    }

    EXPECT_CALL(*csvWriter, writeHeader(*expectedHeaderLine_v2_v3)).WillOnce(Return(true));
    EXPECT_CALL(*csvWriter, writeHeader(*expectedUnitLine_v2_v3)).WillOnce(Return(true));
    EXPECT_CALL(*csvWriter, writeData(compareVectors(expectedVectorCSV), expecedNrColExclusiveTimeStamp)).WillOnce(Return(true));
    feedback = fileDataReader->decodeData();
    EXPECT_TRUE(feedback);
}

TEST_F(Test_FileDataReader, DISABLED_fileVersion03_maxValues)
{
    bool feedback;
    fileDataReader = std::make_shared<FileDataReader>(m_filePath + "fileVersion_03_maxValues.dat", csvWriter, m_enableLog);
    feedback = fileDataReader->readHeaderData();
    EXPECT_TRUE(feedback);

    uint32_t expecedNrColExclusiveTimeStamp = 50;

    std::vector<IFileDataWriterCSV::DataEntryCSV> expectedVectorCSV(1 * (expecedNrColExclusiveTimeStamp + 2), {0, 1});
    expectedVectorCSV.at(0).uiVal = 0xffffffff;      // First part of the timestamp
    expectedVectorCSV.at(1).uiVal = 0xffffffff;      // Second part of the timestamp
    expectedVectorCSV.at(2).uiVal = 1023;            // "Vorlauftemperatur Heizkreis",                                         10, "°C",       10, DataTypeInfo::UNSIGNED }, // 0..102.3
    expectedVectorCSV.at(3).uiVal = 1023;            // "Rücklauftemperatur Heizkreis",                                        10, "°C",       10, DataTypeInfo::UNSIGNED }, // 0..102.3
    expectedVectorCSV.at(4).uiVal = 1023;            // "Rücklauf-Soll Heizkreis",                                             10, "°C",       10, DataTypeInfo::UNSIGNED }, // 0..102.3
    expectedVectorCSV.at(5).uiVal = 1023;            // "Heisgastemperatur",                                                   10, "°C",       10, DataTypeInfo::UNSIGNED }, // 0..102.3
    expectedVectorCSV.at(6).uiVal = 1023;            // "Aussentemperatur",                                                    10, "°C",       11, DataTypeInfo::SIGNED},    // -102.4 .. 102.3
    expectedVectorCSV.at(7).uiVal = 511;             // "Durchschnittstemperatur Aussen über 24 h (Funktion Heizgrenze)",      10, "°C",       10, DataTypeInfo::SIGNED},    // -51.2 .. 51.1
    expectedVectorCSV.at(8).uiVal = 1023;            // "Warmwasser Ist-Temperatur",                                           10, "°C",       10, DataTypeInfo::UNSIGNED }, // 0..102.3
    expectedVectorCSV.at(9).uiVal = 1023;            // "Warmwasser Soll-Temperatur",                                          10, "°C",       10, DataTypeInfo::UNSIGNED }, // 0..102.3
    expectedVectorCSV.at(10).uiVal = 1023;           // "Wärmequellen-Eintrittstemperatur",                                    10, "°C",       10, DataTypeInfo::UNSIGNED }, // 0..102.3
    expectedVectorCSV.at(11).uiVal = 1023;           // "Wärmequellen-Austrittstemperatur",                                    10, "°C",       10, DataTypeInfo::UNSIGNED }, // 0..102.3
    expectedVectorCSV.at(12).uiVal = 1;              // "Eingang Abtauende / Soledruck / Durchfluss",                           0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(13).uiVal = 1;              // "Brauchwarmwasserthermostat",                                           0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(14).uiVal = 1;              // "EVU Sperre",                                                           0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(15).uiVal = 1;              // "Hochdruck Kältekreis",                                                 0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(16).uiVal = 1;              // "Motorschutz OK",                                                       0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(17).uiVal = 1;              // "Niederdruck",                                                          0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(18).uiVal = 1;              // "Überwachungskontakt für Potentiostat",                                 0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(19).uiVal = 1;              // "Abtauventil",                                                          0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(20).uiVal = 1;              // "Brauchwasserpumpe/Umstellventil",                                      0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(21).uiVal = 1;              // "Heizungsumwälzpumpe",                                                  0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(22).uiVal = 1;              // "Ventilation (Lüftung)",                                                0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(23).uiVal = 1;              // "Solepumpe/Ventilator",                                                 0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(24).uiVal = 1;              // "Verdichter 1",                                                         0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(25).uiVal = 1;              // "Zirkulationspumpe",                                                    0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(26).uiVal = 1;              // "Zusatzumwälzpumpe",                                                    0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(27).uiVal = 1;              // "Steuersignal Zusatzheizung v. Heizung",                                0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(28).uiVal = 1;              // "Steuersignal Zusatzheizung/Störsignal",                                0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(29).uiVal = (1 << 30) - 1;  // "Betriebsstunden Verdichter 1",                                         0, "Sekunden", 30, DataTypeInfo::UNSIGNED}, // 0 .. 34 years
    expectedVectorCSV.at(30).uiVal = (1 << 22) - 1;  // "Impulse Verdichter 1",                                                 0, "Impulse" , 22, DataTypeInfo::UNSIGNED}, // 346 times more than today
    expectedVectorCSV.at(31).uiVal = (1 << 30) - 1;  // "Betriebsstunden Zweiter Wärmeerzeuger 2",                              0, "Sekunden", 30, DataTypeInfo::UNSIGNED}, // 0 .. 34 years -> Heats up to 60 deg
    expectedVectorCSV.at(32).uiVal = (1 << 30) - 1;  // "Betriebsstunden Wärmepumpe",                                           0, "Sekunden", 30, DataTypeInfo::UNSIGNED}, // 0 .. 34 years
    expectedVectorCSV.at(33).uiVal = (1 << 30) - 1;  // "Betriebsstunden Heizung",                                              0, "Sekunden", 30, DataTypeInfo::UNSIGNED}, // 0 .. 34 years
    expectedVectorCSV.at(34).uiVal = (1 << 30) - 1;  // "Betriebsstunden Warmwasser",                                           0, "Sekunden", 30, DataTypeInfo::UNSIGNED}, // 0 .. 34 years
    expectedVectorCSV.at(35).uiVal = (1 << 17) - 1;  // "Wärmepumpe läuft seit",                                                0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h
    expectedVectorCSV.at(36).uiVal = (1 << 17) - 1;  // "Zweiter Wärmeerzeuger 2 läuft seit",                                   0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h -> Heats up to 60 deg
    expectedVectorCSV.at(37).uiVal = (1 << 17) - 1;  // "Netzeinschaltverzögerung",                                             0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h
    expectedVectorCSV.at(38).uiVal = (1 << 17) - 1;  // "Schaltspielsperre Aus",                                                0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h
    expectedVectorCSV.at(39).uiVal = (1 << 17) - 1;  // "Schaltspielsperre Ein",                                                0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h
    expectedVectorCSV.at(40).uiVal = (1 << 17) - 1;  // "Verdichter-Standzeit",                                                 0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h
    expectedVectorCSV.at(41).uiVal = (1 << 17) - 1;  // "Heizungsregler Mehr-Zeit",                                             0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h
    expectedVectorCSV.at(42).uiVal = (1 << 17) - 1;  // "Heizungsregler Weniger-Zeit",                                          0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h
    expectedVectorCSV.at(43).uiVal = (1 << 17) - 1;  // "Thermische Desinfektion läuft seit",                                   0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h -> Heats up to 60 deg
    expectedVectorCSV.at(44).uiVal = (1 << 17) - 1;  // "Sperre Warmwasser",                                                    0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h -> potentially 0
    expectedVectorCSV.at(45).uiVal = (1 << 7) - 1;   // "Wärmepumpentyp|0 = ERC|Typenschlüssel",                                0, "-",         7, DataTypeInfo::UNSIGNED}, // enum 0..75 < 2^7 = 128 -> potentially  const
    expectedVectorCSV.at(46).uiVal = (1 << 2) - 1;   // "Bivalenzstufe",                                                        0, "enum",  2, DataTypeInfo::UNSIGNED },  // According the enum
    expectedVectorCSV.at(47).uiVal = (1 << 3) - 1;   // (1<<3)-1;  // "Betriebszustand",                                        0, "enum",      3, DataTypeInfo::UNSIGNED },  // According the enum
    expectedVectorCSV.at(48).uiVal = (1 << 28) - 1;  // "Wärmemengenzähler Heizung",                                           10, "kWh",      28, DataTypeInfo::UNSIGNED}, // 460 times more than today
    expectedVectorCSV.at(49).uiVal = (1 << 26) - 1;  // "Wärmemengenzähler Brauchwasser",                                      10, "kWh",      26, DataTypeInfo::UNSIGNED}, // 382 times more than today
    expectedVectorCSV.at(50).uiVal = (1 << 28) - 1;  // "Wärmemengenzähler Gesamt",                                            10, "kWh",      28, DataTypeInfo::UNSIGNED}, // 350 times more than today
    expectedVectorCSV.at(51).uiVal = (1 << 10) - 1;  // "Wärmemengenzähler Durchfluss",                                         1, "l / h",    10, DataTypeInfo::UNSIGNED}, // 0 .. 1024l/h  -> potentially unused

    expectedVectorCSV.at(0).divisor = 1;
    expectedVectorCSV.at(1).divisor = 1;
    for(uint32_t cnt = 0; cnt < expecedNrColExclusiveTimeStamp; cnt++) {
        expectedVectorCSV.at(2 + cnt).divisor = expectedDivisor_v2_v3.at(cnt);
    }

    EXPECT_CALL(*csvWriter, writeHeader(*expectedHeaderLine_v2_v3)).WillOnce(Return(true));
    EXPECT_CALL(*csvWriter, writeHeader(*expectedUnitLine_v2_v3)).WillOnce(Return(true));
    EXPECT_CALL(*csvWriter, writeData(compareVectors(expectedVectorCSV), expecedNrColExclusiveTimeStamp)).WillOnce(Return(true));
    feedback = fileDataReader->decodeData();
    EXPECT_TRUE(feedback);
}

// signed values! -(max)
TEST_F(Test_FileDataReader, DISABLED_fileVersion03_minValues)
{
    bool feedback;
    fileDataReader = std::make_shared<FileDataReader>(m_filePath + "fileVersion_03_minValues.dat", csvWriter, m_enableLog);
    feedback = fileDataReader->readHeaderData();
    EXPECT_TRUE(feedback);

    uint32_t expecedNrColExclusiveTimeStamp = 50;

    std::vector<IFileDataWriterCSV::DataEntryCSV> expectedVectorCSV(1 * (expecedNrColExclusiveTimeStamp + 2), {0, 1});

    auto setSignedExpectation = [&expectedVectorCSV](uint32_t position, int32_t signedValue, uint32_t nrBits) -> uint32_t {
                                    uint32_t bitMask = (1 << nrBits) - 1;
                                    uint32_t uVal = (bitMask & (*reinterpret_cast<uint32_t*>(&signedValue)));
                                    expectedVectorCSV.at(position).uiVal = uVal;
                                    return uVal;
                                };

    setSignedExpectation(6, -1024, 11);      // "Aussentemperatur",                                                    10, "°C",       11, DataTypeInfo::SIGNED},    // -102.4 .. 102.3
    setSignedExpectation(7, -512, 10);       // "Durchschnittstemperatur Aussen über 24 h (Funktion Heizgrenze)",      10, "°C",       10, DataTypeInfo::SIGNED},    // -51.2 .. 51.1

    expectedVectorCSV.at(0).divisor = 1;
    expectedVectorCSV.at(1).divisor = 1;
    for(uint32_t cnt = 0; cnt < expecedNrColExclusiveTimeStamp; cnt++) {
        expectedVectorCSV.at(2 + cnt).divisor = expectedDivisor_v2_v3.at(cnt);
    }

    EXPECT_CALL(*csvWriter, writeHeader(*expectedHeaderLine_v2_v3)).WillOnce(Return(true));
    EXPECT_CALL(*csvWriter, writeHeader(*expectedUnitLine_v2_v3)).WillOnce(Return(true));
    EXPECT_CALL(*csvWriter, writeData(compareVectors(expectedVectorCSV), expecedNrColExclusiveTimeStamp)).WillOnce(Return(true));
    feedback = fileDataReader->decodeData();
    EXPECT_TRUE(feedback);
}

TEST_F(Test_FileDataReader, DISABLED_fileVersion03_enumerateValues)
{
    bool feedback;
    fileDataReader = std::make_shared<FileDataReader>(m_filePath + "fileVersion_03_enumerateValues.dat", csvWriter, m_enableLog);
    feedback = fileDataReader->readHeaderData();
    EXPECT_TRUE(feedback);

    uint32_t expecedNrColExclusiveTimeStamp = 50;

    std::vector<IFileDataWriterCSV::DataEntryCSV> expectedVectorCSV(1 * (expecedNrColExclusiveTimeStamp + 2), {0, 1});
    expectedVectorCSV.at(0).uiVal = 0x0bc0dcc0;   // 1. April 1976 0640
    expectedVectorCSV.at(1).uiVal = 0;            // Second part of the timestamp
    expectedVectorCSV.at(2).uiVal = 1;            // "Vorlauftemperatur Heizkreis",                                         10, "°C",       10, DataTypeInfo::UNSIGNED }, // 0..102.3
    expectedVectorCSV.at(3).uiVal = 2;            // "Rücklauftemperatur Heizkreis",                                        10, "°C",       10, DataTypeInfo::UNSIGNED }, // 0..102.3
    expectedVectorCSV.at(4).uiVal = 3;            // "Rücklauf-Soll Heizkreis",                                             10, "°C",       10, DataTypeInfo::UNSIGNED }, // 0..102.3
    expectedVectorCSV.at(5).uiVal = 4;            // "Heisgastemperatur",                                                   10, "°C",       10, DataTypeInfo::UNSIGNED }, // 0..102.3
    expectedVectorCSV.at(6).uiVal = 5;            // "Aussentemperatur",                                                    10, "°C",       11, DataTypeInfo::SIGNED},    // -102.4 .. 102.3
    expectedVectorCSV.at(7).uiVal = 6;            // "Durchschnittstemperatur Aussen über 24 h (Funktion Heizgrenze)",      10, "°C",       10, DataTypeInfo::SIGNED},    // -51.2 .. 51.1
    expectedVectorCSV.at(8).uiVal = 7;            // "Warmwasser Ist-Temperatur",                                           10, "°C",       10, DataTypeInfo::UNSIGNED }, // 0..102.3
    expectedVectorCSV.at(9).uiVal = 8;            // "Warmwasser Soll-Temperatur",                                          10, "°C",       10, DataTypeInfo::UNSIGNED }, // 0..102.3
    expectedVectorCSV.at(10).uiVal = 9;           // "Wärmequellen-Eintrittstemperatur",                                    10, "°C",       10, DataTypeInfo::UNSIGNED }, // 0..102.3
    expectedVectorCSV.at(11).uiVal = 10;          // "Wärmequellen-Austrittstemperatur",                                    10, "°C",       10, DataTypeInfo::UNSIGNED }, // 0..102.3
    expectedVectorCSV.at(12).uiVal = 1;           // "Eingang Abtauende / Soledruck / Durchfluss",                           0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(13).uiVal = 0;           // "Brauchwarmwasserthermostat",                                           0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(14).uiVal = 1;           // "EVU Sperre",                                                           0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(15).uiVal = 0;           // "Hochdruck Kältekreis",                                                 0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(16).uiVal = 1;           // "Motorschutz OK",                                                       0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(17).uiVal = 0;           // "Niederdruck",                                                          0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(18).uiVal = 1;           // "Überwachungskontakt für Potentiostat",                                 0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(19).uiVal = 0;           // "Abtauventil",                                                          0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(20).uiVal = 1;           // "Brauchwasserpumpe/Umstellventil",                                      0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(21).uiVal = 0;           // "Heizungsumwälzpumpe",                                                  0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(22).uiVal = 1;           // "Ventilation (Lüftung)",                                                0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(23).uiVal = 0;           // "Solepumpe/Ventilator",                                                 0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(24).uiVal = 1;           // "Verdichter 1",                                                         0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(25).uiVal = 0;           // "Zirkulationspumpe",                                                    0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(26).uiVal = 1;           // "Zusatzumwälzpumpe",                                                    0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(27).uiVal = 0;           // "Steuersignal Zusatzheizung v. Heizung",                                0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(28).uiVal = 1;           // "Steuersignal Zusatzheizung/Störsignal",                                0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(29).uiVal = 11;          // "Betriebsstunden Verdichter 1",                                         0, "Sekunden", 30, DataTypeInfo::UNSIGNED}, // 0 .. 34 years
    expectedVectorCSV.at(30).uiVal = 12;          // "Impulse Verdichter 1",                                                 0, "Impulse" , 22, DataTypeInfo::UNSIGNED}, // 346 times more than today
    expectedVectorCSV.at(31).uiVal = 13;          // "Betriebsstunden Zweiter Wärmeerzeuger 2",                              0, "Sekunden", 30, DataTypeInfo::UNSIGNED}, // 0 .. 34 years -> Heats up to 60 deg
    expectedVectorCSV.at(32).uiVal = 14;          // "Betriebsstunden Wärmepumpe",                                           0, "Sekunden", 30, DataTypeInfo::UNSIGNED}, // 0 .. 34 years
    expectedVectorCSV.at(33).uiVal = 15;          // "Betriebsstunden Heizung",                                              0, "Sekunden", 30, DataTypeInfo::UNSIGNED}, // 0 .. 34 years
    expectedVectorCSV.at(34).uiVal = 16;          // "Betriebsstunden Warmwasser",                                           0, "Sekunden", 30, DataTypeInfo::UNSIGNED}, // 0 .. 34 years
    expectedVectorCSV.at(35).uiVal = 17;          // "Wärmepumpe läuft seit",                                                0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h
    expectedVectorCSV.at(36).uiVal = 18;          // "Zweiter Wärmeerzeuger 2 läuft seit",                                   0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h -> Heats up to 60 deg
    expectedVectorCSV.at(37).uiVal = 19;          // "Netzeinschaltverzögerung",                                             0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h
    expectedVectorCSV.at(38).uiVal = 20;          // "Schaltspielsperre Aus",                                                0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h
    expectedVectorCSV.at(39).uiVal = 21;          // "Schaltspielsperre Ein",                                                0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h
    expectedVectorCSV.at(40).uiVal = 22;          // "Verdichter-Standzeit",                                                 0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h
    expectedVectorCSV.at(41).uiVal = 23;          // "Heizungsregler Mehr-Zeit",                                             0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h
    expectedVectorCSV.at(42).uiVal = 24;          // "Heizungsregler Weniger-Zeit",                                          0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h
    expectedVectorCSV.at(43).uiVal = 25;          // "Thermische Desinfektion läuft seit",                                   0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h -> Heats up to 60 deg
    expectedVectorCSV.at(44).uiVal = 26;          // "Sperre Warmwasser",                                                    0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h -> potentially 0
    expectedVectorCSV.at(45).uiVal = 27;          // "Wärmepumpentyp|0 = ERC|Typenschlüssel",                                0, "-",         7, DataTypeInfo::UNSIGNED}, // enum 0..75 < 2^7 = 128 -> potentially  const
    expectedVectorCSV.at(46).uiVal = 3;           // "Bivalenzstufe",                                                        0, "enum",  2, DataTypeInfo::UNSIGNED },  // According the enum
    expectedVectorCSV.at(47).uiVal = 4;           // (1<<3)-1;  // "Betriebszustand",                                        0, "enum",      3, DataTypeInfo::UNSIGNED },  // According the enum
    expectedVectorCSV.at(48).uiVal = 30;          // "Wärmemengenzähler Heizung",                                           10, "kWh",      28, DataTypeInfo::UNSIGNED}, // 460 times more than today
    expectedVectorCSV.at(49).uiVal = 31;          // "Wärmemengenzähler Brauchwasser",                                      10, "kWh",      26, DataTypeInfo::UNSIGNED}, // 382 times more than today
    expectedVectorCSV.at(50).uiVal = 32;          // "Wärmemengenzähler Gesamt",                                            10, "kWh",      28, DataTypeInfo::UNSIGNED}, // 350 times more than today
    expectedVectorCSV.at(51).uiVal = 33;          // "Wärmemengenzähler Durchfluss",                                         1, "l / h",    10, DataTypeInfo::UNSIGNED}, // 0 .. 1024l/h  -> potentially unused

    expectedVectorCSV.at(0).divisor = 1;
    expectedVectorCSV.at(1).divisor = 1;
    for(uint32_t cnt = 0; cnt < expecedNrColExclusiveTimeStamp; cnt++) {
        expectedVectorCSV.at(2 + cnt).divisor = expectedDivisor_v2_v3.at(cnt);
    }

    EXPECT_CALL(*csvWriter, writeHeader(*expectedHeaderLine_v2_v3)).WillOnce(Return(true));
    EXPECT_CALL(*csvWriter, writeHeader(*expectedUnitLine_v2_v3)).WillOnce(Return(true));
    EXPECT_CALL(*csvWriter, writeData(compareVectors(expectedVectorCSV), expecedNrColExclusiveTimeStamp)).WillOnce(Return(true));
    feedback = fileDataReader->decodeData();
    EXPECT_TRUE(feedback);
}

}  // unnamed namespace
