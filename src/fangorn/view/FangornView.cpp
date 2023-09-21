#include "fangorn/view/fangornview.hpp"

FangornView::FangornView(QWidget *parent)
 : QWidget(parent) { }


bool FangornView::hasResource(unsigned int resourceId) const
{
    return resources.find(resourceId) != resources.end();
}


QWidget *FangornView::getResource(unsigned int resourceId)
{
    FANGORN_GENERIC_ASSERT(hasResource(resourceId), "FangornView does not have resource with id %d!", resourceId);

    return resources[resourceId];
}


void FangornView::addResource(unsigned int resourceId, QWidget *resource)
{
    FANGORN_GENERIC_ASSERT(!hasResource(resourceId), "Duplicate resource ID %d!", resourceId);

    resources.insert({ resourceId, resource });
}
