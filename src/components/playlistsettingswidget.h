#ifndef PLAYLISTSETTINGSWIDGET_H
#define PLAYLISTSETTINGSWIDGET_H

#include <QWidget>
#include "batlabcore/cellplaylist.h"

class PlaylistSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlaylistSettingsWidget(CellPlaylist playlist, QWidget *parent = nullptr);

signals:

public slots:
};

#endif // PLAYLISTSETTINGSWIDGET_H
