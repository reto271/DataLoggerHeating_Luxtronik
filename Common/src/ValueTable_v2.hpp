#pragma once

#include "Common/src/ValueTableCommon.hpp"

class ValueTable_v2 : public ValueTableCommon
{
public:

    ValueTable_v2();

    virtual ~ValueTable_v2();

    uint32_t getFileVersion() const override;
    uint32_t getSizeOfHeader() const override;

private:
    void initValueTable() override;

    const uint32_t FILE_Version = 2;
    const uint32_t FILE_SizeOfHeader = 3;
};
