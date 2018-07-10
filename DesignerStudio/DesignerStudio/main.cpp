#include "Application.h"

using namespace Studio::Softer;

int main(int argc, char *argv[])
{
	//New Instance of application.
	Application app(argc, argv);

	//Indicating of product type
	auto product = Designer;

	//Set informations for the application.
	app.setProductType(product);
	app.setApplicationVersion("1.0.0.0");
	app.setOrganizationName("StudioSofter");
	app.setApplicationName("Designer Studio");
	app.setApplicationIconPath(":/Icons/DesignerStudio.ico");
	app.setOrganizationDomain("https://www.studiosofter.com");
	app.setSplashScreenPath(":/SplashScreen/designer-studio-splash.png");

	//Execute the application.
	return app.exec();
}