#include <QWidget>

#ifndef __RTS2Q_APP__
#define __RTS2Q_APP__

/**
 * Singleton class for all values sourced from RTS2. Rts2Widgets can register
 * to app class to have method rts2Updated called when updated value is fetched
 * from RTS2. 
 *
 * @author Petr Kubanek <kubanek@fzu.cz>
 */

class QRApp
{
	public:
		QRApp();

		static QRApp& getInstance() {Q_ASSERT(singleton); return *singleton;}
	private:

		static QRApp* singleton;

};

#endif // !__RTS2Q_APP__
