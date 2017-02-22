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

	private:
		double ra;
		double dec;

		QBrush background;

		Rts2QVizier viz;
};

#endif // __RTS2Q_CAT__
