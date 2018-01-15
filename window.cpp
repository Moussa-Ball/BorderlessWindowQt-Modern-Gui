#include "Window.h"

#include <QLayout>
#include <QApplication>
#include <Windows.h>
#include <windowsx.h>
#include <dwmapi.h>

#ifdef Q_OS_WIN

namespace {
	// we cannot just use WS_POPUP style
	// WS_THICKFRAME: without this the window cannot be resized and so aero snap, de-maximizing and minimizing won't work
	// WS_SYSMENU: enables the context menu with the move, close, maximize, minize... commands (shift + right-click on the task bar item)
	// WS_CAPTION: enables aero minimize animation/transition
	// WS_MAXIMIZEBOX, WS_MINIMIZEBOX: enable minimize/maximize
	enum class Style : DWORD {
		windowed = WS_OVERLAPPEDWINDOW | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		aero_borderless = WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX,
		basic_borderless = WS_POPUP | WS_THICKFRAME | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX
	};

	auto maximized(HWND hwnd) -> bool {
		WINDOWPLACEMENT placement;
		if (!::GetWindowPlacement(hwnd, &placement)) {
			return false;
		}

		return placement.showCmd == SW_MAXIMIZE;
	}

	/* Adjust client rect to not spill over monitor edges when maximized.
	* rect(in/out): in: proposed window rect, out: calculated client rect
	* Does nothing if the window is not maximized.
	*/
	auto adjust_maximized_client_rect(HWND window, RECT& rect) -> void {
		if (!maximized(window)) {
			return;
		}

		auto monitor = ::MonitorFromWindow(window, MONITOR_DEFAULTTONULL);
		if (!monitor) {
			return;
		}

		MONITORINFO monitor_info{};
		monitor_info.cbSize = sizeof(monitor_info);
		if (!::GetMonitorInfoW(monitor, &monitor_info)) {
			return;
		}

		// when maximized, make the client area fill just the monitor (without task bar) rect,
		// not the whole window rect which extends beyond the monitor.
		rect = monitor_info.rcWork;
	}

	auto composition_enabled() -> bool {
		auto composition_enabled = FALSE;
		auto success = ::DwmIsCompositionEnabled(&composition_enabled) == S_OK;
		return composition_enabled && success;
	}

	auto select_borderless_style() -> Style {
		return composition_enabled() ? Style::aero_borderless : Style::basic_borderless;
	}
}

#endif

Window::Window()
{
	set_borderless(true);

	auto verticalLayout = new QVBoxLayout();
	verticalLayout->setAlignment(Qt::AlignTop);
	verticalLayout->setSpacing(0);
	verticalLayout->setMargin(0);

	auto horizontalLayout = new QHBoxLayout();
	horizontalLayout->setSpacing(0);
	horizontalLayout->setMargin(0);

	//Represent the window title bar.
	title_bar_widget_ = new QWidget(this);
	title_bar_widget_->setFixedHeight(30);
	title_bar_widget_->setObjectName("windowTitleBar");
	title_bar_widget_->setContentsMargins(0, 0, 0, 0);
	title_bar_widget_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	title_bar_widget_->setLayout(horizontalLayout);

	icon_button_ = new QPushButton(title_bar_widget_);
    icon_button_->setIcon(QIcon(QPixmap(":/Icons/icon.ico")));
	icon_button_->setIconSize(QSize(20, 20));
	icon_button_->setObjectName("titleBarIcon");
	icon_button_->setFocusPolicy(Qt::NoFocus);
	connect(icon_button_, SIGNAL(clicked()), this, SLOT(slot_show_system_menu()));
	
	//Represent the menu bar of this window.
	menu_bar_ = new QMenuBar(title_bar_widget_);
	menu_bar_->setObjectName("menuBar");
	menu_bar_->setNativeMenuBar(true);
	
	auto fileMenu = new QMenu(tr("&File"), menu_bar_);
	fileMenu->setObjectName("menu");
	auto editMenu = new QMenu(tr("&Edit"), menu_bar_);
	editMenu->setObjectName("menu");
	auto viewMenu = new QMenu(tr("&View"), menu_bar_);
	viewMenu->setObjectName("menu");
	auto layerMenu = new QMenu(tr("&Layer"), menu_bar_);
	layerMenu->setObjectName("menu");
	auto textMenu = new QMenu(tr("&Text"), menu_bar_);
	textMenu->setObjectName("menu");
	auto selectMenu = new QMenu(tr("&Select"), menu_bar_);
	selectMenu->setObjectName("menu");
	auto windowMenu = new QMenu(tr("&Window"), menu_bar_);
	windowMenu->setObjectName("menu");
	auto helpMenu = new QMenu(tr("&Help"), menu_bar_);
	helpMenu->setObjectName("menu");

	fileMenu->addAction(tr("&New"))->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
	fileMenu->addAction(tr("&Open"))->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
	auto nothing = new QAction(tr("&Nothing"));
	nothing->setObjectName("menu");
	nothing->setEnabled(false);
	fileMenu->addMenu(tr("&Open recents"))->addAction(nothing);
	fileMenu->addSeparator();
	fileMenu->addAction(tr("&Save"))->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
	fileMenu->addAction(tr("&Save as..."))->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
	fileMenu->addSeparator();
	fileMenu->addAction(tr("&Close"))->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_W));
	fileMenu->addAction(tr("&Close all.."))->setShortcut(QKeySequence(Qt::ALT + Qt::CTRL + Qt::Key_W));
	fileMenu->addSeparator();
	fileMenu->addAction(tr("&Edit In Photo Studio"));
	fileMenu->addSeparator();
	fileMenu->addAction(tr("&Import"))->setShortcut(QKeySequence(Qt::ALT + Qt::SHIFT + Qt::Key_I));
	fileMenu->addAction(tr("&Export"))->setShortcut(QKeySequence(Qt::ALT + Qt::SHIFT + Qt::Key_S));
	fileMenu->addSeparator();
	fileMenu->addAction(tr("&Print"))->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
	fileMenu->addSeparator();
	fileMenu->addAction(tr("&Exit"))->setShortcut(QKeySequence(Qt::ALT + Qt::Key_Q));

	editMenu->addAction(tr("&Undo"))->setShortcut(QKeySequence(Qt::ALT + Qt::Key_Z));
	editMenu->addAction(tr("&Redo"))->setShortcut(QKeySequence(Qt::ALT + Qt::Key_Y));
	editMenu->addSeparator();
	editMenu->addAction(tr("&Cut"))->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
	editMenu->addAction(tr("&Copy"))->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
	editMenu->addAction(tr("&Paste"))->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
	editMenu->addAction(tr("&Paste special"));
	editMenu->addAction(tr("&Duplicate"))->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_J));
	editMenu->addAction(tr("&Delete"))->setShortcut(QKeySequence(Qt::Key_Delete));
	editMenu->addSeparator();
	auto preferencesMenu = new QMenu(tr("&Preferences"), editMenu);
	preferencesMenu->setObjectName("submenu");
	preferencesMenu->addAction(tr("&Generals"));
	preferencesMenu->addAction(tr("&Interface"));
	preferencesMenu->addAction(tr("&Colors"));
	preferencesMenu->addAction(tr("&Shorcuts"));
	preferencesMenu->addAction(tr("&Tools"));
	preferencesMenu->addAction(tr("&Performances"));
	editMenu->addMenu(preferencesMenu);

	viewMenu->addAction(tr("&Show Toolbar"));
	viewMenu->addAction(tr("&Show Tools"));
	viewMenu->addSeparator();
	viewMenu->addAction(tr("&Rotate Left"));
	viewMenu->addAction(tr("&Rotate Right"));
	viewMenu->addAction(tr("&Reset Rotation"));
	viewMenu->addSeparator();
	auto zoomMenu = new QMenu(tr("&Zoom"), viewMenu);
	zoomMenu->setObjectName("submenu");
	zoomMenu->addAction(tr("&Zoom In"))->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Plus));
	zoomMenu->addAction(tr("&Zoom Out"))->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_hyphen));
	zoomMenu->addSeparator();
	zoomMenu->addAction(tr("&100%"));
	zoomMenu->addAction(tr("&200%"));
	zoomMenu->addAction(tr("&300%"));
	zoomMenu->addAction(tr("&400%"));
	zoomMenu->addAction(tr("&500%"));
	zoomMenu->addAction(tr("&600%"));
	zoomMenu->addAction(tr("&700%"));
	zoomMenu->addAction(tr("&800%"));
	zoomMenu->addAction(tr("&900%"));
	zoomMenu->addSeparator();
	zoomMenu->addAction(tr("&Print size"));
	viewMenu->addMenu(zoomMenu);
	viewMenu->addSeparator();
	viewMenu->addAction(tr("&Show Rulers"));
	viewMenu->addAction(tr("&Show Guides"));
	viewMenu->addAction(tr("&Show Margins"));
	viewMenu->addAction(tr("&Show Grids"));
	viewMenu->addSeparator();
	auto none = new QAction(tr("&None"));
	none->setObjectName("menu");
	none->setEnabled(false);
	viewMenu->addMenu(tr("&Extensions"))->addAction(none);

	menu_bar_->addMenu(fileMenu);
	menu_bar_->addMenu(editMenu);
	menu_bar_->addMenu(viewMenu);
	menu_bar_->addMenu(layerMenu);
	menu_bar_->addMenu(textMenu);
	menu_bar_->addMenu(selectMenu);
	menu_bar_->addMenu(windowMenu);
	menu_bar_->addMenu(helpMenu);

	//Represent the minimize button.
	minimize_button_ = new QPushButton(title_bar_widget_);
	minimize_button_->setFixedSize(36, 29);
	minimize_button_->setObjectName("minimizeButton");
	minimize_button_->setToolTip(tr("Minimize"));
	minimize_button_->setFocusPolicy(Qt::NoFocus);
	connect(minimize_button_, SIGNAL(clicked()), this, SLOT(slot_minimized()));

	//Represent the restore button.
	restore_button_ = new QPushButton(title_bar_widget_);
	restore_button_->setFixedSize(36, 29);
	restore_button_->setObjectName("restoreButton");
	restore_button_->setToolTip(tr("Restore"));
	restore_button_->setVisible(false);
	restore_button_->setFocusPolicy(Qt::NoFocus);
	connect(restore_button_, SIGNAL(clicked()), this, SLOT(slot_restored()));

	//Represent the maximize button.
	maximize_button_ = new QPushButton(title_bar_widget_);
	maximize_button_->setFixedSize(36, 29);
	maximize_button_->setObjectName("maximizeButton");
	maximize_button_->setToolTip(tr("Maximize"));
	maximize_button_->setFocusPolicy(Qt::NoFocus);
	connect(maximize_button_, SIGNAL(clicked()), this, SLOT(slot_maximized()));

	//Represent the close button.
	close_Button_ = new QPushButton(title_bar_widget_);
	close_Button_->setFixedSize(36, 29);
	close_Button_->setObjectName("closeButton");
	close_Button_->setToolTip(tr("Close"));
	close_Button_->setFocusPolicy(Qt::NoFocus);
	connect(close_Button_, SIGNAL(clicked()), this, SLOT(slot_closed()));

	//Add icon on the title bar.
	horizontalLayout->addWidget(icon_button_);

	//Add a menu bar on the window title bar.
	horizontalLayout->addWidget(menu_bar_);

	//Add space beetwen the menubar and command icon
	horizontalLayout->addStretch(1);

	//Add button command.
	horizontalLayout->addWidget(minimize_button_);
	horizontalLayout->addWidget(restore_button_);
	horizontalLayout->addWidget(maximize_button_);
	horizontalLayout->addWidget(close_Button_);

	//Add the window title bar in the first layout in vertical.
	verticalLayout->addWidget(title_bar_widget_, 0);

	//Add an central widget of this window.
	auto centralWidget = new QWidget(this);
	centralWidget->setObjectName("centralWidget");
	centralWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	centralWidget->setLayout(verticalLayout);
	centralWidget->setContentsMargins(0, 0, 0, 0);
	setCentralWidget(centralWidget);
}

/**
 * \brief Show window and the good settings as state and geometry.
 */
void Window::show()
{
    QSettings settings("SofterStudio", "Designer Studio");
	restoreGeometry(settings.value("Window/Geometry").toByteArray());

	settings.beginGroup("Window");
	if (settings.value("maximized", isMaximized()).toBool())
	{
		showNormal();
		showMaximized();
	}
	else
	{
		showNormal();
	}
	settings.endGroup();
}

/**
 * \brief Represent an event when the window is closing.
 * \param event The closing event.
 */
void Window::closeEvent(QCloseEvent* event)
{
    QSettings settings("SofterStudio", "Designer Studio");
	settings.setValue("Window/Geometry", saveGeometry());
	settings.setValue("Window/WindowState", saveState());
}

#ifdef Q_OS_WIN

bool Window::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
	// ReSharper disable once CppEntityNeverUsed
	auto msg = static_cast<MSG *>(message);

	switch (msg->message)
	{
		case WM_NCCALCSIZE: {
			if (msg->wParam == TRUE) {
				auto& params = *reinterpret_cast<NCCALCSIZE_PARAMS*>(msg->lParam);
				adjust_maximized_client_rect(msg->hwnd, params.rgrc[0]);
			}

			*result = 0;
			return true;
		}

		case WM_NCHITTEST: {
			*result = 0;
			const LONG border_width = 8; //in pixels
			RECT winrect;
			GetWindowRect(reinterpret_cast<HWND>(winId()), &winrect);

			long x = GET_X_LPARAM(msg->lParam);
			long y = GET_Y_LPARAM(msg->lParam);

			auto resizeWidth = minimumWidth() != maximumWidth();
			auto resizeHeight = minimumHeight() != maximumHeight();

			if (resizeWidth)
			{
				//left border
				if (x >= winrect.left && x < winrect.left + border_width)
				{
					*result = HTLEFT;
				}
				//right border
				if (x < winrect.right && x >= winrect.right - border_width)
				{
					*result = HTRIGHT;
				}
			}
			if (resizeHeight)
			{
				//bottom border
				if (y < winrect.bottom && y >= winrect.bottom - border_width)
				{
					*result = HTBOTTOM;
				}
				//top border
				if (y >= winrect.top && y < winrect.top + border_width)
				{
					*result = HTTOP;
				}
			}
			if (resizeWidth && resizeHeight)
			{
				//bottom left corner
				if (x >= winrect.left && x < winrect.left + border_width &&
					y < winrect.bottom && y >= winrect.bottom - border_width)
				{
					*result = HTBOTTOMLEFT;
				}
				//bottom right corner
				if (x < winrect.right && x >= winrect.right - border_width &&
					y < winrect.bottom && y >= winrect.bottom - border_width)
				{
					*result = HTBOTTOMRIGHT;
				}
				//top left corner
				if (x >= winrect.left && x < winrect.left + border_width &&
					y >= winrect.top && y < winrect.top + border_width)
				{
					*result = HTTOPLEFT;
				}
				//top right corner
				if (x < winrect.right && x >= winrect.right - border_width &&
					y >= winrect.top && y < winrect.top + border_width)
				{
					*result = HTTOPRIGHT;
				}
			}

			if (*result != 0)
				return true;

			auto action = QApplication::widgetAt(QCursor::pos());
			if (action == title_bar_widget_) {
				*result = HTCAPTION;
				return true;
			}
			break;
		}

		case WM_NCACTIVATE: {
			if (!composition_enabled()) {
				// Prevents window frame reappearing on window activation
				// in "basic" theme, where no aero shadow is present.
				return true;
			}
			break;
		}

		case WM_SIZE: {
			RECT winrect;
			GetClientRect(msg->hwnd, &winrect);

			WINDOWPLACEMENT wp;
			wp.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(msg->hwnd, &wp);
			if (this)
			{
				if (wp.showCmd == SW_MAXIMIZE)
				{
					::SetWindowPos(reinterpret_cast<HWND>(winId()), nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
				}
			}
		}

		default: QWidget::nativeEvent(eventType, message, result);
	}

	return QWidget::nativeEvent(eventType, message, result);
}

void Window::mousePressEvent(QMouseEvent* event)
{
	if(event->button() == Qt::RightButton)
	{
		if(icon_button_->underMouse())
		{
			auto hMenu = GetSystemMenu(reinterpret_cast<HWND>(winId()), FALSE);

			if (hMenu)
			{
				MENUITEMINFO mii;
				mii.cbSize = sizeof(MENUITEMINFO);
				mii.fMask = MIIM_STATE;
				mii.fType = 0;

				// update the options
				mii.fState = MF_ENABLED;
				SetMenuItemInfo(hMenu, SC_RESTORE, FALSE, &mii);
				SetMenuItemInfo(hMenu, SC_SIZE, FALSE, &mii);
				SetMenuItemInfo(hMenu, SC_MOVE, FALSE, &mii);
				SetMenuItemInfo(hMenu, SC_MAXIMIZE, FALSE, &mii);
				SetMenuItemInfo(hMenu, SC_MINIMIZE, FALSE, &mii);

				mii.fState = MF_GRAYED;

				WINDOWPLACEMENT wp;
				GetWindowPlacement(reinterpret_cast<HWND>(winId()), &wp);

				switch (wp.showCmd)
				{
				case SW_SHOWMAXIMIZED:
					SetMenuItemInfo(hMenu, SC_SIZE, FALSE, &mii);
					SetMenuItemInfo(hMenu, SC_MOVE, FALSE, &mii);
					SetMenuItemInfo(hMenu, SC_MAXIMIZE, FALSE, &mii);
					SetMenuDefaultItem(hMenu, SC_CLOSE, FALSE);
					break;
				case SW_SHOWMINIMIZED:
					SetMenuItemInfo(hMenu, SC_MINIMIZE, FALSE, &mii);
					SetMenuDefaultItem(hMenu, SC_RESTORE, FALSE);
					break;
				case SW_SHOWNORMAL:
					SetMenuItemInfo(hMenu, SC_RESTORE, FALSE, &mii);
					SetMenuDefaultItem(hMenu, SC_CLOSE, FALSE);
					break;
				default:;
				}
				
				POINT p;
				if (GetCursorPos(&p))
				{
					LPARAM cmd = TrackPopupMenu(hMenu, (TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
						p.x, p.y, NULL, reinterpret_cast<HWND>(winId()), Q_NULLPTR);

					if (cmd) PostMessage(reinterpret_cast<HWND>(winId()), WM_SYSCOMMAND, cmd, 0);
				}
			}
		}
	}
}

void Window::changeEvent(QEvent* e)
{
	if (e->type() == QEvent::WindowStateChange)
	{
		auto ev = static_cast<QWindowStateChangeEvent*>(e);

		if (!(ev->oldState() & Qt::WindowMaximized) && windowState() & Qt::WindowMaximized)
		{
			restore_button_->setVisible(true);
			maximize_button_->setVisible(false);
		}
		else
		{
			restore_button_->setVisible(false);
			maximize_button_->setVisible(true);
		}
	}

	QWidget::changeEvent(e);
}

void Window::slot_show_system_menu() const
{
	auto hMenu = GetSystemMenu(reinterpret_cast<HWND>(winId()), FALSE);

	if (hMenu)
	{
		MENUITEMINFO mii;
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fMask = MIIM_STATE;
		mii.fType = 0;

		// update the options
		mii.fState = MF_ENABLED;
		SetMenuItemInfo(hMenu, SC_RESTORE, FALSE, &mii);
		SetMenuItemInfo(hMenu, SC_SIZE, FALSE, &mii);
		SetMenuItemInfo(hMenu, SC_MOVE, FALSE, &mii);
		SetMenuItemInfo(hMenu, SC_MAXIMIZE, FALSE, &mii);
		SetMenuItemInfo(hMenu, SC_MINIMIZE, FALSE, &mii);

		mii.fState = MF_GRAYED;

		WINDOWPLACEMENT wp;
		GetWindowPlacement(reinterpret_cast<HWND>(winId()), &wp);

		switch (wp.showCmd)
		{
			case SW_SHOWMAXIMIZED:
				SetMenuItemInfo(hMenu, SC_SIZE, FALSE, &mii);
				SetMenuItemInfo(hMenu, SC_MOVE, FALSE, &mii);
				SetMenuItemInfo(hMenu, SC_MAXIMIZE, FALSE, &mii);
				SetMenuDefaultItem(hMenu, SC_CLOSE, FALSE);
				break;
			case SW_SHOWMINIMIZED:
				SetMenuItemInfo(hMenu, SC_MINIMIZE, FALSE, &mii);
				SetMenuDefaultItem(hMenu, SC_RESTORE, FALSE);
				break;
			case SW_SHOWNORMAL:
				SetMenuItemInfo(hMenu, SC_RESTORE, FALSE, &mii);
				SetMenuDefaultItem(hMenu, SC_CLOSE, FALSE);
				break;
			default:;
		}

		RECT winrect;
		GetWindowRect(reinterpret_cast<HWND>(winId()), &winrect);
		
		if(windowState() != Qt::WindowMaximized)
		{
			LPARAM cmd = TrackPopupMenu(hMenu, (TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
                winrect.left, winrect.top + 30, NULL, reinterpret_cast<HWND>(winId()), NULL);

			if (cmd) PostMessage(reinterpret_cast<HWND>(winId()), WM_SYSCOMMAND, cmd, 0);
		}else
		{
			auto monitor = MonitorFromWindow(reinterpret_cast<HWND>(winId()), MONITOR_DEFAULTTONULL);
			MONITORINFO monitor_info{};
			monitor_info.cbSize = sizeof(monitor_info);
			GetMonitorInfoW(monitor, &monitor_info);
			auto rect = monitor_info.rcWork;

			LPARAM cmd = TrackPopupMenu(hMenu, (TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
                rect.left, rect.top + 30, NULL, reinterpret_cast<HWND>(winId()), NULL);

			if (cmd) PostMessage(reinterpret_cast<HWND>(winId()), WM_SYSCOMMAND, cmd, 0);
		}
		
	}
}

void Window::slot_minimized()
{
	setWindowState(Qt::WindowMinimized);
}

void Window::slot_restored()
{
	restore_button_->setVisible(false);
	maximize_button_->setVisible(true);
	setWindowState(Qt::WindowNoState);
}

void Window::slot_maximized()
{
	restore_button_->setVisible(true);
	maximize_button_->setVisible(false);
	setWindowState(Qt::WindowMaximized);
}

void Window::slot_closed()
{
	close();
}

auto Window::set_borderless(bool enabled) const -> void
{
	auto new_style = (enabled) ? select_borderless_style() : Style::windowed;
	auto old_style = static_cast<Style>(::GetWindowLongPtrW(reinterpret_cast<HWND>(winId()), GWL_STYLE));

	if (new_style != old_style) {
		::SetWindowLongPtrW(reinterpret_cast<HWND>(winId()), GWL_STYLE, static_cast<LONG>(new_style));

		// redraw frame
		::SetWindowPos(reinterpret_cast<HWND>(winId()), nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
		::ShowWindow(reinterpret_cast<HWND>(winId()), SW_SHOW);
	}
}

#endif
