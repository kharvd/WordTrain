#include "abstractdictionary.h"

AbstractDictionary::AbstractDictionary(QObject *parent) :
    QObject(parent), m_IsError(false)
{
}
