#ifndef COLORBAR_H
#define COLORBAR_H

#include <vector>
#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>

class ColorBar : public QWidget
{
	Q_OBJECT
public:
	ColorBar(QWidget * parent = 0);
	~ColorBar();
	/**
	 * @param data 颜色数据
	 * @param num 颜色个数
	 * @param BPP Byte per pixel,颜色占用的字节数，比如RGB的BPP为3，RGBA为4
	 */
	void setColor(unsigned char* data, int num, int BPP = 4);

	//void selectColorAt(int x);

	int colorIndexAt(int x);
	int colorMappedIndexAt(int x);

	void setSelected(int idx, int span = 1);

	void setIndexMap(const int* idxMap);

	const int* getIndexMap();

signals:
	void mousePressedAt(int idx);

protected:
	void paintEvent(QPaintEvent * event);
	void mousePressEvent(QMouseEvent * e);
	//void mouseMoveEvent(QMouseEvent * e);

private:
	QColor* m_color;
	int* m_idxMap;
	int m_colorNum;
	int m_binW;
	int m_binH;
	int m_paddingV;
	int m_paddingH;
	std::vector< std::pair<int, int> > m_selected;
};

#endif //COLORBAR_H
