#include "Application.h"
#include "Window.h"

#include <QFile>

/**
* \brief Allows to initialize a new application.
* \param argc The integer(argc) of the main function.
* \param argv The *char[] pointer of the main function.
*/
Studio::Softer::Application::Application(int argc, char *argv[]) :
	m_splashScreen(Q_NULLPTR), m_sharedMemory(Q_NULLPTR), m_application(Q_NULLPTR),
	m_menuBar(Q_NULLPTR)
{
	//Initialize a new application.
	m_application = new QApplication(argc, argv);

	//Initialize a new shared memory.
	m_sharedMemory = new QSharedMemory("{cca65ba4-6e42-4997-99a3-7e143aaf83b5}");

	//Enables high-DPI scaling on windows.
	m_application->setAttribute(Qt::AA_EnableHighDpiScaling);
	//Allows to use pixmap sizes in layout geometry.
	//Calculations should typically divide by devicePixelRatio().
	m_application->setAttribute(Qt::AA_UseHighDpiPixmaps);

	//Indicates the application infos.
	m_application->setApplicationName(getApplicationName());
	m_application->setApplicationVersion(getApplicationVersion());
	m_application->setOrganizationName(getOrganizationName());
	m_application->setOrganizationDomain(getOrganizationDomain());

	//Allows to use a stylesheet file to skinning this application.
	QFile File(":/themes/DarkStyle.qss");
	File.open(QFile::ReadOnly);
	m_application->setStyleSheet(File.readAll());
}


/**
* \brief Allows to set the product type for an application.
* \param product The product type of an application.
*/
void Studio::Softer::Application::setProductType(const ProductType& product)
{
	m_product = product;
}


/**
* \brief Allows to execute the application.
* \return The good integer when the application is running or not.
*/
int Studio::Softer::Application::exec()
{
	//Verifies if the application is already launched.
	if (m_sharedMemory->create(sizeof(int)) == false)
	{
		exit(1);
	}

	//New instance of QSplashScreen;
	m_splashScreen = new QSplashScreen();

	//Shows the splash screen and version of an application, if getSplashScreenPath() is not empty.
	if (!getSplashScreenPath().isEmpty())
	{
		QPixmap pixmap(getSplashScreenPath());
		m_splashScreen->setPixmap(pixmap);
		m_splashScreen->showMessage("Version: " + getApplicationVersion(),
			Qt::AlignBottom | Qt::AlignRight, QColor(255, 255, 255));
		m_splashScreen->show();
	}

	//Thread for an process.
	m_application->processEvents();

	//Creates and Shows the main window.
	Windows::Window window(m_menuBar);
	window.setOrganizationName(getOrganizationName());
	window.setapplicationName(getApplicationName());
	window.setWindowTitle(getApplicationName());
	if (!getApplicationIconPath().isEmpty())
		window.setIcon(getApplicationIconPath());
	else
		window.setIcon(":/Icons/icon.png");
	window.showWindow();
	if (!getSplashScreenPath().isEmpty()) m_splashScreen->finish(&window);
	
	return m_application->exec();
}


/**
* \brief Allows to get the path of application icon.
* \return The path of application icon.
*/
QString Studio::Softer::Application::getApplicationIconPath() const
{
	return m_appIconPath;
}


/**
* \brief Allows to get the application version.
* \return The application version.
*/
QString Studio::Softer::Application::getApplicationVersion() const
{
	return m_appVersion;
}


/**
* \brief Allows to get the organization domain.
* \return The organization domain.
*/
QString Studio::Softer::Application::getOrganizationDomain() const
{
	return m_orgDomain;
}


/**
* \brief Allows to get the application name.
* \return The application name.
*/
QString Studio::Softer::Application::getApplicationName() const
{
	return m_appName;
}


/**
* \brief Allows to get the organization name.
* \return The organization name.
*/
QString Studio::Softer::Application::getOrganizationName() const
{
	return m_orgName;
}


/**
* \brief Allows to get the path of splashscreen.
* \return The path of splashscreen.
*/
QString Studio::Softer::Application::getSplashScreenPath() const
{
	return m_splashPath;
}


/**
* \brief Allows to get the product type of an application.
* \return The product type.
*/
ProductType Studio::Softer::Application::getProductType() const
{
	return m_product;
}


/**
* \brief Allows to set the application name.
* \param appName The name of this application.
*/
void Studio::Softer::Application::setApplicationName(const QString& appName)
{
	m_appName = appName;
}


/**
* \brief Allows to set the application version.
* \param appVersion The version of this application.
*/
void Studio::Softer::Application::setApplicationVersion(const QString& appVersion)
{
	m_appVersion = appVersion;
}


/**
* \brief Allows to set the organization name.
* \param orgName The organization name of this application.
*/
void Studio::Softer::Application::setOrganizationName(const QString& orgName)
{
	m_orgName = orgName;
}


/**
* \brief Allows to set the organization domain of an application.
* \param orgDomain The organization doamin of an application.
*/
void Studio::Softer::Application::setOrganizationDomain(const QString& orgDomain)
{
	m_orgDomain = orgDomain;
}


/**
* \brief Allows to set the splash screen path.
* \param splashPath The splash screen path.
*/
void Studio::Softer::Application::setSplashScreenPath(const QString& splashPath)
{
	m_splashPath = splashPath;
}


/**
* \brief Allows to set the icon path for an application.
* \param iconPath The icon path.
*/
void Studio::Softer::Application::setApplicationIconPath(const QString& iconPath)
{
	m_appIconPath = iconPath;
}
