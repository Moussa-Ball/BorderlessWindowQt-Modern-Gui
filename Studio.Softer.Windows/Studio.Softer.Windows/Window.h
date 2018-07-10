#ifndef __WINDOW__H_
#define __WINDOW__H_

#include "studiosofterwindows_global.h"

#include <QtWidgets>
#include <Windows.h>

namespace Studio
{
	namespace Softer
	{
		namespace Windows
		{
			class STUDIOSOFTERWINDOWS_EXPORT Window : public QMainWindow
			{
				Q_OBJECT
			public:
				explicit Window(QMenuBar *menuBar);
				void setIcon(const QString &iconPath);
				void setapplicationName(const QString &appName);
				void setOrganizationName(const QString &orgName);
				void showWindow();

			protected:
				bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
				void mousePressEvent(QMouseEvent *event) override;
				void closeEvent(QCloseEvent *event) override;
				void changeEvent(QEvent* e) override;

			private slots:
				void slot_show_system_menu() const;
				void slot_minimized();
				void slot_maximized();
				void slot_restored();
				void slot_closed();

			private:
				auto set_borderless(bool enabled) const -> void;
				QPushButton *minimize_button_;
				QPushButton *maximize_button_;
				QPushButton *restore_button_;
				QPushButton *close_Button_;
				QWidget *title_bar_widget_;
				QPushButton *icon_button_;
				QWidget *m_centralWidget;
				QString m_appIconPath;
				QPoint mouse_point_;
				QString m_appName;
				QString m_orgName;
			};
		}
	}
}


#endif