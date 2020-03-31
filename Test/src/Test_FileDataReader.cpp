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
    }

    void TearDown() override
    {
        // Dump Trace buffer
    }

    std::shared_ptr<MockFileDataWriterCSV> csvWriter;
    FileDataReader_SPtr fileDataReader;
    std::shared_ptr<std::string> expectedHeaderLine_v1;
    std::shared_ptr<std::string> expectedHeaderLine_v2_v3;
};

// ::testing::AssertionResult compareVectors(std::vector<uint32_t> expVec, std::vector<uint32_t> actVec)
// {
//    if (expVec.size() != actVec.size()) {
//        return ::testing::AssertionFailure() << "Exp len: " << expVec.size() << " length is not act len: " << actVec.size();
//    }
//    else {
//        return ::testing::AssertionSuccess();
//    }
////    else
////        return ::testing::AssertionFailure()
////               << val << " is outside the range " << a << " to " << b;
// }
MATCHER_P (compareVectors, expVec, "")
{
    bool feedback = true;
    std::cout << "-------------------------------------" << std::endl;
    if(expVec.size() != arg.size()) {
        std::cout << "Exp len: " << expVec.size() << " length is not act len: " << arg.size() << std::endl;
        feedback = false;
    } else {
        for(uint32_t cnt = 0; cnt < arg.size(); cnt++) {
            if(expVec.at(cnt) != arg.at(cnt)) {
                std::cout << "expect(" << cnt << ") is " << expVec.at(cnt) << " != actual(" << cnt << ") is " << arg.at(cnt) << std::endl;
                feedback = false;
//            } else {
//                std::cout << "expect(" << cnt << ") is " << expVec.at(cnt) << " == actual(" << cnt << ") is " << arg.at(cnt) << std::endl;
            }
        }
    }
    std::cout << "-------------------------------------" << std::endl;
    return feedback;
}

TEST_F(Test_FileDataReader, fileVersion1)
{
    bool feedback;
    fileDataReader = std::make_shared<FileDataReader>("Test/testData/fileVersion_01.dat", csvWriter); // real test
//    fileDataReader = std::make_shared<FileDataReader>("../testData/fileVersion_01.dat", csvWriter); //debugger
    feedback = fileDataReader->readHeaderData();
    EXPECT_TRUE(feedback);

    uint32_t expecedNrColExclusiveTimeStamp = 69;

    std::vector<uint32_t> expectedVectorCSV(2 * (expecedNrColExclusiveTimeStamp + 2));
    expectedVectorCSV.at(0) = 0x5e6677B4;         // B4 77 66 5E
    expectedVectorCSV.at(1) = 0x00000000;         // 00 00 00 00
    expectedVectorCSV.at(2) = 0x00000110;         // 10 01 00 00
    expectedVectorCSV.at(3) = 0x00000110;         // 10 01 00 00
    expectedVectorCSV.at(4) = 0x00000116;         // 16 01 00 00
    expectedVectorCSV.at(5) = 0x00000032;         // 32 00 00 00
    expectedVectorCSV.at(6) = 0x0000018C;         // 8C 01 00 00
    expectedVectorCSV.at(7) = 0x0000001C;         // 1C 00 00 00
    expectedVectorCSV.at(8) = 0x00000028;         // 28 00 00 00
    expectedVectorCSV.at(9) = 0x000001EB;         // EB 01 00 00
    expectedVectorCSV.at(10) = 0x000001F4;        // F4 01 00 00
    expectedVectorCSV.at(11) = 0x00000058;        // 58 00 00 00
    expectedVectorCSV.at(12) = 0x00000062;        // 62 00 00 00
    expectedVectorCSV.at(13) = 0x000002EE;        // EE 02 00 00
    expectedVectorCSV.at(14) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(15) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(16) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(17) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(18) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(19) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(20) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(21) = 0x00000001;        // 01 00 00 00
    expectedVectorCSV.at(22) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(23) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(24) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(25) = 0x00000001;        // 01 00 00 00
    expectedVectorCSV.at(26) = 0x00000001;        // 01 00 00 00
    expectedVectorCSV.at(27) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(28) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(29) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(30) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(31) = 0x00000001;        // 01 00 00 00
    expectedVectorCSV.at(32) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(33) = 0x00000001;        // 01 00 00 00
    expectedVectorCSV.at(34) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(35) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(36) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(37) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(38) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(39) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(40) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(41) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(42) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(43) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(44) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(45) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(46) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(47) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(48) = 0x01644F2E;        // 2E 4F 64 01
    expectedVectorCSV.at(49) = 0x00002EF3;        // F3 2E 00 00
    expectedVectorCSV.at(50) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(51) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(52) = 0x002E47DF;        // DF 47 2E 00
    expectedVectorCSV.at(53) = 0x003613A4;        // A4 13 36 00
    expectedVectorCSV.at(54) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(55) = 0x01644F2E;        // 2E 4F 64 01
    expectedVectorCSV.at(56) = 0x010F9930;        // 30 99 0F 01
    expectedVectorCSV.at(57) = 0x0054B5D7;        // D7 B5 54 00
    expectedVectorCSV.at(58) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(59) = 0x00000001;        // 01 00 00 00
    expectedVectorCSV.at(60) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(61) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(62) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(63) = 0x00000002;        // 02 00 00 00
    expectedVectorCSV.at(64) = 0x000000DF;        // DF 00 00 00
    expectedVectorCSV.at(65) = 0x00000129;        // 29 01 00 00
    expectedVectorCSV.at(66) = 0x00000020;        // 20 00 00 00
    expectedVectorCSV.at(67) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(68) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(69) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(70) = 0x00000003;        // 03 00 00 00
    expectedVectorCSV.at(71) = 0x5E6677F0;        // F0 77 66 5E
    expectedVectorCSV.at(72) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(73) = 0x0000010E;        // 0E 01 00 00
    expectedVectorCSV.at(74) = 0x0000010E;        // 0E 01 00 00
    expectedVectorCSV.at(75) = 0x00000116;        // 16 01 00 00
    expectedVectorCSV.at(76) = 0x00000032;        // 32 00 00 00
    expectedVectorCSV.at(77) = 0x0000017C;        // 7C 01 00 00
    expectedVectorCSV.at(78) = 0x0000001B;        // 1B 00 00 00
    expectedVectorCSV.at(79) = 0x00000028;        // 28 00 00 00
    expectedVectorCSV.at(80) = 0x000001EB;        // EB 01 00 00
    expectedVectorCSV.at(81) = 0x000001F4;        // F4 01 00 00
    expectedVectorCSV.at(82) = 0x00000059;        // 59 00 00 00
    expectedVectorCSV.at(83) = 0x00000063;        // 63 00 00 00
    expectedVectorCSV.at(84) = 0x000002EE;        // EE 02 00 00
    expectedVectorCSV.at(85) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(86) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(87) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(88) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(89) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(90) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(91) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(92) = 0x00000001;        // 01 00 00 00
    expectedVectorCSV.at(93) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(94) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(95) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(96) = 0x00000001;        // 01 00 00 00
    expectedVectorCSV.at(97) = 0x00000001;        // 01 00 00 00
    expectedVectorCSV.at(98) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(99) = 0x00000000;        // 00 00 00 00
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

    EXPECT_CALL(*csvWriter, writeHeader(*expectedHeaderLine_v1)).WillOnce(Return(true));
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


    uint32_t expecedNrColExclusiveTimeStamp = 50;

    std::vector<uint32_t> expectedVectorCSV(3 * (expecedNrColExclusiveTimeStamp + 2));
    expectedVectorCSV.at(0) = 0x5E7A90F0;        // F0 90 7A 5E
    expectedVectorCSV.at(1) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(2) = 0x00000173;        // 73 01 00 00
    expectedVectorCSV.at(3) = 0x0000013E;        // 3E 01 00 00
    expectedVectorCSV.at(4) = 0x00000123;        // 23 01 00 00
    expectedVectorCSV.at(5) = 0x0000027F;        // 7F 02 00 00
    expectedVectorCSV.at(6) = 0xFFFFFFEF;        // EF FF FF FF
    expectedVectorCSV.at(7) = 0xFFFFFFFF;        // FF FF FF FF
    expectedVectorCSV.at(8) = 0x000001D8;        // D8 01 00 00
    expectedVectorCSV.at(9) = 0x000001F4;        // F4 01 00 00
    expectedVectorCSV.at(10) = 0x00000042;       // 42 00 00 00
    expectedVectorCSV.at(11) = 0x00000024;       // 24 00 00 00
    expectedVectorCSV.at(12) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(13) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(14) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(15) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(16) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(17) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(18) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(19) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(20) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(21) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(22) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(23) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(24) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(25) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(26) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(27) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(28) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(29) = 0x0168BC84;       // 84 BC 68 01
    expectedVectorCSV.at(30) = 0x00002F51;       // 51 2F 00 00
    expectedVectorCSV.at(31) = 0x003693FF;       // FF 93 36 00
    expectedVectorCSV.at(32) = 0x0168BC84;       // 84 BC 68 01
    expectedVectorCSV.at(33) = 0x011357EB;       // EB 57 13 01
    expectedVectorCSV.at(34) = 0x00556472;       // 72 64 55 00
    expectedVectorCSV.at(35) = 0x00000C17;       // 17 0C 00 00
    expectedVectorCSV.at(36) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(37) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(38) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(39) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(40) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(41) = 0x00000118;       // 18 01 00 00
    expectedVectorCSV.at(42) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(43) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(44) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(45) = 0x0000003C;       // 3C 00 00 00
    expectedVectorCSV.at(46) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(47) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(48) = 0x0008E4D0;       // D0 E4 08 00
    expectedVectorCSV.at(49) = 0x0002AD4E;       // 4E AD 02 00
    expectedVectorCSV.at(50) = 0x000B9631;       // 31 96 0B 00
    expectedVectorCSV.at(51) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(52) = 0x5E7A912C;       // 2C 91 7A 5E
    expectedVectorCSV.at(53) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(54) = 0x00000173;       // 73 01 00 00
    expectedVectorCSV.at(55) = 0x0000013E;       // 3E 01 00 00
    expectedVectorCSV.at(56) = 0x00000123;       // 23 01 00 00
    expectedVectorCSV.at(57) = 0x00000280;       // 80 02 00 00
    expectedVectorCSV.at(58) = 0xFFFFFFEE;       // EE FF FF FF
    expectedVectorCSV.at(59) = 0xFFFFFFFF;       // FF FF FF FF
    expectedVectorCSV.at(60) = 0x000001D7;       // D7 01 00 00
    expectedVectorCSV.at(61) = 0x000001F4;       // F4 01 00 00
    expectedVectorCSV.at(62) = 0x00000042;       // 42 00 00 00
    expectedVectorCSV.at(63) = 0x00000025;       // 25 00 00 00
    expectedVectorCSV.at(64) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(65) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(66) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(67) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(68) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(69) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(70) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(71) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(72) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(73) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(74) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(75) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(76) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(77) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(78) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(79) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(80) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(81) = 0x0168BCC0;       // C0 BC 68 01
    expectedVectorCSV.at(82) = 0x00002F51;       // 51 2F 00 00
    expectedVectorCSV.at(83) = 0x003693FF;       // FF 93 36 00
    expectedVectorCSV.at(84) = 0x0168BCC0;       // C0 BC 68 01
    expectedVectorCSV.at(85) = 0x01135827;       // 27 58 13 01
    expectedVectorCSV.at(86) = 0x00556472;       // 72 64 55 00
    expectedVectorCSV.at(87) = 0x00000C53;       // 53 0C 00 00
    expectedVectorCSV.at(88) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(89) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(90) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(91) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(92) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(93) = 0x00000118;       // 18 01 00 00
    expectedVectorCSV.at(94) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(95) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(96) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(97) = 0x0000003C;       // 3C 00 00 00
    expectedVectorCSV.at(98) = 0x00000001;       // 01 00 00 00
    expectedVectorCSV.at(99) = 0x00000000;       // 00 00 00 00
    expectedVectorCSV.at(100) = 0x0008E4D2;      // D2 E4 08 00
    expectedVectorCSV.at(101) = 0x0002AD4E;      // 4E AD 02 00
    expectedVectorCSV.at(102) = 0x000B9633;      // 33 96 0B 00
    expectedVectorCSV.at(103) = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(104) = 0x5E7A9168;      // 68 91 7A 5E
    expectedVectorCSV.at(105) = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(106) = 0x00000173;      // 73 01 00 00
    expectedVectorCSV.at(107) = 0x0000013F;      // 3F 01 00 00
    expectedVectorCSV.at(108) = 0x00000123;      // 23 01 00 00
    expectedVectorCSV.at(109) = 0x00000281;      // 81 02 00 00
    expectedVectorCSV.at(110) = 0xFFFFFFEF;      // EF FF FF FF
    expectedVectorCSV.at(111) = 0xFFFFFFFF;      // FF FF FF FF
    expectedVectorCSV.at(112) = 0x000001D8;      // D8 01 00 00
    expectedVectorCSV.at(113) = 0x000001F4;      // F4 01 00 00
    expectedVectorCSV.at(114) = 0x00000042;      // 42 00 00 00
    expectedVectorCSV.at(115) = 0x00000024;      // 24 00 00 00
    expectedVectorCSV.at(116) = 0x00000001;      // 01 00 00 00
    expectedVectorCSV.at(117) = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(118) = 0x00000001;      // 01 00 00 00
    expectedVectorCSV.at(119) = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(120) = 0x00000001;      // 01 00 00 00
    expectedVectorCSV.at(121) = 0x00000001;      // 01 00 00 00
    expectedVectorCSV.at(122) = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(123) = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(124) = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(125) = 0x00000001;      // 01 00 00 00
    expectedVectorCSV.at(126) = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(127) = 0x00000001;      // 01 00 00 00
    expectedVectorCSV.at(128) = 0x00000001;      // 01 00 00 00
    expectedVectorCSV.at(129) = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(130) = 0x00000001;      // 01 00 00 00
    expectedVectorCSV.at(131) = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(132) = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(133) = 0x0168BCFC;      // FC BC 68 01
    expectedVectorCSV.at(134) = 0x00002F51;      // 51 2F 00 00
    expectedVectorCSV.at(135) = 0x003693FF;      // FF 93 36 00
    expectedVectorCSV.at(136) = 0x0168BCFC;      // FC BC 68 01
    expectedVectorCSV.at(137) = 0x01135863;      // 63 58 13 01
    expectedVectorCSV.at(138) = 0x00556472;      // 72 64 55 00
    expectedVectorCSV.at(139) = 0x00000C8F;      // 8F 0C 00 00
    expectedVectorCSV.at(140) = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(141) = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(142) = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(143) = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(144) = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(145) = 0x00000118;      // 18 01 00 00
    expectedVectorCSV.at(146) = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(147) = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(148) = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(149) = 0x0000003C;      // 3C 00 00 00
    expectedVectorCSV.at(150) = 0x00000001;      // 01 00 00 00
    expectedVectorCSV.at(151) = 0x00000000;      // 00 00 00 00
    expectedVectorCSV.at(152) = 0x0008E4D4;      // D4 E4 08 00
    expectedVectorCSV.at(153) = 0x0002AD4E;      // 4E AD 02 00
    expectedVectorCSV.at(154) = 0x000B9635;      // 35 96 0B 00
    expectedVectorCSV.at(155) = 0x00000000;      // 00 00 00 00

    EXPECT_CALL(*csvWriter, writeHeader(*expectedHeaderLine_v2_v3)).WillOnce(Return(true));
    EXPECT_CALL(*csvWriter, writeData(expectedVectorCSV, expecedNrColExclusiveTimeStamp)).WillOnce(Return(true));
    feedback = fileDataReader->decodeData();
    EXPECT_TRUE(feedback);
}

TEST_F(Test_FileDataReader, unsupportedFileVersion)
{
    bool feedback;
    fileDataReader = std::make_shared<FileDataReader>("Test/testData/fileVersion_NotSupported.dat", csvWriter); // real test
    feedback = fileDataReader->readHeaderData();
    EXPECT_FALSE(feedback);
}

TEST_F(Test_FileDataReader, fileVersion03_allZeros)
{
    bool feedback;
    fileDataReader = std::make_shared<FileDataReader>("Test/testData/fileVersion_03_allZeros.dat", csvWriter); // real test
    feedback = fileDataReader->readHeaderData();
    EXPECT_TRUE(feedback);


    uint32_t expecedNrColExclusiveTimeStamp = 50;

    std::vector<uint32_t> expectedVectorCSV(1 * (expecedNrColExclusiveTimeStamp + 2), 0);

    EXPECT_CALL(*csvWriter, writeHeader(*expectedHeaderLine_v2_v3)).WillOnce(Return(true));
    EXPECT_CALL(*csvWriter, writeData(expectedVectorCSV, expecedNrColExclusiveTimeStamp)).WillOnce(Return(true));
    feedback = fileDataReader->decodeData();
    EXPECT_TRUE(feedback);
}

TEST_F(Test_FileDataReader, fileVersion03_validTimestamp_allMaxValues)
{
    bool feedback;
    fileDataReader = std::make_shared<FileDataReader>("Test/testData/fileVersion_03_allMax.dat", csvWriter); // real test
    // fileDataReader = std::make_shared<FileDataReader>("../testData/fileVersion_03_allMax.dat", csvWriter); //debugger
    feedback = fileDataReader->readHeaderData();
    EXPECT_TRUE(feedback);

    uint32_t expecedNrColExclusiveTimeStamp = 50;

    std::vector<uint32_t> expectedVectorCSV(1 * (expecedNrColExclusiveTimeStamp + 2), 0);
    expectedVectorCSV.at(0) = 0x0bc0dcc0;  // 1. April 1976 0640
    expectedVectorCSV.at(1) = 0;           // Second part of the timestamp
    expectedVectorCSV.at(2) = 1023;        // "Vorlauftemperatur Heizkreis",                                         10, "°C",       10, DataTypeInfo::UNSIGNED }, // 0..102.3
    expectedVectorCSV.at(3) = 1023;        // "Rücklauftemperatur Heizkreis",                                        10, "°C",       10, DataTypeInfo::UNSIGNED }, // 0..102.3
    expectedVectorCSV.at(4) = 1023;        // "Rücklauf-Soll Heizkreis",                                             10, "°C",       10, DataTypeInfo::UNSIGNED }, // 0..102.3
    expectedVectorCSV.at(5) = 1023;        // "Heisgastemperatur",                                                   10, "°C",       10, DataTypeInfo::UNSIGNED }, // 0..102.3
    expectedVectorCSV.at(6) = 1023;        // "Aussentemperatur",                                                    10, "°C",       11, DataTypeInfo::SIGNED},    // -102.4 .. 102.3
    expectedVectorCSV.at(7) = 511;         // "Durchschnittstemperatur Aussen über 24 h (Funktion Heizgrenze)",      10, "°C",       10, DataTypeInfo::SIGNED},    // -51.2 .. 51.1
    expectedVectorCSV.at(8) = 1023;        // "Warmwasser Ist-Temperatur",                                           10, "°C",       10, DataTypeInfo::UNSIGNED }, // 0..102.3
    expectedVectorCSV.at(9) = 1023;        // "Warmwasser Soll-Temperatur",                                          10, "°C",       10, DataTypeInfo::UNSIGNED }, // 0..102.3
    expectedVectorCSV.at(10) = 1023;       // "Wärmequellen-Eintrittstemperatur",                                    10, "°C",       10, DataTypeInfo::UNSIGNED }, // 0..102.3
    expectedVectorCSV.at(11) = 1023;       // "Wärmequellen-Austrittstemperatur",                                    10, "°C",       10, DataTypeInfo::UNSIGNED }, // 0..102.3
    expectedVectorCSV.at(12) = 1;          // "Eingang Abtauende / Soledruck / Durchfluss",                           0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(13) = 1;          // "Brauchwarmwasserthermostat",                                           0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(14) = 1;          // "EVU Sperre",                                                           0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(15) = 1;          // "Hochdruck Kältekreis",                                                 0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(16) = 1;          // "Motorschutz OK",                                                       0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(17) = 1;          // "Niederdruck",                                                          0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(18) = 1;          // "Überwachungskontakt für Potentiostat",                                 0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(19) = 1;          // "Abtauventil",                                                          0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(20) = 1;          // "Brauchwasserpumpe/Umstellventil",                                      0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(21) = 1;          // "Heizungsumwälzpumpe",                                                  0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(22) = 1;          // "Ventilation (Lüftung)",                                                0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(23) = 1;          // "Solepumpe/Ventilator",                                                 0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(24) = 1;          // "Verdichter 1",                                                         0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(25) = 1;          // "Zirkulationspumpe",                                                    0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(26) = 1;          // "Zusatzumwälzpumpe",                                                    0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(27) = 1;          // "Steuersignal Zusatzheizung v. Heizung",                                0,  "-",        1, DataTypeInfo::BOOL },
    expectedVectorCSV.at(28) = 1;          // "Steuersignal Zusatzheizung/Störsignal",                                0,  "-",        1, DataTypeInfo::BOOL },

    expectedVectorCSV.at(29) = (1 << 30) - 1;              // "Betriebsstunden Verdichter 1",                                         0, "Sekunden", 30, DataTypeInfo::UNSIGNED}, // 0 .. 34 years

    expectedVectorCSV.at(30) = (1 << 22) - 1; // "Impulse Verdichter 1",                                                 0, "Impulse" , 22, DataTypeInfo::UNSIGNED}, // 346 times more than today

    expectedVectorCSV.at(31) = (1 << 30) - 1;// "Betriebsstunden Zweiter Wärmeerzeuger 2",                              0, "Sekunden", 30, DataTypeInfo::UNSIGNED}, // 0 .. 34 years -> Heats up to 60 deg

    expectedVectorCSV.at(32) = (1 << 30) - 1;  // "Betriebsstunden Wärmepumpe",                                           0, "Sekunden", 30, DataTypeInfo::UNSIGNED}, // 0 .. 34 years
    expectedVectorCSV.at(33) = (1 << 30) - 1;  // "Betriebsstunden Heizung",                                              0, "Sekunden", 30, DataTypeInfo::UNSIGNED}, // 0 .. 34 years
    expectedVectorCSV.at(34) = (1 << 30) - 1;  // "Betriebsstunden Warmwasser",                                           0, "Sekunden", 30, DataTypeInfo::UNSIGNED}, // 0 .. 34 years


    expectedVectorCSV.at(35) = (1 << 17) - 1;  // "Wärmepumpe läuft seit",                                                0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h
    expectedVectorCSV.at(36) = (1 << 17) - 1;  // "Zweiter Wärmeerzeuger 2 läuft seit",                                   0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h -> Heats up to 60 deg
    expectedVectorCSV.at(37) = (1 << 17) - 1;  // "Netzeinschaltverzögerung",                                             0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h
    expectedVectorCSV.at(38) = (1 << 17) - 1;  // "Schaltspielsperre Aus",                                                0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h
    expectedVectorCSV.at(39) = (1 << 17) - 1;  // "Schaltspielsperre Ein",                                                0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h
    expectedVectorCSV.at(40) = (1 << 17) - 1;  // "Verdichter-Standzeit",                                                 0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h
    expectedVectorCSV.at(41) = (1 << 17) - 1;  // "Heizungsregler Mehr-Zeit",                                             0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h
    expectedVectorCSV.at(42) = (1 << 17) - 1;  // "Heizungsregler Weniger-Zeit",                                          0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h
    expectedVectorCSV.at(43) = (1 << 17) - 1;  // "Thermische Desinfektion läuft seit",                                   0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h -> Heats up to 60 deg
    expectedVectorCSV.at(44) = (1 << 17) - 1;  // "Sperre Warmwasser",                                                    0, "Sekunden", 17, DataTypeInfo::UNSIGNED}, // 0 .. 36h -> potentially 0

    expectedVectorCSV.at(45) = (1 << 7) - 1;  // "Wärmepumpentyp|0 = ERC|Typenschlüssel",                                0, "-",         7, DataTypeInfo::UNSIGNED}, // enum 0..75 < 2^7 = 128 -> potentially  const

    expectedVectorCSV.at(46) = (1 << 2) - 1;      // "Bivalenzstufe",                                                      0, "enum",  2, DataTypeInfo::UNSIGNED },  // According the enum
    expectedVectorCSV.at(47) = (1 << 3) - 1;  // (1<<3)-1;  // "Betriebszustand",                                                      0, "enum",      3, DataTypeInfo::UNSIGNED },  // According the enum

    expectedVectorCSV.at(48) = (1 << 28) - 1;  // "Wärmemengenzähler Heizung",                                           10, "kWh",      28, DataTypeInfo::UNSIGNED}, // 460 times more than today
    expectedVectorCSV.at(49) = (1 << 26) - 1;  // "Wärmemengenzähler Brauchwasser",                                      10, "kWh",      26, DataTypeInfo::UNSIGNED}, // 382 times more than today
    expectedVectorCSV.at(50) = (1 << 28) - 1;  // "Wärmemengenzähler Gesamt",                                            10, "kWh",      28, DataTypeInfo::UNSIGNED}, // 350 times more than today
    expectedVectorCSV.at(51) = (1 << 10) - 1;  // "Wärmemengenzähler Durchfluss",                                         1, "l / h",    10, DataTypeInfo::UNSIGNED}, // 0 .. 1024l/h  -> potentially unused


    EXPECT_CALL(*csvWriter, writeHeader(*expectedHeaderLine_v2_v3)).WillOnce(Return(true));
    EXPECT_CALL(*csvWriter, writeData(compareVectors(expectedVectorCSV), expecedNrColExclusiveTimeStamp)).WillOnce(Return(true));
    feedback = fileDataReader->decodeData();
    EXPECT_TRUE(feedback);
}



TEST_F(Test_FileDataReader, DISABLED_fileVersion03_enumerateValues)
{
    bool feedback;
    fileDataReader = std::make_shared<FileDataReader>("Test/testData/fileVersion_03_allMax.dat", csvWriter); // real test
    feedback = fileDataReader->readHeaderData();
    EXPECT_TRUE(feedback);

    uint32_t expecedNrColExclusiveTimeStamp = 50;

    std::vector<uint32_t> expectedVectorCSV(1 * (expecedNrColExclusiveTimeStamp + 2), 0);
    expectedVectorCSV.at(0) = 0x0bc0dcc0;
#if 0

    {  10, /*ID_WEB_Temperatur_TVL*/ "Vorlauftemperatur Heizkreis", 10, "°C", 10, DataTypeInfo::UNSIGNED },                                                              // 0..102.3
    {  11, /*ID_WEB_Temperatur_TRL*/ "Rücklauftemperatur Heizkreis", 10, "°C", 10, DataTypeInfo::UNSIGNED },                                                             // 0..102.3
    {  12, /*ID_WEB_Sollwert_TRL_HZ*/ "Rücklauf-Soll Heizkreis", 10, "°C", 10, DataTypeInfo::UNSIGNED },                                                                 // 0..102.3
    {  14, /*ID_WEB_Temperatur_THG*/ "Heisgastemperatur", 10, "°C", 10, DataTypeInfo::UNSIGNED },                                                                        // 0..102.3
    {  15, /*ID_WEB_Temperatur_TA*/ "Aussentemperatur", 10, "°C", 11, DataTypeInfo::SIGNED},                                                                             // -102.4 .. 102.3
    {  16, /*ID_WEB_Mitteltemperatur*/ "Durchschnittstemperatur Aussen über 24 h (Funktion Heizgrenze)", 10, "°C", 10, DataTypeInfo::SIGNED},                            // -51.2 .. 51.1
    {  17, /*ID_WEB_Temperatur_TBW*/ "Warmwasser Ist-Temperatur", 10, "°C", 10, DataTypeInfo::UNSIGNED },                                                                // 0..102.3
    {  18, /*ID_WEB_Einst_BWS_akt*/ "Warmwasser Soll-Temperatur", 10, "°C", 10, DataTypeInfo::UNSIGNED },                                                                // 0..102.3
    {  19, /*ID_WEB_Temperatur_TWE*/ "Wärmequellen-Eintrittstemperatur", 10, "°C", 10, DataTypeInfo::UNSIGNED },                                                         // 0..102.3
    {  20, /*ID_WEB_Temperatur_TWA*/ "Wärmequellen-Austrittstemperatur", 10, "°C", 10, DataTypeInfo::UNSIGNED },                                                         // 0..102.3
    {  29, /*ID_WEB_ASDin*/ "Eingang Abtauende / Soledruck / Durchfluss", 0, "-", 1, DataTypeInfo::BOOL },
    {  30, /*ID_WEB_BWTin_Eingang*/ "Brauchwarmwasserthermostat", 0, "-", 1, DataTypeInfo::BOOL },
    {  31, /*ID_WEB_EVUin_Eingang*/ "EVU Sperre", 0, "-", 1, DataTypeInfo::BOOL },
    {  32, /*ID_WEB_HDin_Eingang*/ "Hochdruck Kältekreis", 0, "-", 1, DataTypeInfo::BOOL },
    {  33, /*ID_WEB_MOTin_Eingang*/ "Motorschutz OK", 0, "-", 1, DataTypeInfo::BOOL },
    {  34, /*ID_WEB_NDin_Eingang*/ "Niederdruck", 0, "-", 1, DataTypeInfo::BOOL },
    {  35, /*ID_WEB_PEXin_Eingang*/ "Überwachungskontakt für Potentiostat", 0, "-", 1, DataTypeInfo::BOOL },
    {  37, /*ID_WEB_AVout_Ausgang*/ "Abtauventil", 0, "-", 1, DataTypeInfo::BOOL },
    {  38, /*ID_WEB_BUPout_Ausgang*/ "Brauchwasserpumpe/Umstellventil", 0, "-", 1, DataTypeInfo::BOOL },
    {  39, /*ID_WEB_HUPout_Ausgang*/ "Heizungsumwälzpumpe", 0, "-", 1, DataTypeInfo::BOOL },
    {  42, /*ID_WEB_VENout_Ausgang*/ "Ventilation (Lüftung)", 0, "-", 1, DataTypeInfo::BOOL },
    {  43, /*ID_WEB_VBOout_Ausgang*/ "Solepumpe/Ventilator", 0, "-", 1, DataTypeInfo::BOOL },
    {  44, /*ID_WEB_VD1out_Ausgang*/ "Verdichter 1", 0, "-", 1, DataTypeInfo::BOOL },
    {  46, /*ID_WEB_ZIPout_Ausgang*/ "Zirkulationspumpe", 0, "-", 1, DataTypeInfo::BOOL },
    {  47, /*ID_WEB_ZUPout_Ausgang*/ "Zusatzumwälzpumpe", 0, "-", 1, DataTypeInfo::BOOL },
    {  48, /*ID_WEB_ZW1out_Ausgang*/ "Steuersignal Zusatzheizung v. Heizung", 0, "-", 1, DataTypeInfo::BOOL },
    {  49, /*ID_WEB_ZW2SSTout_Ausgang*/ "Steuersignal Zusatzheizung/Störsignal", 0, "-", 1, DataTypeInfo::BOOL },
    {  56, /*ID_WEB_Zaehler_BetrZeitVD1*/ "Betriebsstunden Verdichter 1", 0, "Sekunden", 30, DataTypeInfo::UNSIGNED},                                                   // 0 .. 34 years
    {  57, /*ID_WEB_Zaehler_BetrZeitImpVD1*/ "Impulse Verdichter 1", 0, "Impulse", 22, DataTypeInfo::UNSIGNED},                                                         // 346 times more than today
    {  61, /*ID_WEB_Zaehler_BetrZeitZWE2*/ "Betriebsstunden Zweiter Wärmeerzeuger 2", 0, "Sekunden", 30, DataTypeInfo::UNSIGNED},                                       // 0 .. 34 years -> Heats up to 60 deg
    {  63, /*ID_WEB_Zaehler_BetrZeitWP*/ "Betriebsstunden Wärmepumpe", 0, "Sekunden", 30, DataTypeInfo::UNSIGNED},                                                      // 0 .. 34 years
    {  64, /*ID_WEB_Zaehler_BetrZeitHz*/ "Betriebsstunden Heizung", 0, "Sekunden", 30, DataTypeInfo::UNSIGNED},                                                         // 0 .. 34 years
    {  65, /*ID_WEB_Zaehler_BetrZeitBW*/ "Betriebsstunden Warmwasser", 0, "Sekunden", 30, DataTypeInfo::UNSIGNED},                                                      // 0 .. 34 years
    {  67, /*ID_WEB_Time_WPein_akt*/ "Wärmepumpe läuft seit", 0, "Sekunden", 17, DataTypeInfo::UNSIGNED},                                                               // 0 .. 36h
    {  69, /*ID_WEB_Time_ZWE2_akt*/ "Zweiter Wärmeerzeuger 2 läuft seit", 0, "Sekunden", 17, DataTypeInfo::UNSIGNED},                                                   // 0 .. 36h -> Heats up to 60 deg
    {  70, /*ID_WEB_Timer_EinschVerz*/ "Netzeinschaltverzögerung", 0, "Sekunden", 17, DataTypeInfo::UNSIGNED},                                                          // 0 .. 36h
    {  71, /*ID_WEB_Time_SSPAUS_akt*/ "Schaltspielsperre Aus", 0, "Sekunden", 17, DataTypeInfo::UNSIGNED},                                                              // 0 .. 36h
    {  72, /*ID_WEB_Time_SSPEIN_akt*/ "Schaltspielsperre Ein", 0, "Sekunden", 17, DataTypeInfo::UNSIGNED},                                                              // 0 .. 36h
    {  73, /*ID_WEB_Time_VDStd_akt*/ "Verdichter-Standzeit", 0, "Sekunden", 17, DataTypeInfo::UNSIGNED},                                                                // 0 .. 36h
    {  74, /*ID_WEB_Time_HRM_akt*/ "Heizungsregler Mehr-Zeit", 0, "Sekunden", 17, DataTypeInfo::UNSIGNED},                                                              // 0 .. 36h
    {  75, /*ID_WEB_Time_HRW_akt*/ "Heizungsregler Weniger-Zeit", 0, "Sekunden", 17, DataTypeInfo::UNSIGNED},                                                           // 0 .. 36h
    {  76, /*ID_WEB_Time_LGS_akt*/ "Thermische Desinfektion läuft seit", 0, "Sekunden", 17, DataTypeInfo::UNSIGNED},                                                    // 0 .. 36h -> Heats up to 60 deg
    {  77, /*ID_WEB_Time_SBW_akt*/ "Sperre Warmwasser", 0, "Sekunden", 17, DataTypeInfo::UNSIGNED},                                                                     // 0 .. 36h -> potentially 0
    {  78, /*ID_WEB_Code_WP_akt*/ "Wärmepumpentyp|0 = ERC|Typenschlüssel", 0, "-", 7, DataTypeInfo::UNSIGNED},                                                          // enum 0..75 < 2^7 = 128 -> potentially  const
    {  79, /*ID_WEB_BIV_Stufe_akt*/ "Bivalenzstufe", 0, "enum", 2, DataTypeInfo::UNSIGNED },                                                                 // According the enum
    {  80, /*ID_WEB_WP_BZ_akt*/ "Betriebszustand", 0, "enum", 3, DataTypeInfo::UNSIGNED },                                                                                // According the enum
    { 151, /*ID_WEB_WMZ_Heizung*/ "Wärmemengenzähler Heizung", 10, "kWh", 28, DataTypeInfo::UNSIGNED},                                                                  // 460 times more than today
    { 152, /*ID_WEB_WMZ_Brauchwasser*/ "Wärmemengenzähler Brauchwasser", 10, "kWh", 26, DataTypeInfo::UNSIGNED},                                                        // 382 times more than today
    { 154, /*ID_WEB_WMZ_Seit*/ "Wärmemengenzähler Gesamt", 10, "kWh", 28, DataTypeInfo::UNSIGNED},                                                                      // 350 times more than today
    { 155, /*ID_WEB_WMZ_Durchfluss*/ "Wärmemengenzähler Durchfluss", 1, "l / h", 10, DataTypeInfo::UNSIGNED},                                                           // 0 .. 1024l/h  -> potentially unused
#endif






    EXPECT_CALL(*csvWriter, writeHeader(*expectedHeaderLine_v2_v3)).WillOnce(Return(true));
    EXPECT_CALL(*csvWriter, writeData(expectedVectorCSV, expecedNrColExclusiveTimeStamp)).WillOnce(Return(true));
    feedback = fileDataReader->decodeData();
    EXPECT_TRUE(feedback);
}



TEST_F(Test_FileDataReader, DISABLED_template)
{
    bool feedback;
    fileDataReader = std::make_shared<FileDataReader>("Test/testData/fileVersion_02.dat", csvWriter); // real test
    feedback = fileDataReader->readHeaderData();
    EXPECT_TRUE(feedback);

    uint32_t expecedNrColExclusiveTimeStamp = 50;

    std::vector<uint32_t> expectedVectorCSV(3 * (expecedNrColExclusiveTimeStamp + 2));
    expectedVectorCSV.at(0) = 0x5E7A90F0;        // F0 90 7A 5E
    expectedVectorCSV.at(1) = 0x00000000;        // 00 00 00 00
    expectedVectorCSV.at(2) = 0x00000173;        // 73 01 00 00
    expectedVectorCSV.at(3) = 0x0000013E;        // 3E 01 00 00

    EXPECT_CALL(*csvWriter, writeHeader(*expectedHeaderLine_v2_v3)).WillOnce(Return(true));
    EXPECT_CALL(*csvWriter, writeData(expectedVectorCSV, expecedNrColExclusiveTimeStamp)).WillOnce(Return(true));
    feedback = fileDataReader->decodeData();
    EXPECT_TRUE(feedback);
}

}  // unnamed namespace



#if 0
{  10, /*ID_WEB_Temperatur_TVL*/ "Vorlauftemperatur Heizkreis", 10, "°C", 10, DataTypeInfo::UNSIGNED },                                                                  // 0..102.3
{  11, /*ID_WEB_Temperatur_TRL*/ "Rücklauftemperatur Heizkreis", 10, "°C", 10, DataTypeInfo::UNSIGNED },                                                                 // 0..102.3
{  12, /*ID_WEB_Sollwert_TRL_HZ*/ "Rücklauf-Soll Heizkreis", 10, "°C", 10, DataTypeInfo::UNSIGNED },                                                                     // 0..102.3
{  14, /*ID_WEB_Temperatur_THG*/ "Heisgastemperatur", 10, "°C", 10, DataTypeInfo::UNSIGNED },                                                                            // 0..102.3
{  15, /*ID_WEB_Temperatur_TA*/ "Aussentemperatur", 10, "°C", 11, DataTypeInfo::SIGNED},                                                                                 // -102.4 .. 102.3
{  16, /*ID_WEB_Mitteltemperatur*/ "Durchschnittstemperatur Aussen über 24 h (Funktion Heizgrenze)", 10, "°C", 10, DataTypeInfo::SIGNED},                                // -51.2 .. 51.1
{  17, /*ID_WEB_Temperatur_TBW*/ "Warmwasser Ist-Temperatur", 10, "°C", 10, DataTypeInfo::UNSIGNED },                                                                    // 0..102.3
{  18, /*ID_WEB_Einst_BWS_akt*/ "Warmwasser Soll-Temperatur", 10, "°C", 10, DataTypeInfo::UNSIGNED },                                                                    // 0..102.3
{  19, /*ID_WEB_Temperatur_TWE*/ "Wärmequellen-Eintrittstemperatur", 10, "°C", 10, DataTypeInfo::UNSIGNED },                                                             // 0..102.3
{  20, /*ID_WEB_Temperatur_TWA*/ "Wärmequellen-Austrittstemperatur", 10, "°C", 10, DataTypeInfo::UNSIGNED },                                                             // 0..102.3
{  29, /*ID_WEB_ASDin*/ "Eingang Abtauende / Soledruck / Durchfluss", 0, "-", 1, DataTypeInfo::BOOL },
{  30, /*ID_WEB_BWTin_Eingang*/ "Brauchwarmwasserthermostat", 0, "-", 1, DataTypeInfo::BOOL },
{  31, /*ID_WEB_EVUin_Eingang*/ "EVU Sperre", 0, "-", 1, DataTypeInfo::BOOL },
{  32, /*ID_WEB_HDin_Eingang*/ "Hochdruck Kältekreis", 0, "-", 1, DataTypeInfo::BOOL },
{  33, /*ID_WEB_MOTin_Eingang*/ "Motorschutz OK", 0, "-", 1, DataTypeInfo::BOOL },
{  34, /*ID_WEB_NDin_Eingang*/ "Niederdruck", 0, "-", 1, DataTypeInfo::BOOL },
{  35, /*ID_WEB_PEXin_Eingang*/ "Überwachungskontakt für Potentiostat", 0, "-", 1, DataTypeInfo::BOOL },
{  37, /*ID_WEB_AVout_Ausgang*/ "Abtauventil", 0, "-", 1, DataTypeInfo::BOOL },
{  38, /*ID_WEB_BUPout_Ausgang*/ "Brauchwasserpumpe/Umstellventil", 0, "-", 1, DataTypeInfo::BOOL },
{  39, /*ID_WEB_HUPout_Ausgang*/ "Heizungsumwälzpumpe", 0, "-", 1, DataTypeInfo::BOOL },
{  42, /*ID_WEB_VENout_Ausgang*/ "Ventilation (Lüftung)", 0, "-", 1, DataTypeInfo::BOOL },
{  43, /*ID_WEB_VBOout_Ausgang*/ "Solepumpe/Ventilator", 0, "-", 1, DataTypeInfo::BOOL },
{  44, /*ID_WEB_VD1out_Ausgang*/ "Verdichter 1", 0, "-", 1, DataTypeInfo::BOOL },
{  46, /*ID_WEB_ZIPout_Ausgang*/ "Zirkulationspumpe", 0, "-", 1, DataTypeInfo::BOOL },
{  47, /*ID_WEB_ZUPout_Ausgang*/ "Zusatzumwälzpumpe", 0, "-", 1, DataTypeInfo::BOOL },
{  48, /*ID_WEB_ZW1out_Ausgang*/ "Steuersignal Zusatzheizung v. Heizung", 0, "-", 1, DataTypeInfo::BOOL },
{  49, /*ID_WEB_ZW2SSTout_Ausgang*/ "Steuersignal Zusatzheizung/Störsignal", 0, "-", 1, DataTypeInfo::BOOL },
{  56, /*ID_WEB_Zaehler_BetrZeitVD1*/ "Betriebsstunden Verdichter 1", 0, "Sekunden", 30, DataTypeInfo::UNSIGNED},                                                       // 0 .. 34 years
{  57, /*ID_WEB_Zaehler_BetrZeitImpVD1*/ "Impulse Verdichter 1", 0, "Impulse", 22, DataTypeInfo::UNSIGNED},                                                             // 346 times more than today
{  61, /*ID_WEB_Zaehler_BetrZeitZWE2*/ "Betriebsstunden Zweiter Wärmeerzeuger 2", 0, "Sekunden", 30, DataTypeInfo::UNSIGNED},                                           // 0 .. 34 years -> Heats up to 60 deg
{  63, /*ID_WEB_Zaehler_BetrZeitWP*/ "Betriebsstunden Wärmepumpe", 0, "Sekunden", 30, DataTypeInfo::UNSIGNED},                                                          // 0 .. 34 years
{  64, /*ID_WEB_Zaehler_BetrZeitHz*/ "Betriebsstunden Heizung", 0, "Sekunden", 30, DataTypeInfo::UNSIGNED},                                                             // 0 .. 34 years
{  65, /*ID_WEB_Zaehler_BetrZeitBW*/ "Betriebsstunden Warmwasser", 0, "Sekunden", 30, DataTypeInfo::UNSIGNED},                                                          // 0 .. 34 years
{  67, /*ID_WEB_Time_WPein_akt*/ "Wärmepumpe läuft seit", 0, "Sekunden", 17, DataTypeInfo::UNSIGNED},                                                                   // 0 .. 36h
{  69, /*ID_WEB_Time_ZWE2_akt*/ "Zweiter Wärmeerzeuger 2 läuft seit", 0, "Sekunden", 17, DataTypeInfo::UNSIGNED},                                                       // 0 .. 36h -> Heats up to 60 deg
{  70, /*ID_WEB_Timer_EinschVerz*/ "Netzeinschaltverzögerung", 0, "Sekunden", 17, DataTypeInfo::UNSIGNED},                                                              // 0 .. 36h
{  71, /*ID_WEB_Time_SSPAUS_akt*/ "Schaltspielsperre Aus", 0, "Sekunden", 17, DataTypeInfo::UNSIGNED},                                                                  // 0 .. 36h
{  72, /*ID_WEB_Time_SSPEIN_akt*/ "Schaltspielsperre Ein", 0, "Sekunden", 17, DataTypeInfo::UNSIGNED},                                                                  // 0 .. 36h
{  73, /*ID_WEB_Time_VDStd_akt*/ "Verdichter-Standzeit", 0, "Sekunden", 17, DataTypeInfo::UNSIGNED},                                                                    // 0 .. 36h
{  74, /*ID_WEB_Time_HRM_akt*/ "Heizungsregler Mehr-Zeit", 0, "Sekunden", 17, DataTypeInfo::UNSIGNED},                                                                  // 0 .. 36h
{  75, /*ID_WEB_Time_HRW_akt*/ "Heizungsregler Weniger-Zeit", 0, "Sekunden", 17, DataTypeInfo::UNSIGNED},                                                               // 0 .. 36h
{  76, /*ID_WEB_Time_LGS_akt*/ "Thermische Desinfektion läuft seit", 0, "Sekunden", 17, DataTypeInfo::UNSIGNED},                                                        // 0 .. 36h -> Heats up to 60 deg
{  77, /*ID_WEB_Time_SBW_akt*/ "Sperre Warmwasser", 0, "Sekunden", 17, DataTypeInfo::UNSIGNED},                                                                         // 0 .. 36h -> potentially 0
{  78, /*ID_WEB_Code_WP_akt*/ "Wärmepumpentyp|0 = ERC|Typenschlüssel", 0, "-", 7, DataTypeInfo::UNSIGNED},                                                              // enum 0..75 < 2^7 = 128 -> potentially  const
{  79, /*ID_WEB_BIV_Stufe_akt*/ "Bivalenzstufe", 0, "enum", 2, DataTypeInfo::UNSIGNED },                                                                     // According the enum
{  80, /*ID_WEB_WP_BZ_akt*/ "Betriebszustand", 0, "enum", 3, DataTypeInfo::UNSIGNED },                                                                                    // According the enum
{ 151, /*ID_WEB_WMZ_Heizung*/ "Wärmemengenzähler Heizung", 10, "kWh", 28, DataTypeInfo::UNSIGNED},                                                                      // 460 times more than today
{ 152, /*ID_WEB_WMZ_Brauchwasser*/ "Wärmemengenzähler Brauchwasser", 10, "kWh", 26, DataTypeInfo::UNSIGNED},                                                            // 382 times more than today
{ 154, /*ID_WEB_WMZ_Seit*/ "Wärmemengenzähler Gesamt", 10, "kWh", 28, DataTypeInfo::UNSIGNED},                                                                          // 350 times more than today
{ 155, /*ID_WEB_WMZ_Durchfluss*/ "Wärmemengenzähler Durchfluss", 1, "l / h", 10, DataTypeInfo::UNSIGNED},                                                               // 0 .. 1024l/h  -> potentially unused
#endif
