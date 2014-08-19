#ifndef SCATTERPLOTCALC_H
#define SCATTERPLOTCALC_H

#include <QObject>
#include <QVector>
#include "dm/Volume.h"

class ScatterPlotCalc : public QObject
{
	Q_OBJECT

public:
	ScatterPlotCalc();
	~ScatterPlotCalc();

signals:
	void samples(const QVector<QPointF> &samples);
	void dataChanged(const unsigned char *v1, std::pair<int,int> v1Range, const unsigned char*v2, std::pair<int,int> v2Range, Vector3i dim );

public slots:
	void setVolume(const Volume* v);

private:
	QVector<QPointF> m_samples;
	unsigned char* m_data;
};

#endif // SCATTERPLOTCALC_H