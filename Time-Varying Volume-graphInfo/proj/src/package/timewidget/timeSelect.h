#ifndef TIMESELECT_H
#define TIMESELECT_H

#include <QWidget>
#include <QPushButton>
#include "util\Vector2.h"
#include "util\Vector3.h"
#include <QSlider>
#include <QLabel>
class TimeSelectWidget : public QWidget
{
	Q_OBJECT

public:
	TimeSelectWidget(QWidget *parent = 0);
	~TimeSelectWidget();

	size_t getCurTimestep();
	
	size_t getCurTimeWindowWide();

	void setupUi();
signals:
	void timeChanged(int time);

	public slots:
		void setParams( int timestepsNum);
		void setDataRange( float minData, float maxData);
		void setCurTimestep(int timestep);

protected:
	 

public:
	 QSlider* m_sliderTime;
	 QLabel*  m_timeSliderLabel;
};

#endif // TIMESELECT_H
