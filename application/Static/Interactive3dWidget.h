#ifndef INTERACTIVE3DWIDGET_H
#define INTERACTIVE3DWIDGET_H

#include <QWidget>
#include <QList>
#include <QVector3D>
#include <QListWidget>

namespace Qt3DExtras {
    class Qt3DWindow;
    class QPhongMaterial;
    class QSphereMesh;
}

namespace Qt3DCore {
    class QEntity;
    class QTransform;
}

namespace Qt3DRender {
    class QMesh;
    class QObjectPicker;
    class QPickEvent;
}

class Interactive3DWidget : public QWidget {
    Q_OBJECT

public:
    explicit Interactive3DWidget(QWidget *parent = nullptr);

private slots:
    void onObjectClicked(Qt3DRender::QPickEvent *event);
    void deleteSelectedMarker();
    void savePositions();
    void loadPositions();

private:
    void addMarker(const QVector3D &position);
    QString positionToString(const QVector3D &position) const;
    QVector3D stringToPosition(const QString &str) const;

    Qt3DExtras::Qt3DWindow *view;
    Qt3DCore::QEntity *rootEntity;
    Qt3DCore::QEntity *headEntity;
    Qt3DCore::QTransform *transform;

    QList<QVector3D> markedPositions;         // Stores marked positions
    QList<Qt3DCore::QEntity *> markers;       // Stores marker entities
    QListWidget *markedPositionsList;         // UI to display positions
};

#endif // INTERACTIVE3DWIDGET_H
