#pragma once

#include "fangorn/fangorncommon.hpp"
#include <QWidget>

class FangornView : public QWidget {
    public:
    DEF_SMART_PTR_TYPES(FangornView);

    FangornView(QWidget *parent);

    bool hasResource(unsigned int resourceId) const;
    QWidget *getResource(unsigned int resourceId);

    protected:
    void addResource(unsigned int resourceId, QWidget *resource);

    private:
    std::map<unsigned int, QWidget*> resources;
};
