#pragma once

#include "Common/src/ValueTableCommon.hpp"

class ValueTable_v4 : public ValueTableCommon
{
public:

    ValueTable_v4(bool enableLog);

    virtual ~ValueTable_v4();

    uint32_t getFileVersion() const override;
    uint32_t getSizeOfHeader() const override;

private:
    void initValueTable() override;

    const uint32_t FILE_Version = 4;
    const uint32_t FILE_SizeOfHeader = 3;
};
