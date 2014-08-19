
#include "package/timewidget/timeSelect.h"
#include <QVBoxLayout>


TimeSelectWidget::TimeSelectWidget(QWidget *parent)
{
	setupUi();
	connect(m_sliderTime, SIGNAL(valueChanged(int) ), this, SLOT(setCurTimestep(int)));
}

TimeSelectWidget::~TimeSelectWidget()
{}

void TimeSelectWidget::setParams( int timestepsNum)
{
	//m_bufferDisplay->setParameters(volumeIDs,timestepsNum);
//	m_sliderTime->setRange(0,timestepsNum-1);//the sum of curtimestep 
	m_sliderTime->setRange(0,timestepsNum-1);//the sum of curtimestep 
	m_sliderTime->setTickInterval(1);
	m_sliderTime->setTickPosition(QSlider::TicksBelow);
	//std::cout<<"tvslider"<<timestepsNum<<endl;
}

void TimeSelectWidget::setDataRange( float minData, float maxData)
{

}


size_t TimeSelectWidget::getCurTimestep()
{
	return m_sliderTime->value();
}
size_t TimeSelectWidget::getCurTimeWindowWide()
{
	return m_sliderTime->value();
}
void TimeSelectWidget::setCurTimestep(int timestep)
{
	m_timeSliderLabel->setText(QString("time step:%1").arg(timestep));
}

void TimeSelectWidget::setupUi()
{
	//QWidget *time = new QWidget(parent);
	this->setFixedHeight(50);
	m_sliderTime = new QSlider(this);
	m_sliderTime->setRange(1, 45);
	m_sliderTime->setFixedWidth(256);
	m_sliderTime->setMinimumHeight(20);
	m_sliderTime->setOrientation(Qt::Horizontal);
	//sliderTime->setRange(0,timestepsNum-1);//the sum of curtimestep 
	m_sliderTime->setTickInterval(1);
	m_sliderTime->setTickPosition(QSlider::TicksBelow);
	//connect(sliderTime, SIGNAL(valueChanged(int) ), this, SLOT(timeChanged(int)));

	m_timeSliderLabel = new QLabel(this);
	m_timeSliderLabel->setText( QString( "time slider:"));
	m_timeSliderLabel->setFixedHeight(10);

	QVBoxLayout *layout = new QVBoxLayout(this);
	//layout->setSpacing(2);
	layout->addWidget(m_timeSliderLabel);
	layout->addWidget(m_sliderTime);
	
}