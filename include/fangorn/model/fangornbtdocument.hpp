#pragma once

#include "fangorn/fangornbase.hpp"
#include "fangorn/model/fangornbtnode.hpp"
#include "fangorn/model/fangornforest.hpp"

class FangornBtDocument : public FangornForestItem {
    public:
    DEF_SHARED_PTR_TYPES(FangornBtDocument);
    FangornBtDocument(const std::string& name);
    FangornBtDocument(const QDomDocument& document);

    private:
    std::list<FangornBTNode::SharedPtr> trees;
};