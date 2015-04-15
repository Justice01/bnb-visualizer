#ifndef PROCESSOR
#define PROCESSOR
#include <qvector.h>
struct processor
{
    QVector<double> activity;
    QVector<double> sending;
    QVector<double> receiving;
};

#endif // PROCESSOR

