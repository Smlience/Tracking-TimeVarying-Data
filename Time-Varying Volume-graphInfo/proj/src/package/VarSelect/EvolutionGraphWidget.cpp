#include "EvolutionGraphWidget.h"
#include "tf/1dtf/TF1D.h"
#include <sstream>
#include <memory>
#include <QToolTip>
#include <fstream>
#include <algorithm>
#include <QColor>
#include <QColorDialog>

EvolutionGraphWidget::EvolutionGraphWidget(QWidget* parent)
	: QWidget(parent)
	, m_iMaximumIntensity(255)
	, m_bIsNoColor(false)
	, m_xRange(0.f, 1.f), m_yRange(0.f, 1.f)
	, m_xAxisText(tr("time")), m_yAxisText(tr("feature"))
	,m_clusterNum(5)
	,m_timeWindowWide(6)
	,m_pathNum(5)//attention!!
	,m_mouseMoveSelected(-1)
	,m_sequenceSelected(-1)
{
	m_iPadding = 12;
	m_iBottomSpace = 42;
	m_iArrowLength = 10;
	m_iArrowWidth = 3;
	m_iPointSize = 10;
	m_pSelectedKey = 0;
	m_iMinCellSize = 8;
	m_bIsDragging = false;
	m_iDragLine = -1;
	m_fDragLineAlphaLeft = -1.f;
	m_fDragLineAlphaRight = -1.f;
	m_bIsDrawHist = false;

	m_timeWindowWide=43;
	m_timeNum=45;
	m_curTimeStep=0;
	m_clusterNumSelected=0;
	m_keyColletion = new TFMappingKeyCollection();
	m_pTransfunc = new TF1D();
	//m_tf = new ClusteredColorTF();


	m_startPoint=Vector2i(2500,2500);
	m_endPoint=Vector2i(2500,2500);

	/*m_pHistogramPainter = new HistogramPainter(this, m_xRange, m_yRange, m_iPadding, m_iPadding, m_iBottomSpace, m_iPadding, m_iArrowLength);
	m_pHistogramPainter->setHisStyle(0);
	m_pHistogramPainter->installEventFilter(this);

	m_colorBar = new ColorBar(this);
	m_colorBar->installEventFilter(this);*/
	
	dataLoad=false;

	setObjectName("SequenceCanvas");
	setMouseTracking(true);
	setFocusPolicy(Qt::StrongFocus);
	//printf("%s\n" , topLevelWidget()->objectName().toLocal8Bit().data());

	
	setFocus();


}

EvolutionGraphWidget::~EvolutionGraphWidget()
{
	delete [] m_arryNumFeature;
	m_arryNumFeature = NULL;
}

QSize EvolutionGraphWidget::minimumSizeHint() const
{
	return QSize(300,100);
}

QSize EvolutionGraphWidget::sizeHint() const
{
	return QSize(1000, 100);
}

QSizePolicy EvolutionGraphWidget::sizePolicy() const
{
	return QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}

bool EvolutionGraphWidget::eventFilter( QObject *obj, QEvent *e )
{
	if (e->type() == QEvent::MouseMove)
	{
		mouseMoveEvent(static_cast<QMouseEvent*>(e));
	}

	// pass the event on to the parent class
	return QWidget::eventFilter(obj, e);
}

void EvolutionGraphWidget::resizeEvent( QResizeEvent* event )
{
	QWidget::resizeEvent(event);
	//m_pHistogramPainter->resize(width(), height());

	////////////////////////////////////////////////////////////////////////////
	//Vector2f sp = wtos(Vector2f(0.f, 0.f));
	//Vector2f ep = wtos(Vector2f(1.f, 1.f));
	//m_colorBar->move(int(sp.x), int(ep.y - 15));
	//m_colorBar->resize(ep.x - sp.x, 25);
	////////////////////////////////////////////////////////////////////////////

	m_gridSpacing = Vector2f(m_xRange.y, m_yRange.y);
	// refine gridSpacing_ as good as possible
	Vector2f cellSize = wtos(m_gridSpacing) - wtos(Vector2f(m_xRange.x, m_yRange.x));
	//计算格子数目
	m_gridSpacingNum.x = cellSize.x * (0.04);/* * (0.04)*/
	m_gridSpacingNum.y = cellSize.y * (0.04);
}


void EvolutionGraphWidget::paintEvent( QPaintEvent* event )
{

	if (!m_keyColletion && dataLoad)
		return;
	event->accept();
	QPainter paint(this);

	QMatrix m;
	m.translate(0.0, static_cast<float>(height())-1);
	m.scale(1.f, -1.f);
	paint.setMatrix(m);

	paint.setMatrixEnabled(true);
	paint.setRenderHint(QPainter::Antialiasing, false);
	paint.setPen(Qt::NoPen);
	paint.setBrush(QColor(40,40,40));  //0.156863, 0.156863, 0.156863
	paint.drawRect(0, 0, width() - 1, height() - 1);

	//////////////////////////////////////////////////////////////////////////
	// draw path
	paint.setPen(QColor(220, 220, 220));
	paint.setRenderHint(QPainter::Antialiasing, false);

	Vector2f pmin = Vector2f(m_xRange.x, m_yRange.x); //相对坐标的范围   m_xRange(0.f, 1.f), m_yRange(0.f, 1.f)
	Vector2f pmax = Vector2f(m_xRange.y, m_yRange.y);
	int xgridNum = m_gridSpacingNum.x;
	int ygridNum = m_gridSpacingNum.y;
	m_gridSpacing.x = (pmax.x - pmin.x) / (m_timeNum+1);  //xgridNum
	m_gridSpacing.y = (pmax.y - pmin.y) / ygridNum;

	float fx,fy;
	int h,hy;
	float m_xTimeSpacing=(pmax.x - pmin.x)/float(m_timeNum+1);


	//////////////////////////////////////////////////////////////////////////
	//draw grid
	// draw grid
	paint.setPen(QColor(70, 70, 70));
	paint.setRenderHint(QPainter::Antialiasing, false);

	int num = 0;
	for (float f=pmin.x; f<pmax.x-m_xTimeSpacing*0.5; f+=m_xTimeSpacing, ++num) 
	{
		Vector2f p = wtos(Vector2f(f, 0.f));
		Vector2f a = wtos(Vector2f(0.f, 0.f));
		Vector2f b = wtos(Vector2f(0.f, 1.f));   //width() - m_iPadding

		paint.drawLine(QPointF(p.x, a.y),QPointF(p.x, b.y));

		paint.scale(-1.f, 1.f);
		paint.rotate(180.f);
		QString strval = QString::number((int)(num));
		paint.setPen(Qt::gray);
		if (num % 2 == 0)
			paint.drawText(static_cast<int>(p.x ), static_cast<int>(-0.7 * (m_iBottomSpace)),  strval);
		else
			paint.drawText(static_cast<int>(p.x ), static_cast<int>(-0.45 * (m_iBottomSpace)),  strval);
		paint.setPen(QColor(70, 70, 70));
		paint.rotate(180.f);
		paint.scale(-1.f, 1.f);
	}
	paint.setPen(QColor(220, 220, 220));
	//for (float f=pmin.y; f<pmax.y+m_gridSpacing.y*0.5; f+=m_gridSpacing.y) 
	//{
	//	Vector2f p = wtos(Vector2f(0.f, f));
	//	Vector2f a = wtos(Vector2f(0.f, 0.f));
	//	Vector2f b = wtos(Vector2f(1.f, 0.f));
	//	paint.drawLine(QPointF(a.x, p.y),
	//		QPointF(b.x, p.y));
	//}

	//float m_yTimeSpacing=(pmax.y - pmin.y)/float(m_arryNumFeature[]+1);

	//// 绘制路径  line
	//for (fy=m_yTimeSpacing,hy=0;hy<m_pathNum;++hy,fy+=m_yTimeSpacing)
	//{
	//
	//	//cout<<"cluster end:"<<*(pathInfo[hy].end()-1)<<endl;
	//	fx=pmin.x+m_xTimeSpacing*startID;// here is the difference
	//
	//	Vector2f y = wtos(Vector2f(1.f, fy));
	//	paint.setPen(QColor(113,189,232));

	//	paint.drawLine(QPointF(a.x, y.y), QPointF(a.y, y.y));

	//}
	//
	//cout<<"m_curTimeStep"<<m_curTimeStep<<endl;

	//////////////////////////////////////////////////////////////////////////
	//draw graph edge
	float m_yTimeSpacing=(pmax.y - pmin.y)/float(m_arryNumFeature[0]+1);
	fx=m_xTimeSpacing;
	for (int timeStep=0; timeStep<m_timeNum;fx+=m_xTimeSpacing,timeStep++)
	{
		
		fy=m_yTimeSpacing;
		Vector2f node1X = wtos(Vector2f(fx, 0.f));	
		Vector2f node2X = wtos(Vector2f(fx+m_xTimeSpacing, 0.f));	

		m_keyColletion=m_keyColletionFull[timeStep];
		int edgeSum=0;
		//std::cout<<"i:"<<timeStep<<"m_arryNumFeature:"<<int(m_arryNumFeature[timeStep+1])<<std::endl;
		//std::cout<<"i:"<<timeStep<<"m_keyColletion:"<<int(m_keyColletion->getNumKeys())<<std::endl;
		for (int yNum=0;yNum<m_arryNumFeature[timeStep+1];fy+=m_yTimeSpacing,yNum++)
		{
			Vector2f node1Y = wtos(Vector2f(1.f, fy));

			edgeSum=dag.graph[timeStep][yNum].edgeSum;
			struct edgenode *p;
		
			p=dag.graph[timeStep][yNum].first;
			while (p!=NULL)
			{
				Vector2i nodeIndex=p->no;
				Vector2f node2Y = wtos(Vector2f(0.0, m_yTimeSpacing+ nodeIndex.y*m_yTimeSpacing));	
								
				paint.setPen(QColor(70, 70, 70));
				
				paint.drawLine(QPointF(node1X.x, node1Y.y), QPointF(node2X.x, node2Y.y));

				p=p->next;
			}			
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//draw new add edge
	//m_newAddEdge


	//////////////////////////////////////////////////////////////////////////
	//draw path
	// 绘制路径  line
	fx=m_xTimeSpacing;
	fy=m_yTimeSpacing;
	for (int i=0;i<dag.m_pathInfo.size();i++)
	{
		vector<Vector2i> onePahth=dag.m_pathInfo[i];

		for (int j=0;j<onePahth.size()-1;j++)// 一条路径的长度
		{
			float x1=  fx + m_xTimeSpacing * onePahth[j].x; 
			float x2=  fx + m_xTimeSpacing * onePahth[j+1].x;

			float y1= fy + m_yTimeSpacing * onePahth[j].y; 
			float y2= fy + m_yTimeSpacing * onePahth[j+1].y; 

			Vector2f node1 = Vector2f( x1 ,y1);
			Vector2f node2 = Vector2f( x2 ,y2);

			node1 = wtos( node1 ) ;
			node2 = wtos( node2 );
			paint.setPen(QColor(220,220,220));

			paint.drawLine(QPointF(node1.x, node1.y), QPointF(node2.x, node2.y));

		}
	}
	


	//////////////////////////////////////////////////////////////////////////
	//draw marker
	fx=m_xTimeSpacing;
	for (int timeStep=0; timeStep<m_timeNum;fx+=m_xTimeSpacing,timeStep++)
	{
		
		float m_yTimeSpacing=(pmax.y - pmin.y)/float(m_arryNumFeature[0]+1);
		fy=m_yTimeSpacing;
		Vector2f p = wtos(Vector2f(fx, 0.f));	

		m_keyColletion=m_keyColletionFull[timeStep];

		//std::cout<<"i:"<<timeStep<<"m_arryNumFeature:"<<int(m_arryNumFeature[timeStep+1])<<std::endl;
		//std::cout<<"i:"<<timeStep<<"m_keyColletion:"<<int(m_keyColletion->getNumKeys())<<std::endl;
		for (int yNum=0;yNum<m_arryNumFeature[timeStep+1];fy+=m_yTimeSpacing,yNum++)
		{
			TFMappingKey *key = m_keyColletion->getKey(yNum);			

			int props;
			props = MARKER_NORMAL;
			if (key == m_pSelectedKey)
			{
				//std::cout<<"i:"<<timeStep<<"yNum:"<<yNum<<"key:"<<key<<"m_pSelectedKey:"<<m_pSelectedKey<<::endl;
				props |= MARKER_SELECTED;
			}

			Vector2f y = wtos(Vector2f(1.f, fy));
			drawMarker(paint, key->getColor(),Vector2f(p.x, y.y),props);    //  key->getColor()
		}
		

	}


	//////////////////////////////////////////////////////////////////////////
	//draw  a  square  to show a feature's attribute
	paint.setPen(Qt::gray);
	paint.setBrush(QColor(40,40,40));  //0.156863, 0.156863, 0.156863
	
	Vector2f bottom=wtos(Vector2f(0.7f, 0.8f));
	Vector2f top=wtos(Vector2f(1.f, 1.f));	

	paint.drawRect(bottom.x, bottom.y, (width() - m_iPadding)*0.27, (height() - m_iPadding)*0.17);  //xy wh
	 
	//paint.scale(-1.f, 1.f);
	//paint.rotate(180.f);
	
	QString m_FeatureNo;
	QString m_CentroidText;
	QString m_VolumeText;
	QString m_MassText;

	vector <QString> attribute;
	m_FeatureNo.sprintf("%s %d %d ","Feature index: ",m_curTimeStep,m_clusterNumSelected);
	m_CentroidText.sprintf("%s %.0lf  %.0lf  %.0lf ","Centroid: ",	
		dag.m_FeatureInfo[m_curTimeStep][m_clusterNumSelected].centroid.x,
		dag.m_FeatureInfo[m_curTimeStep][m_clusterNumSelected].centroid.y,
		dag.m_FeatureInfo[m_curTimeStep][m_clusterNumSelected].centroid.z);//
	m_VolumeText.sprintf("%s %.0lf ","Volume: ",	dag.m_FeatureInfo[m_curTimeStep][m_clusterNumSelected].volume);//0);//
	m_MassText.sprintf("%s %.0lf","Mass: ",	dag.m_FeatureInfo[m_curTimeStep][m_clusterNumSelected].mass);//);//

	attribute.push_back(m_FeatureNo);
	attribute.push_back(m_CentroidText);
	attribute.push_back(m_VolumeText);
	attribute.push_back(m_MassText);

	paint.setPen(Qt::gray);
	paint.setBrush(Qt::NoBrush);
	float yWide=(height() - m_iPadding)*0.17 /6.0;
	paint.scale(-1.f, 1.f);
	paint.rotate(180.f);
	for (int i=0;i<4;i++)
	{
		//x is the same   // y   yBase+i*wide;
		float x=bottom.x;
		float y=bottom.y+float(i+1)*yWide;
		paint.drawText( x+1.5f * m_iPadding,-y,attribute[i]);
	}
	paint.rotate(180.f);
	paint.scale(-1.f, 1.f);
//	paint.drawText(static_cast<int>(width() - 5.5f * m_iPadding), static_cast<int>(-1 * (origin.y - 0.8 * m_iBottomSpace)), m_CentroidText);
//paint.drawText(static_cast<int>(1.6f * m_iPadding), static_cast<int>(-1 * (height() - 1.85f * m_iPadding)), m_yAxisText);


	//////////////////////////////////////////////////////////////////////////
	// draw x and y axes
	paint.setRenderHint(QPainter::Antialiasing, true);
	paint.setPen(Qt::gray);
	paint.setBrush(Qt::gray);

	// draw axes independently from visible range
	float oldx0_ = m_xRange[0];
	float oldx1_ = m_xRange[1];
	m_xRange[0] = 0.f;
	m_xRange[1] = 1.f;

	Vector2f origin = wtos(Vector2f(0.f, 0.f));
	origin.x = floor(origin.x) + 0.5f;
	origin.y = floor(origin.y) + 0.5f;

	paint.setRenderHint(QPainter::Antialiasing, true);

	paint.drawLine(QPointF(m_iPadding, origin.y),
		QPointF(width() - m_iPadding, origin.y));

	paint.drawLine(QPointF(origin.x, m_iBottomSpace),
		QPointF(origin.x, height() - m_iPadding));


	//////////////////////////////////////////////////////////////////////////
	// draw arrows
	QPointF arrow[3];
	arrow[0] = QPointF(origin.x, height() - m_iPadding);
	arrow[1] = QPointF(origin.x + m_iArrowWidth, height() - m_iPadding - m_iArrowLength);
	arrow[2] = QPointF(origin.x - m_iArrowWidth, height() - m_iPadding - m_iArrowLength);

	paint.drawConvexPolygon(arrow, 3);

	arrow[0] = QPointF(width() - m_iPadding, origin.y);
	arrow[1] = QPointF(width() - m_iPadding - m_iArrowLength, origin.y - m_iArrowWidth);
	arrow[2] = QPointF(width() - m_iPadding - m_iArrowLength, origin.y + m_iArrowWidth);

	paint.drawConvexPolygon(arrow, 3);

	paint.scale(-1.f, 1.f);
	paint.rotate(180.f);
	paint.drawText(static_cast<int>(width() - 5.5f * m_iPadding), static_cast<int>(-1 * (origin.y - 0.8 * m_iBottomSpace)), m_xAxisText);
	paint.drawText(static_cast<int>(1.6f * m_iPadding), static_cast<int>(-1 * (height() - 1.85f * m_iPadding)), m_yAxisText);

	paint.rotate(180.f);
	paint.scale(-1.f, 1.f);

	m_xRange[0] = oldx0_;
	m_xRange[1] = oldx1_;


	paint.setRenderHint(QPainter::Antialiasing, false);

	paint.setPen(Qt::lightGray);
	paint.setBrush(Qt::NoBrush);
	paint.drawRect(0, 0, width() - 1, height() - 1);

	paint.setMatrixEnabled(false);
	//m_MoveSelected.clear();
	//m_sequenceSelected=-1;
	//setupTransfunc();
}

void EvolutionGraphWidget::mousePressEvent(QMouseEvent* event)
{
	event->accept();

	if (event->button() == Qt::LeftButton)	
	{
		if (event->modifiers().testFlag(Qt::ControlModifier))// + Control  的话  连接线  边加入到图中 
		{

			m_dotAddSelected=hitDot(Vector2f(event->x(), event->y()));
			if ( m_dotAddSelected.x>=0 &&  m_dotAddSelected.y>=0  ) 
				if (m_startPoint==Vector2i(2500,2500))
				{
					m_startPoint=m_dotAddSelected;
				} 
				else
				{
					m_endPoint=m_dotAddSelected;
					dag.addEdge(m_startPoint,m_endPoint);

					m_newAddEdge=Edge(m_startPoint,m_endPoint);
					m_startPoint=Vector2i(2500,2500);
					m_endPoint=Vector2i(2500,2500);
				}
				//第一个  加入  start

				//第二个
		} 
		else
		{
			// see if a key was selected
			Vector2f sHit = Vector2f(event->x(), static_cast<float>(height()) - event->y());
			Vector2f hit = stow(sHit);


			m_dotSelected=hitDot(Vector2f(event->x(), event->y()));
			cout<<m_dotSelected<<endl;

			if (m_dotSelected.x>=0)
			{
				m_curTimeStep=m_dotSelected.x;
				setupTransfunc();
				emit sendTimeNumSelected(m_dotSelected.x+1);
				if (m_dotSelected.y>=0 && m_dotSelected.y<m_arryNumFeature[m_dotSelected.x+1])
				{
					m_clusterNumSelected=m_dotSelected.y;
					m_pSelectedKey=m_keyColletionFull[m_dotSelected.x]->getKey(m_dotSelected.y);
				}
			}
		}
		
		update();
	}



}
void EvolutionGraphWidget::mouseMoveEvent(QMouseEvent* event)
{
	/*event->accept();


	if (event->buttons().testFlag(Qt::LeftButton))
	{
	if (event->modifiers().testFlag(Qt::ControlModifier) && m_startPoint!=Vector2i(2500,2500))
	{
	m_current = event->pos();
	emit canvasChanged();
	}


	update();
	}*/
}



void EvolutionGraphWidget::mouseReleaseEvent(QMouseEvent* event)
{
	//event->accept();
	//if (event->button() == Qt::LeftButton) {
	//	m_bIsDragging = false;
	//	m_iDragLine = -1;
	//	m_fDragLineAlphaLeft = -1.f;
	//	m_fDragLineAlphaRight = -1.f;
	//	QToolTip::hideText();
	//	update();
	//	//		emit toggleInteractionMode(false);
	//}
}

void EvolutionGraphWidget::keyPressEvent(QKeyEvent* event)
{
	/*if (event->key() == Qt::Key_Shift  && underMouse() && hitLine(Vector2f(m_mousePos.x(), m_mousePos.y())) >= 0 && !m_bIsDragging) {
		setCursor(Qt::SizeVerCursor);
	}*/

	
}

void EvolutionGraphWidget::keyReleaseEvent(QKeyEvent* event)
{
	/*unsetCursor();
	if (event->key() == Qt::Key_Delete && m_pSelectedKey != 0) {
	event->accept();
	deleteKey();
	}*/
}

void EvolutionGraphWidget::updateCoordinatesTooltip( QPoint pos, Vector2f values )
{
	std::ostringstream os;
	os.precision(2);
	os.setf(std::ios::fixed, std::ios::floatfield);
	os << values.x*m_iMaximumIntensity << " / " << values.y*255.f;
	QToolTip::showText(mapToGlobal(pos), QString(os.str().c_str()));
}

int EvolutionGraphWidget::hitLine( const Vector2f& p )//sequence selected
{
	int hit = -1;
	float f;
	
	Vector2f pmin = Vector2f(m_xRange.x, m_yRange.x); //相对坐标的范围
	Vector2f pmax = Vector2f(m_xRange.y, m_yRange.y);

	float m_yTimeSpacing=(pmax.y - pmin.y)/float(m_pathNum+1);

	Vector2f sHit = Vector2f(p.x,  static_cast<float>(height()) - p.y); //世界坐标 妥妥滴



	for (int i=0; i <m_pathNum; ++i) 
	{
		f=pmin.y+m_yTimeSpacing*(i+1);
		Vector2f p = wtos(  Vector2f(0.0  ,  f )   );

		if (  abs(sHit.y - p.y) < 10) {  
			hit = i ;
			break;
		}

	}
	
	return hit;
}
Vector2i EvolutionGraphWidget::hitDot( const Vector2f& p )//sequence selected
{
	Vector2i hit = Vector2i(-1,-1) ;
	float f;

	Vector2f pmin = Vector2f(m_xRange.x, m_yRange.x); //相对坐标的范围
	Vector2f pmax = Vector2f(m_xRange.y, m_yRange.y);

	float m_yTimeSpacing=(pmax.y - pmin.y)/float(m_arryNumFeature[0]+1);
	float m_xTimeSpacing=(pmax.x - pmin.x)/float(m_timeNum+1);

	Vector2f sHit = Vector2f(p.x,  static_cast<float>(height()) - p.y); //世界坐标 妥妥滴


	//////////////////////////////////////////////////////////////////////////
	/*Vector2f(event->x(), event->y())

	Vector2f sHit = Vector2f(event->x(), static_cast<float>(height()) - event->y());
	Vector2f hit = stow(sHit);

	Vector2f sp = wtos(Vector2f(key->getIntensity(), key->getColor().a / 255.0));

	if (sHit.x > sp.x - m_iPointSize && sHit.x < sp.x + m_iPointSize &&
		sHit.y > sp.y - m_iPointSize && sHit.y < sp.y + m_iPointSize) {
			m_pSelectedKey = key;
	}*/
	//////////////////////////////////////////////////////////////////////////

	//cout<<"sHit："<<sHit;
	for (int i=0; i <m_timeNum; ++i) 
	{
		f=pmin.x+m_xTimeSpacing*(i+1);
		Vector2f p = wtos(  Vector2f(  f ,0.0  )   );

		if (  abs(sHit.x - p.x) < m_iPointSize) {  
			//cout<<"p："<<p;
			hit.x = i ;
			break;
		}

	}

	for (int j=0; j <m_timeNum; ++j) 
	{
		f=pmin.y+m_yTimeSpacing*(j+1);
		Vector2f p = wtos(  Vector2f(0.0  ,  f )   );

		if (  abs(sHit.y - p.y) < m_iPointSize) {  
			//cout<<"p："<<p;
			hit.y = j ;
			break;
		}

	}


	return hit;
}

Vector2f EvolutionGraphWidget::wtos( Vector2f p )
{
	float sx = (p.x - m_xRange[0])   /    (m_xRange[1] - m_xRange[0])   * (static_cast<float>(width()) \
		-  2*m_iPadding - 1.5 * m_iArrowLength) + m_iPadding;

	float sy = (p.y - m_yRange[0]) / (m_yRange[1] - m_yRange[0]) * (static_cast<float>(height()) \
		- m_iPadding - 1.5 * m_iArrowLength - m_iBottomSpace) + m_iBottomSpace;
	return Vector2f(sx, sy);
}

Vector2f EvolutionGraphWidget::stow( Vector2f p )
{
	float wx = (p.x - m_iPadding) / (static_cast<float>(width())  - \
		2 * m_iPadding - 1.5 * m_iArrowLength) * (m_xRange[1] - m_xRange[0]) + m_xRange[0];
	float wy = (p.y - m_iPadding - m_iBottomSpace) / (static_cast<float>(height()) - \
		m_iPadding - 1.5 * m_iArrowLength - m_iBottomSpace) * (m_yRange[1] - m_yRange[0]) + m_yRange[0];
	return Vector2f(wx, wy);
}

void EvolutionGraphWidget::drawMarker( QPainter& paint, const Color4& color, const Vector2f& p, int props /*= 0*/ )
{
	//if (m_bIsNoColor)
	//	paint.setBrush(Qt::transparent);
	//else
		paint.setBrush(Col2QColor(color));

	QPen pen(QBrush(QColor(62,145,237)), Qt::SolidLine);
	if (props & MARKER_SELECTED)
		pen.setWidth(3);
	paint.setPen(pen);

	paint.drawEllipse(QRectF(p.x - m_iPointSize/2, p.y - m_iPointSize/2, m_iPointSize, m_iPointSize));
}

QColor EvolutionGraphWidget::Col2QColor( const Color4& color )
{
	return QColor(color.r, color.g, color.b); // ignore alpha
}

Color4 EvolutionGraphWidget::QColor2Col( const QColor& color )
{
	return Color4(color.red(), color.green(), color.blue(), 255); // ignore alpha
}

void EvolutionGraphWidget::mouseDoubleClickEvent(QMouseEvent *event) 
{
	event->accept();
	if (event->button() == Qt::LeftButton)
		changeCurrentColor();
}

void EvolutionGraphWidget::changeCurrentColor()
{
	if (!m_pSelectedKey || m_bIsNoColor)
		return;

	QColor oldColor;
	oldColor = Col2QColor( m_pSelectedKey->getColor() );

	QColor newColor = QColorDialog::getColor(oldColor, 0);
	if (newColor.isValid())
		changeCurrentColor(newColor);
}

void EvolutionGraphWidget::changeCurrentColor(const QColor& c)
{
	if (!m_pSelectedKey || !c.isValid())
		return;

	Color4 voxercolor = QColor2Col(c);
	bool changedColor = false;

	//////////////////////////////////////////////////////////////////////////
	//set color
	

	set<Vector2i>::iterator iter;  
	iter = dag.pathNode.find(m_dotSelected);
	vector<int> pathFlag;
	if(iter != dag.pathNode.end())   //selected node里
	{  

		//////////////////////////////////////////////////////////////////////////
		// 在路径里查找
		for (int i=0;i<dag.m_pathInfo.size();i++)
		{
			vector<Vector2i> onePath=dag.m_pathInfo[i];
			vector<Vector2i>::iterator iterPath=onePath.begin();
			cout<<endl<<dag.m_pathInfo[i].size()<<i<<endl;
			for (;iterPath<onePath.end();iterPath++)
			{
				if (*iterPath==m_dotSelected)
				{
					
					pathFlag.push_back(i);
					
				}
			}

		}
		for (int i=0;i<pathFlag.size();i++)
		{
			vector<Vector2i> onePath=dag.m_pathInfo[pathFlag[i]];
			vector<Vector2i>::iterator iterPath=onePath.begin();
			for (;iterPath<onePath.end();iterPath++)
			{
				//	m_keyColletion=m_keyColletionFull[m_curTimeStep];
				//m_keys = m_keyColletion->getKeyCollection();

				TFMappingKey* m_key=m_keyColletionFull[(*iterPath).x]->getKey((*iterPath).y);
				m_key->setColor(voxercolor);

				changedColor = true;
			}

			
		}

	} 
	else 
	{  
		voxercolor.a = m_pSelectedKey->getColor().a;
		if (m_pSelectedKey->getColor() != voxercolor) {
			m_pSelectedKey->setColor(voxercolor);
			changedColor = true;
		}
	}  

	

	if (changedColor) {
		update();
		emit changed();
		emit colorChanged(c);
		setupTransfunc();
	}
}

void EvolutionGraphWidget::setupTransfunc()
{
	
	m_keyColletion=m_keyColletionFull[m_curTimeStep];
	m_keys = m_keyColletion->getKeyCollection();

	// yNum<m_arryNumFeature[timeStep+1]  
	unsigned char *transfuncArray = m_pTransfunc->getTFData();

	
	for (int x = 1; x < m_arryNumFeature[m_curTimeStep+1]+1; ++x)
	{
		TFMappingKey *key = m_keyColletion->getKey(x-1);			

		Color4 temp = key->getColor();
		transfuncArray[x*4] = temp.r;
		transfuncArray[x*4+1] = temp.g;
		transfuncArray[x*4+2] = temp.b;
		transfuncArray[x*4+3] = temp.a;
	}	
	transfuncArray[3] = 0;
	for (int x =  m_arryNumFeature[m_curTimeStep+1]+1; x < 256; ++x)
	{
				

		Color4 temp =  Color4(0, 255, 0, 0);
		transfuncArray[x*4] = temp.r;
		transfuncArray[x*4+1] = temp.g;
		transfuncArray[x*4+2] = temp.b;
		transfuncArray[x*4+3] = temp.a;
	}	

	emit sendTF(transfuncArray);
}
void EvolutionGraphWidget::setClusterTimeStep(const int timeStep  )
{
	
	m_curTimeStep=timeStep;

	update();
	
}


void EvolutionGraphWidget::setDataMin(float minData)
{
	m_dataMin=minData;
}
void EvolutionGraphWidget::setDataMax(float maxData)
{
	m_dataMax=maxData;
}
void EvolutionGraphWidget::setTimeNum(int timestepsNum)
{
	m_timeNum=timestepsNum;
}
void EvolutionGraphWidget::setFileDirName( char* fileDirName )
{
	dataLoad=true;

	m_fileDirName=fileDirName;
	dag.setFileDir(fileDirName);

	loadSequenceInfo();
	initMappingKey();

	dag.setTimeNum(m_timeNum);
	dag.setFeatureArray(m_arryNumFeature);
	dag.graphInput();
	dag.graphOutput();
	dag.initFeatureList();
	dag.pathGenerate();

	//send the tf to renderwidget
	setupTransfunc();

	update();
}

void EvolutionGraphWidget::loadSequenceInfo()
{
	char fn[255];
	memset(fn, 0, 255);
	//printf("Save to file.\n");
	sprintf(fn, "%s%s", m_fileDirName,"num.data");   //clusterCenterInfoTest3


	m_arryNumFeature = new unsigned char[m_timeNum+1];
	std::ifstream cin1(fn,std::ios::binary); 
	
	if(!cin1) 
		throw printf("无法打开文件");
	else
		printf("the information of the path has loaded......");


	//fread( &m_pathNum, sizeof(int), 1, fp1);
	cin1.read((char *)(m_arryNumFeature),(m_timeNum+1)*sizeof(unsigned char));
	
	

	cout<<(int)m_arryNumFeature[0]<<" "<<(int)m_arryNumFeature[1]<<" "<<(int)m_arryNumFeature[2]<<" "<<(int)m_arryNumFeature[3]<<endl;
	cin1.close();

}

void EvolutionGraphWidget::initMappingKey()
{
	for (int i=0;i<m_timeNum;i++)
	{
		TFMappingKeyCollection* keyTemp=new TFMappingKeyCollection();
		keyTemp->cleanNoInit();
		for (int j=0;j<m_arryNumFeature[i+1];j++)
		{
			TFMappingKey* key = new TFMappingKey(j, QColor2Col(Qt::green));//j+1
			keyTemp->addKey(key);
		}

		std::cout<<"i:"<<i<<"m_arryNumFeature:"<<int(m_arryNumFeature[i+1])<<"m_keyColletion:"<<keyTemp<<std::endl;
		m_keyColletionFull.push_back(keyTemp);
	}
	
}