/**
 * Widget to display catalogues.
 */

#ifndef __RTS2Q_CAT__
#define __RTS2Q_CAT__

#include "rts2q_vizier.h"
#include "rts2q_fits.h"

#include "ui_histogramDialog.h"

#include <QWidget>
#include <QMenu>

class Rts2QCat: public QWidget
{
	Q_OBJECT
	public:
		Rts2QCat(double ra, double dec, QWidget *parent = 0);

	protected slots:
		void starAdded();
		void showHistogram();
		void histogramApply();

	protected:
		void paintEvent(QPaintEvent *event) override;
		void mouseMoveEvent(QMouseEvent *event) override;
		void mousePressEvent(QMouseEvent *event) override;
		void mouseReleaseEvent(QMouseEvent *event) override;
		void wheelEvent(QWheelEvent *event) override;
		void keyPressEvent(QKeyEvent *event) override;

	private:
		double ra;
		double dec;

		QBrush background;
		QBrush origin;
		QBrush selected;

		Rts2QConditions conditions;
		Rts2QVizier viz;

		QVector <QString> selectedStars;

		void paintStars(QPainter *painter);
		void paintAG(QPainter *painter);

		uint16_t imgMin;
		uint16_t imgMax;

		QFitsImage fi;

		int lastX;
		int lastY;

		QMenu contextMenu;

		Ui_histogramDialog histogramDialog;
};

#endif // __RTS2Q_CAT__
