#ifndef INTERACTIVE3DWIDGET_H
#define INTERACTIVE3DWIDGET_H

#include <QWidget>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QConeMesh>
#include <QVector3D>
#include <QLineEdit>
#include <QTimer>
#include <QTcpSocket>

class Interactive3DWidget : public QWidget {
    Q_OBJECT

public:
    explicit Interactive3DWidget(QWidget *parent = nullptr);

private slots:
    void setMarkedPoint();                      // Slot to update the marked point
    void startStreaming();                      // Slot to start receiving position data
    void readPositionData();                    // Slot to read position data from the stream
    void updateAlignmentVisualization();        // Slot to update alignment visualization and arrow

private:
    // Helper methods
    void createArrow();                         // Method to create the arrow visualization
    void updateArrow(const QVector3D &alignmentVector); // Method to update the arrow's position, rotation, and scale
    QVector3D parsePoint(const QString &str) const; // Helper to parse 3D points from strings

    // 3D Visualization Components
    Qt3DExtras::Qt3DWindow *view;               // The 3D view window
    Qt3DCore::QEntity *rootEntity;              // Root entity of the 3D scene
    Qt3DCore::QEntity *headEntity;              // Entity for the head model
    Qt3DCore::QTransform *transform;           // Transform for the head model

    Qt3DCore::QEntity *trackedObject;           // Entity for the tracked object
    Qt3DCore::QTransform *trackedTransform;     // Transform for the tracked object

    Qt3DCore::QEntity *arrowEntity;             // Entity for the arrow
    Qt3DCore::QTransform *arrowTransform;       // Transform for the arrow

    // Data
    QVector3D headCenter;                       // Center of the head
    QVector3D markedPoint;                      // Marked point on the head
    QVector3D trackedPoint;                     // Current position of the tracked object
    QVector3D latestPosition;                   // Latest position from the data stream

    // UI Components
    QLineEdit *markerInput;                     // Input field for setting the marked point
    QTimer *updateTimer;                        // Timer to update visualization

    // Streaming Components
    QTcpSocket *socket;                         // Socket for streaming position data
};

#endif // INTERACTIVE3DWIDGET_H
