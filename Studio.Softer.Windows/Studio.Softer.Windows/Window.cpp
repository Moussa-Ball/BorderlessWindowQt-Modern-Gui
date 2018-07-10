#include "Window.h"
#include "Designer.h"

#include <cmath>
#include <QLayout>
#include <dwmapi.h>
#include <windows.h>
#include <windowsx.h>
#include <QApplication>
#pragma comment(lib, "dwmapi.lib")

namespace {
	// we cannot just use WS_POPUP style
	// WS_THICKFRAME: without this the window cannot be resized and so aero snap, de-maximizing and minimizing won't work
	// WS_SYSMENU: enables the context menu with the move, close, maximize, minimize... commands (shift + right-click on the task bar item)
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

Studio::Softer::Windows::Window::Window(QMenuBar *menuBar)
	: minimize_button_(Q_NULLPTR), maximize_button_(Q_NULLPTR), restore_button_(Q_NULLPTR),
	close_Button_(Q_NULLPTR), title_bar_widget_(Q_NULLPTR), icon_button_(Q_NULLPTR)
{
	setWindowFlags(Qt::FramelessWindowHint);
	set_borderless(true);

	// set black background
	auto pal = palette();
	pal.setColor(QPalette::Background, Qt::black);
	setAutoFillBackground(true);
	setPalette(pal);
	setBackgroundRole(QPalette::Dark);

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
	icon_button_->setIconSize(QSize(20, 20));
	icon_button_->setStyleSheet("margin-left: 5px; background-color: transparent; border: none;");
	icon_button_->setObjectName("titleBarIcon");
	icon_button_->setFocusPolicy(Qt::NoFocus);
	connect(icon_button_, SIGNAL(clicked()), this, SLOT(slot_show_system_menu()));

	//Represent the minimize button.
	minimize_button_ = new QPushButton(title_bar_widget_);
	minimize_button_->setFixedSize(36, 29);
	minimize_button_->setObjectName("minimizeButton");
	minimize_button_->setToolTip(tr("Minimize"));
	minimize_button_->setFocusPolicy(Qt::NoFocus);
	connect(minimize_button_, SIGNAL(clicked()), this, SLOT(slot_minimized()));

	//Represent the maximize button.
	maximize_button_ = new QPushButton(title_bar_widget_);
	maximize_button_->setFixedSize(36, 29);
	maximize_button_->setObjectName("maximizeButton");
	maximize_button_->setToolTip(tr("Maximize"));
	maximize_button_->setVisible(true);
	maximize_button_->setFocusPolicy(Qt::NoFocus);
	connect(maximize_button_, SIGNAL(clicked()), this, SLOT(slot_maximized()));

	//Represent the restore button.
	restore_button_ = new QPushButton(title_bar_widget_);
	restore_button_->setFixedSize(36, 29);
	restore_button_->setObjectName("restoreButton");
	restore_button_->setToolTip(tr("Restore"));
	restore_button_->setVisible(false);
	restore_button_->setFocusPolicy(Qt::NoFocus);
	connect(restore_button_, SIGNAL(clicked()), this, SLOT(slot_restored()));

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
	horizontalLayout->addWidget(menuBar);

	//Add space beetwen the menubar and command icon
	horizontalLayout->addStretch(1);

	//Add button command.
	horizontalLayout->addWidget(minimize_button_);
	horizontalLayout->addWidget(restore_button_);
	horizontalLayout->addWidget(maximize_button_);
	horizontalLayout->addWidget(close_Button_);

	auto verticalLayout = new QVBoxLayout();
	verticalLayout->setAlignment(Qt::AlignTop);
	verticalLayout->setSpacing(0);
	verticalLayout->setMargin(0);

	//Add the window title bar in the first layout in vertical.
	verticalLayout->addWidget(title_bar_widget_, 0, Qt::AlignTop);

	//Add the window content here.
	auto winHost = new Designer;
	verticalLayout->addWidget(winHost, 1);

	//Add an central widget of this window.
	m_centralWidget = new QWidget(this);
	m_centralWidget->setObjectName("centralWidget");
	m_centralWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_centralWidget->setLayout(verticalLayout);
	m_centralWidget->setContentsMargins(0, 0, 0, 0);
	setCentralWidget(m_centralWidget);
}

bool Studio::Softer::Windows::Window::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
	Q_UNUSED(eventType);

	auto msg = static_cast<MSG *>(message);

	switch (msg->message)
	{
	case WM_NCCREATE: {
		auto userdata = reinterpret_cast<CREATESTRUCTW*>(msg->lParam)->lpCreateParams;
		// store window instance pointer in window user data
		::SetWindowLongPtrW(msg->hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(userdata));
	}

	case WM_ERASEBKGND:
	{
		auto brush = CreateSolidBrush(RGB(48, 48, 48));
		SetClassLongPtr(msg->hwnd, GCLP_HBRBACKGROUND, reinterpret_cast<LONG_PTR>(brush));
	}

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

	case WM_GETMINMAXINFO: {
		MINMAXINFO* mmi = reinterpret_cast<MINMAXINFO*>(msg->lParam);

		if (maximized(msg->hwnd)) {

			RECT window_rect;

			if (!GetWindowRect(msg->hwnd, &window_rect)) {
				return false;

			}

			HMONITOR monitor = MonitorFromRect(&window_rect, MONITOR_DEFAULTTONULL);
			if (!monitor) {
				return false;
			}

			MONITORINFO monitor_info = { 0 };
			monitor_info.cbSize = sizeof(monitor_info);
			GetMonitorInfo(monitor, &monitor_info);

			RECT work_area = monitor_info.rcWork;
			RECT monitor_rect = monitor_info.rcMonitor;

			mmi->ptMaxPosition.x = abs(work_area.left - monitor_rect.left);
			mmi->ptMaxPosition.y = abs(work_area.top - monitor_rect.top);
			
			mmi->ptMaxSize.x = abs(work_area.right - work_area.left);
			mmi->ptMaxSize.y = abs(work_area.bottom - work_area.top);
			mmi->ptMaxTrackSize.x = mmi->ptMaxSize.x;
			mmi->ptMaxTrackSize.y = mmi->ptMaxSize.y;

			*result = 1;
			return true;
		}
	}

	case WM_NCACTIVATE: {
		if (!composition_enabled()) {
			// Prevents window frame reappearing on window activation
			// in "basic" theme, where no aero shadow is present.
			*result = 1;
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
				::SetWindowPos(reinterpret_cast<HWND>(winId()), Q_NULLPTR, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
			}
		}
	}

	default: QWidget::nativeEvent(eventType, message, result);
	}

	return QWidget::nativeEvent(eventType, message, result);
}

void Studio::Softer::Windows::Window::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::RightButton)
	{
		if (icon_button_->underMouse())
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

void Studio::Softer::Windows::Window::changeEvent(QEvent* e)
{
	QWidget::changeEvent(e);

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

	/*if (e->type() == QEvent::ActivationChange)
	{
	if (this->isActiveWindow())
	{

	}
	else
	{

	}
	}*/
}

void Studio::Softer::Windows::Window::slot_show_system_menu() const
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

		if (windowState() != Qt::WindowMaximized)
		{
			LPARAM cmd = TrackPopupMenu(hMenu, (TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
				winrect.left, winrect.top + 30, NULL, reinterpret_cast<HWND>(winId()), Q_NULLPTR);

			if (cmd) PostMessage(reinterpret_cast<HWND>(winId()), WM_SYSCOMMAND, cmd, 0);
		}
		else
		{
			auto monitor = MonitorFromWindow(reinterpret_cast<HWND>(winId()), MONITOR_DEFAULTTONULL);
			MONITORINFO monitor_info{};
			monitor_info.cbSize = sizeof(monitor_info);
			GetMonitorInfoW(monitor, &monitor_info);
			auto rect = monitor_info.rcWork;

			LPARAM cmd = TrackPopupMenu(hMenu, (TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
				rect.left, rect.top + 30, NULL, reinterpret_cast<HWND>(winId()), Q_NULLPTR);

			if (cmd) PostMessage(reinterpret_cast<HWND>(winId()), WM_SYSCOMMAND, cmd, 0);
		}

	}
}

void Studio::Softer::Windows::Window::slot_minimized()
{
	setWindowState(Qt::WindowMinimized);
}

void Studio::Softer::Windows::Window::slot_maximized()
{
	restore_button_->setVisible(true);
	maximize_button_->setVisible(false);
	setWindowState(Qt::WindowMaximized);
}

void Studio::Softer::Windows::Window::slot_restored()
{
	restore_button_->setVisible(false);
	maximize_button_->setVisible(true);
	setWindowState(Qt::WindowNoState);
}

void Studio::Softer::Windows::Window::slot_closed()
{
	close();
}

auto Studio::Softer::Windows::Window::set_borderless(bool enabled) const -> void
{
	auto new_style = (enabled) ? select_borderless_style() : Style::windowed;
	auto old_style = static_cast<Style>(::GetWindowLongPtrW(reinterpret_cast<HWND>(winId()), GWL_STYLE));

	if (new_style != old_style) {
		::SetWindowLongPtrW(reinterpret_cast<HWND>(winId()), GWL_STYLE, static_cast<LONG>(new_style));

		// redraw frame
		::SetWindowPos(reinterpret_cast<HWND>(winId()), nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
	}
}

void Studio::Softer::Windows::Window::showWindow()
{
	setWindowIcon(QIcon(m_appIconPath));
	icon_button_->setIcon(QIcon(QPixmap(m_appIconPath)));

	QSettings settings(m_orgName, m_appName);
	restoreGeometry(settings.value("Window/Geometry").toByteArray());
	restoreState(settings.value("Window/WindowState").toByteArray());
	
	show();

	settings.beginGroup("Window/WindowState");
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

void Studio::Softer::Windows::Window::closeEvent(QCloseEvent* event)
{
	QSettings settings(m_orgName, m_appName);
	settings.setValue("Window/WindowState", saveState());
	settings.setValue("Window/Geometry", saveGeometry());
	event->accept();
}

void Studio::Softer::Windows::Window::setapplicationName(const QString& appName)
{
	m_appName = appName;
}

void Studio::Softer::Windows::Window::setOrganizationName(const QString& orgName)
{
	m_orgName = orgName;
}

void Studio::Softer::Windows::Window::setIcon(const QString& iconPath)
{
	m_appIconPath = iconPath;
}
