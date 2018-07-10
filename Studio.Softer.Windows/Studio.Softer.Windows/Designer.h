#ifndef __DESIGNER__H_
#define __DESIGNER__H_

#include "studiosofterwindows_global.h"
#include "ui_Designer.h"

namespace Studio
{
	namespace Softer
	{
		namespace Windows
		{
			class STUDIOSOFTERWINDOWS_EXPORT Designer : public QMainWindow
			{
				Q_OBJECT

			public:
				explicit Designer(QWidget *parent = Q_NULLPTR);
				~Designer();

			private:
				Ui::Designer *ui;
			};
		}
	}
}

#endif