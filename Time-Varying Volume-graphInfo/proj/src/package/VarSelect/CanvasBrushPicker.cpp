#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QButtonGroup>
#include <QPushButton>
#include <QVBoxLayout>
#include <QColorDialog>
#include "CanvasBrushPicker.h"

CanvasBrushPicker::CanvasBrushPicker(QWidget *parent):
    CanvasPicker( parent ),
	m_brushSize(5),
	m_pen(Qt::red, 15),
	m_canvas(256, 256, QImage::Format_ARGB32),
	m_brushType(CanvasBrushPicker::BrushTypeFill)
{
	m_pen.setColor(QColor(255, 0, 0, 128));
 	m_canvas.fill(Qt::transparent);
}

void CanvasBrushPicker::mousePressEvent(QMouseEvent *e)
{
	if(e->buttons() & Qt::LeftButton)
	{
// 		QImage* tmp_canvas= new QImage(m_canvas.size(), QImage::Format_ARGB32);
// 		tmp_canvas->fill(Qt::transparent);
		//if (m_canvas != NULL)
		{
			QPainter p(&m_canvas);
			switch(m_brushType)
			{
			case BrushTypeFill:
				p.setPen(m_pen);
				p.drawPoint(e->pos());
				break;
			case BrushTypeErase:
				p.setPen(m_pen);
				p.setCompositionMode(QPainter::CompositionMode_Clear);
				p.drawPoint(e->pos());
				break;
			default:
				break;
			}
			//painter.drawPixmap(e->pos().x()-50,e->pos().y()-50,*p3); //鼠标按下时产生一个点
			update();
			emit canvasChanged();
		}
		lastPos = e->pos();
//		delete tmp_canvas;
	}
}

void CanvasBrushPicker::mouseMoveEvent( QMouseEvent* e )
{
	if(e->buttons() & Qt::LeftButton){
		QPainter p(&m_canvas);
		p.setRenderHint(QPainter::Antialiasing);
		p.setPen(m_pen);

		switch(m_brushType)
		{
		case BrushTypeFill:
			p.drawLine(lastPos, e->pos());
			break;
		case BrushTypeErase:
			p.setCompositionMode(QPainter::CompositionMode_Clear);
			p.drawLine(lastPos, e->pos());
			break;
		default:
			break;
		}

		update();
		emit canvasChanged();
		lastPos = e->pos(); //更新坐标
	}
}

void CanvasBrushPicker::mouseReleaseEvent( QMouseEvent* e )
{

}

void CanvasBrushPicker::resizeEvent( QResizeEvent* e )
{
	//if (e->size() != this->size())
	{
// 		QImage* img = m_canvas;
// 		m_canvas = new QImage(e->size(), QImage::Format_ARGB32);
// 		m_canvas->fill(Qt::transparent);
// 		delete img;
		m_canvas = m_canvas.scaled(e->size());
	}
}

void CanvasBrushPicker::paintEvent(QPaintEvent *e)
{
	QPainter p(this);
	p.setCompositionMode(QPainter::CompositionMode_Multiply);
	p.drawImage(rect(), m_canvas);
}

void CanvasBrushPicker::setPen( const QColor &c, int w )
{
	m_pen.setColor(c);
	m_pen.setWidth(w);
}

QRgb CanvasBrushPicker::colorAt(int x, int y)
{
	return m_canvas.pixel(x, y);
}

const QImage CanvasBrushPicker::result()
{
	return m_canvas;
}

QColor CanvasBrushPicker::getPenColor() const
{
	return m_pen.color();
}

void CanvasBrushPicker::setPenColor(const QColor &c)
{
	m_pen.setColor(c);
}

void CanvasBrushPicker::setBrushType(BrushType t)
{
	m_brushType = t;
}

void CanvasBrushPicker::clearCanvas()
{
	m_canvas.fill(Qt::transparent);
	emit canvasChanged();
	update();
}

void CanvasBrushPicker::selectBrushColor()
{
	QColor c = QColorDialog::getColor(m_pen.color(), this, "Select brush color");
	setPenColor(c);
}

void CanvasBrushPicker::changeBrushToOpactiyIncrease()
{
	setBrushType(CanvasBrushPicker::BrushTypeOpacityIncrease);
}

void CanvasBrushPicker::changeBrushToOpactiyDecrease()
{
	setBrushType(CanvasBrushPicker::BrushTypeOpacityDecrease);
}

void CanvasBrushPicker::changeBrushToErase()
{
	setBrushType(CanvasBrushPicker::BrushTypeErase);
}

void CanvasBrushPicker::changeBrushToFill()
{
	setBrushType(CanvasBrushPicker::BrushTypeFill);
}
QWidget* CanvasBrushPicker::createToolbar( QWidget* parent )
{
	//////////////////////////////////////////////////////////////////////////
	// 交互组件
	QWidget *m_toolbar;
	QPushButton *btnColor;
	QPushButton *btnFill;
	QPushButton *btnOpacityPlus;
	QPushButton *btnOpacitySub;
	QPushButton *btnErase;
	QPushButton *btnClear;
	//////////////////////////////////////////////////////////////////////////
	// 交互组件
	m_toolbar = new QWidget(parent);

	btnColor = new QPushButton(m_toolbar);
	QIcon icon_color(QStringLiteral(":/MainWin/color.png"));
	btnColor->setIcon(icon_color);
	btnColor->setIconSize(QSize(16, 16));
	btnColor->setFixedSize(20, 20);
	btnColor->setToolTip("Select Brush Color");
	connect(btnColor, SIGNAL(clicked()), this, SLOT(selectBrushColor()));

	btnFill = new QPushButton(m_toolbar);
	QIcon icon_brush(QStringLiteral(":/MainWin/fill.png"));
	btnFill->setIcon(icon_brush);
	btnFill->setIconSize(QSize(16, 16));
	btnFill->setFixedSize(20, 20);
	btnFill->setToolTip("Brush");
	btnFill->setCheckable(true);
	btnFill->setChecked(true);
	connect(btnFill, SIGNAL(clicked()), this, SLOT(changeBrushToFill()));

	btnOpacityPlus = new QPushButton(m_toolbar);
	QIcon icon_opacityplus(QStringLiteral(":/MainWin/opacity_inc.png"));
	btnOpacityPlus->setIcon(icon_opacityplus);
	btnOpacityPlus->setIconSize(QSize(16, 16));
	btnOpacityPlus->setFixedSize(20, 20);
	btnOpacityPlus->setToolTip("Increase Opacity");
	btnOpacityPlus->setCheckable(true);
	connect(btnOpacityPlus, SIGNAL(clicked()), this, SLOT(changeBrushToOpactiyIncrease()));

	btnOpacitySub = new QPushButton(m_toolbar);
	QIcon icon_opacitysub(QStringLiteral(":/MainWin/opacity_dec.png"));
	btnOpacitySub->setIcon(icon_opacitysub);
	btnOpacitySub->setIconSize(QSize(16, 16));
	btnOpacitySub->setFixedSize(20, 20);
	btnOpacitySub->setToolTip("Decrease Opacity");
	btnOpacitySub->setCheckable(true);
	connect(btnOpacitySub, SIGNAL(clicked()), this, SLOT(changeBrushToOpactiyDecrease()));

	btnErase = new QPushButton(m_toolbar);
	QIcon icon_erase(QStringLiteral(":/MainWin/eraser.png"));
	btnErase->setIcon(icon_erase);
	btnErase->setIconSize(QSize(16, 16));
	btnErase->setFixedSize(20, 20);
	btnErase->setToolTip("Erase");
	btnErase->setCheckable(true);
	connect(btnErase, SIGNAL(clicked()), this, SLOT(changeBrushToErase()));

	btnClear = new QPushButton(m_toolbar);
	QIcon icon_clear(QStringLiteral(":/MainWin/clear.png"));
	btnClear->setIcon(icon_clear);
	btnClear->setIconSize(QSize(16, 16));
	btnClear->setFixedSize(20, 20);
	btnClear->setToolTip("Clear");
	connect(btnClear, SIGNAL(clicked()), this, SLOT(clearCanvas()));

	QButtonGroup* btnGroup = new QButtonGroup(m_toolbar);
	btnGroup->addButton(btnFill);
	btnGroup->addButton(btnOpacityPlus);
	btnGroup->addButton(btnOpacitySub);
	btnGroup->addButton(btnErase);

	QVBoxLayout *vLayout = new QVBoxLayout(m_toolbar);
	vLayout->setSpacing(2);
	vLayout->setContentsMargins(0, 0, 0, 0);
	vLayout->addWidget(btnColor);
	vLayout->addWidget(btnFill);
	vLayout->addWidget(btnOpacityPlus);
	vLayout->addWidget(btnOpacitySub);
	vLayout->addWidget(btnErase);
	vLayout->addWidget(btnClear);
	m_toolbar->resize(20, vLayout->count() * (20 + vLayout->spacing()) - vLayout->spacing());
	//////////////////////////////////////////////////////////////////////////
	return m_toolbar;
}
