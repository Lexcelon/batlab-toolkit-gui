#ifndef QTAUTOUPDATER_ADMINAUTHORIZATION_P_H
#define QTAUTOUPDATER_ADMINAUTHORIZATION_P_H

#include "qtautoupdatergui_global.h"
#include "../autoupdatercore/adminauthoriser.h"

namespace QtAutoUpdater
{

class AdminAuthorization : public AdminAuthoriser
{
public:
    bool hasAdminRights() override;
	bool executeAsAdmin(const QString &program, const QStringList &arguments) override;
};

}

#endif // QTAUTOUPDATER_ADMINAUTHORIZATION_P_H
