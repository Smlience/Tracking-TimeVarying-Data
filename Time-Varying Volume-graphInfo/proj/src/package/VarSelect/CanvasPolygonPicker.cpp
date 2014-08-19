#include <QPainter>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QApplication>
#include <QVBoxLayout>
#include <QColorDialog>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include "CanvasPolygonPicker.h"
#include "util/util.h"
#include <iostream>
using namespace std;

class Polygon : public QPolygon
{
public:
	Polygon(const QPolygon & polygon)
		: QPolygon(polygon), c(200, 200, 200, 50)
	{};

	const QColor& color()
	{
		return c;
	}
	void setColor(const QColor& c_)
	{
		c = c_;
	};

	void setOpacity(int opacity)
	{
		c.setAlpha(opacity);
	}

private:
	QColor c;
};

CanvasPolygonPicker::CanvasPolygonPicker(QWidget *parent):
    CanvasPicker(parent), m_borderColor(128, 128, 128),
	m_borderColorSelectedPoly(187, 68, 86), m_borderColorSelectedVertex(223, 128, 32),
	m_selected(NULL), m_drawing(NULL), m_editingVertexIdx(-1), m_first(-1, -1)
{
	//addRect(QPoint(10, 10), QPoint(190, 670));
	setFocusPolicy(Qt::StrongFocus);
}

CanvasPolygonPicker::~CanvasPolygonPicker()
{
	//SAFE_DELETE(m_scene);
}


void CanvasPolygonPicker::set_default_tf(int num_of_features){
	clear();
	QSize tsize = size();
	//cout << tsize.height() <<" "<<tsize.width() <<endl;
	for(int i = 1;i < num_of_features;i++){
		QPoint p1 = QPoint(0,i*tsize.height()/num_of_features-5);
		QPoint p2 = QPoint(tsize.width(),i*tsize.height()/num_of_features+2);
		addRect(p1,p2)->setColor(QColor(i*6,0,0));
	}
}

void CanvasPolygonPicker::selectAt(const QPoint & pos)
{
	Q_ASSERT(m_vPoly.size() < 0xfffe);
	QImage img(size(), QImage::Format_ARGB32);
	img.fill(Qt::transparent);
	QPainter p(&img);
	p.setCompositionMode(QPainter::CompositionMode_Source);
	for (int i = 0; i < m_vPoly.size(); ++i)
	{
		drawPolygonForSelection(&p, i, m_vPoly[i]);
	}
	unsigned int s = img.pixel(pos);
	s &= 0x00ffffff;
	int pidx = (s >> 8) - 1;
	m_editingVertexIdx = (s & 0xff) - 1;
// 	printf("s=%x (s >> 8)=%x (s & 0xff)=%x\n", s, (s >> 8), (s & 0xff));
// 	fflush(stdout);
	if (pidx >= 0)
	{
		m_selected = m_vPoly.at(pidx);
		emit selectedPolygonOpacityChanged(m_selected->color().alpha());
	}
	else
	{
		m_selected = NULL;
	}
}

void CanvasPolygonPicker::mousePressEvent( QMouseEvent* e )
{
	if (e->button() == Qt::LeftButton)
	{
		if (e->modifiers().testFlag(Qt::ShiftModifier)) // Create new rectangle
		{
			m_selected = NULL;
			m_first = e->pos();
		}
		else if (e->modifiers().testFlag(Qt::ControlModifier))
		{
			m_selected = NULL;
			if (m_drawingPolygon.size() > 2 && (e->pos() - m_drawingPolygon.first()).manhattanLength() <= 3)
			{
				m_selected = addPolygon(m_drawingPolygon);
				m_drawingPolygon.clear();
				emit canvasChanged();
			}
			else
			{
				m_drawingPolygon.push_back(e->pos());
			}
		}
		else // Select control point or polygon
		{
			selectAt(e->pos());
		}
		update();
	}
	m_last = e->pos();
}

void CanvasPolygonPicker::mouseMoveEvent( QMouseEvent* e )
{
	if (e->buttons().testFlag(Qt::LeftButton))
	{
		if (e->modifiers().testFlag(Qt::ShiftModifier) && m_first.x() >= 0)
		{
			m_current = e->pos();
			if (m_drawing == NULL)
			{
				m_drawing = addRect(m_first, m_current);
			}
			else
			{
				m_drawing->setPoint(1, m_current.x(), m_first.y());
				m_drawing->setPoint(2, m_current.x(), m_current.y());
				m_drawing->setPoint(3, m_first.x(), m_current.y());
			}
			emit canvasChanged();
		}
		
		if (m_selected != NULL)
		{
			if (m_editingVertexIdx >= 0)
			{
				m_selected->setPoint(m_editingVertexIdx, e->pos());
			}
			else
			{
				movePolygon(m_selected, m_last, e->pos());
			}
			emit canvasChanged();
		}

		update();
	}
	m_last = e->pos();
}

void CanvasPolygonPicker::mouseReleaseEvent( QMouseEvent* e )
{
	if (e->button() == Qt::LeftButton)
	{
		if (e->modifiers().testFlag(Qt::ShiftModifier) && m_first.x() >= 0)
		{
			m_current = e->pos();
			if (m_drawing != NULL)
			{
				m_drawing->setPoint(1, m_current.x(), m_first.y());
				m_drawing->setPoint(2, m_current.x(), m_current.y());
				m_drawing->setPoint(3, m_first.x(), m_current.y());
				m_selected = m_drawing;
				m_drawing = NULL;
			}
			m_first.setX(-1);
			update();
		}
		if (m_editingVertexIdx >= 0)
		{
			m_editingVertexIdx = -1;
		}
	}
	m_last = e->pos();
}

void CanvasPolygonPicker::mouseDoubleClickEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		selectAt(e->pos());
		if (m_selected != NULL)
		{
			selectPolygonColor(m_selected);
			emit canvasChanged();
		}
	}
}

const QImage CanvasPolygonPicker::result()
{
	QImage r(size(), QImage::Format_ARGB32);
	r.fill(Qt::transparent);
	QPainter p(&r);
	for (int i = 0; i < m_vPoly.size(); ++i)
	{
		drawPolygon(&p, m_vPoly[i]);
	}
	return r;
}

void CanvasPolygonPicker::paintEvent(QPaintEvent* e)
{
	QPainter p(this);
	for (QVector<Polygon*>::Iterator it = m_vPoly.begin(); it != m_vPoly.end(); ++it)
	{
		drawPolygonForInteraction(&p, *it);
	}

	if (m_drawingPolygon.size() > 0)
	{
		p.setPen(m_borderColorSelectedPoly);
		p.setBrush(Qt::lightGray);
		for (int i = 1; i < m_drawingPolygon.size(); ++i)
		{
			p.drawLine(m_drawingPolygon[i - 1], m_drawingPolygon[i]);
			p.drawEllipse(m_drawingPolygon[i], 3, 3);
		}
		p.setPen(QPen(m_borderColorSelectedVertex, 2));
		p.drawEllipse(m_drawingPolygon[0], 4, 4);
	}
}

Polygon* CanvasPolygonPicker::addPolygon( QVector<QPoint>& v )
{
	Polygon* r = NULL;
	if (v.size() < 3)
		return r;
	QPolygon polygon;
	for (QVector<QPoint>::iterator it = v.begin(); it != v.end(); ++it)
	{
		polygon << *it;
	}
	r = new Polygon(polygon);
	m_vPoly.push_back(r);
	return r;
}

Polygon* CanvasPolygonPicker::addRect(const QPoint &p1, const QPoint &p2)
{
	QPolygon polygon;
	polygon << p1 << QPoint(p2.x(), p1.y()) << p2 << QPoint(p1.x(), p2.y());
	Polygon *polygonItem = new Polygon(polygon);
	m_vPoly.push_back(polygonItem);
	return polygonItem;
}

void CanvasPolygonPicker::drawPolygon(QPainter *const p, Polygon *polygon)
{
	p->setPen(polygon->color());
	p->setBrush(polygon->color());
	p->drawPolygon(*polygon);
}

void CanvasPolygonPicker::drawPolygonForInteraction( QPainter *const p , Polygon *polygon)
{
	if (m_selected == polygon)
	{
		p->setPen(m_borderColorSelectedPoly);
		QColor c = polygon->color();
		c.setAlpha(c.alpha() * (200 - 50 + 1) / 256 + 50); // clamp opacity to [50, 200], for interaction purpose
		p->setBrush(c);
		p->drawPolygon(*polygon);

		// Draw control points
		for (QVector<QPoint>::iterator it = polygon->begin(); it != polygon->end(); ++it)
		{
			p->drawEllipse(*it, 3, 3);
		}
		if (m_editingVertexIdx >= 0)
		{
			p->setPen(QPen(m_borderColorSelectedVertex, 2));
			p->drawEllipse(polygon->at(m_editingVertexIdx), 4, 4);
		}
	}
	else
	{
		p->setPen(m_borderColor);
		p->setBrush(polygon->color());
		p->drawPolygon(*polygon);

		// Draw control points
		for (QVector<QPoint>::iterator it = polygon->begin(); it != polygon->end(); ++it)
		{
			p->drawEllipse(*it, 3, 3);
		}
	}
}

void CanvasPolygonPicker::drawPolygonForSelection(QPainter *const p, unsigned int idx, Polygon *polygon)
{
	Q_ASSERT(polygon->size() < 0xfe);
	QRgb c_ = (idx + 1) << 8 | 0xff000000;
	QColor c(c_);
	p->setPen(c);
	p->setBrush(c);
	p->drawPolygon(*polygon);
	// Draw control points
	for (int i = 0; i < polygon->size(); ++i)
	{
		c.setRgba(c_ | (i + 1));
		p->setPen(c);
		p->setBrush(c);
		p->drawEllipse(polygon->at(i), 3, 3);
	}
}

void CanvasPolygonPicker::movePolygon(Polygon* p, const QPoint &from, const QPoint &to)
{
	QPoint diff = to - from;
	for (QVector<QPoint>::iterator it = p->begin(); it != p->end(); ++it)
	{
		*it += diff;
	}
}

void CanvasPolygonPicker::selectPolygonColor(Polygon* p)
{
	QColor c = QColorDialog::getColor(p->color(), this, "SelectColor", QColorDialog::ShowAlphaChannel);
	if (c.isValid())
	{
		p->setColor(c);
		update();
		emit canvasChanged();
		emit selectedPolygonOpacityChanged(c.alpha());
	}
}

QWidget* CanvasPolygonPicker::createToolbar( QWidget* parent )
{
	QWidget *toolbar = new QWidget(parent);;
	QPushButton *btnColor = new QPushButton(toolbar);
	QIcon icon_color(QStringLiteral(":/MainWin/color.png"));
	btnColor->setIcon(icon_color);
	btnColor->setIconSize(QSize(16, 16));
	btnColor->setFixedSize(20, 20);
	btnColor->setToolTip("Select color for selected polygon");
	connect(btnColor, SIGNAL(clicked()), this, SLOT(selectSelectedPolygonColor()));

	QPushButton *	btnClear = new QPushButton(toolbar);
	QIcon icon_clear(QStringLiteral(":/MainWin/clear.png"));
	btnClear->setIcon(icon_clear);
	btnClear->setIconSize(QSize(16, 16));
	btnClear->setFixedSize(20, 20);
	btnClear->setToolTip("Clear");
	connect(btnClear, SIGNAL(clicked()), this, SLOT(clear()));

	QLabel* label = new QLabel("0");
	label->setFixedSize(20, 20);

	QSlider* slider = new QSlider(/*Qt::Horizontal, */toolbar);
	slider->setRange(0, 255);
	slider->setFixedWidth(20);
	slider->setMinimumHeight(256);
	//slider->setMaximumWidth(256);
	//slider->setFixedHeight(20);
	connect(slider, SIGNAL(valueChanged(int) ), this, SLOT(setSelectedPolygonOpacity(int)));
	connect(slider, SIGNAL(valueChanged(int) ), label, SLOT(setNum(int)));
	connect(this, SIGNAL(selectedPolygonOpacityChanged(int)), slider, SLOT(setValue(int)));

	QVBoxLayout *layout = new QVBoxLayout(toolbar);
	layout->setSpacing(2);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(btnColor);
	layout->addWidget(btnClear);
	layout->addWidget(label);
	layout->addWidget(slider);
	//toolbar->resize(20, hLayout->count() * (20 + hLayout->spacing()) - hLayout->spacing());
	return toolbar;
}

void CanvasPolygonPicker::selectSelectedPolygonColor()
{
	if (m_selected != NULL)
	{
		selectPolygonColor(m_selected);
	}
}

void CanvasPolygonPicker::setSelectedPolygonOpacity( int v )
{
	if (m_selected != NULL)
	{
		QColor c = m_selected->color();
		c.setAlpha(v);
		m_selected->setColor(c);
		update();
		emit canvasChanged();
	}
}

void CanvasPolygonPicker::keyPressEvent( QKeyEvent *e )
{
	if (e->key() == Qt::Key_Delete && m_selected != NULL)
	{
		m_vPoly.remove(m_vPoly.indexOf(m_selected));
		delete m_selected;
		m_selected = NULL;
		update();
	}
}

void CanvasPolygonPicker::keyReleaseEvent( QKeyEvent *e )
{
	if (e->key() == Qt::Key_Control)
	{
		if (m_drawingPolygon.size() > 0)
		{
			m_drawingPolygon.clear();
			update();
		}
	}
}

void CanvasPolygonPicker::clear()
{
	for(QVector<Polygon*>::iterator it = m_vPoly.begin(); it != m_vPoly.end(); ++it)
	{
		delete *it;
	}
	m_vPoly.clear();
	update();
	emit canvasChanged();
}
