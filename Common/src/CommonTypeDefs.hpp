#pragma once

#define PointerDefinition(ClassName) \
    typedef std::shared_ptr<ClassName> ClassName ## _SPtr
