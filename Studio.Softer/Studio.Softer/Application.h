#ifndef __APPLICATION__H_
#define __APPLICATION__H_

#include "studiosofter_global.h"
#include "ProductType.h"

#include <QSplashScreen>
#include <QSharedMemory>
#include <QApplication>
#include <QMenuBar>

namespace Studio
{
	namespace Softer
	{
		class STUDIOSOFTER_EXPORT Application
		{
		public:
			Application(int argc, char *argv[]);
			void setProductType(const ProductType &product);
			void setApplicationName(const QString &appName);
			void setOrganizationName(const QString &orgName);
			void setSplashScreenPath(const QString &splashPath);
			void setApplicationIconPath(const QString &iconPath);
			void setOrganizationDomain(const QString &orgDomain);
			void setApplicationVersion(const QString &appVersion);
			int exec();

		private:
			QString getApplicationIconPath() const;
			QString getApplicationVersion() const;
			QString getOrganizationDomain() const;
			QString getOrganizationName() const;
			QString getSplashScreenPath() const;
			QString getApplicationName() const;
			ProductType getProductType() const;

			QSplashScreen *m_splashScreen;
			QSharedMemory *m_sharedMemory;
			QApplication *m_application;
			ProductType m_product;
			QString m_appIconPath;
			QString m_appVersion;
			QString m_splashPath;
			QString m_orgDomain;
			QMenuBar *m_menuBar;
			QString m_orgName;
			QString m_appName;
		};
	}
}

#endif