#ifndef QTAUTOUPDATER_UPDATECONTROLLER_P_H
#define QTAUTOUPDATER_UPDATECONTROLLER_P_H

#include "qtautoupdatergui_global.h"
#include "updatecontroller.h"
#include "updateinfodialog_p.h"
#include "progressdialog_p.h"

#include "../autoupdatercore/updater.h"
#include "../autoupdatercore/simplescheduler_p.h"

#include <QtCore/QPointer>

namespace QtAutoUpdater
{

class UpdateControllerPrivate
{
public:
	typedef QPair<UpdateController::DisplayLevel, bool> UpdateTask;

	static QIcon getUpdatesIcon();

	UpdateController *q;

	QPointer<QWidget> window;

	UpdateController::DisplayLevel displayLevel;
	bool running;
	Updater *mainUpdater;
	bool runAdmin;
	bool adminUserEdit;
	QStringList runArgs;
	bool detailedInfo;

	QPointer<ProgressDialog> checkUpdatesProgress;
	bool wasCanceled;

	SimpleScheduler *scheduler;

	UpdateControllerPrivate(UpdateController *q_ptr, QWidget *window);
	UpdateControllerPrivate(UpdateController *q_ptr, const QString &toolPath, QWidget *window);
	~UpdateControllerPrivate();
};

}

#endif // QTAUTOUPDATER_UPDATECONTROLLER_P_H
