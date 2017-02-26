/**
 * Widget to display catalogues.
 */

#ifndef __RTS2Q_CAT__
#define __RTS2Q_CAT__

#include "rts2q_vizier.h"

#include <QWidget>

class Rts2QCat: public QWidget
{
	Q_OBJECT
	public:
		Rts2QCat(double ra, double dec, QWidget *parent = 0);

	protected slots:
		void starAdded();

	protected:
		void paintEvent(QPaintEvent *event) override;
		void mouseMoveEvent(QMouseEvent *event) override;
		void mousePressEvent(QMouseEvent *event) override;

	private:
		double ra;
		double dec;

		QBrush background;
		QBrush origin;

		Rts2QConditions conditions;
		Rts2QVizier viz;
};

#endif // __RTS2Q_CAT__
