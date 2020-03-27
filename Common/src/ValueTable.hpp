#pragma once

#include "Common/src/ValueTableCommon.hpp"

class ValueTable : public ValueTableCommon
{
public:

    ValueTable();

    virtual ~ValueTable();
    void init() override;

    uint32_t getFileVersion() const override;
    uint32_t getSizeOfHeader() const override;

private:
    void initValueTable() override;

    const uint32_t FILE_Version = 3;
    const uint32_t FILE_SizeOfHeader = 3;
};
