#include <QApplication>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DCore/QEntity>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DRender/QMesh>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QSphereMesh>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTimer>
#include <QVector3D>
#include <QMatrix4x4>
#include <QTcpSocket> // For position data streaming

class Interactive3DWidget : public QWidget {
    Q_OBJECT

public:
    Interactive3DWidget(QWidget *parent = nullptr)
        : QWidget(parent),
          view(new Qt3DExtras::Qt3DWindow()),
          rootEntity(new Qt3DCore::QEntity()),
          headEntity(new Qt3DCore::QEntity(rootEntity)),
          transform(new Qt3DCore::QTransform()),
          calibrationMatrix(QMatrix4x4()) {

        // Set up the 3D window
        view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));
        view->setRootEntity(rootEntity);

        // Create the head mesh
        Qt3DRender::QMesh *mesh = new Qt3DRender::QMesh();
        mesh->setSource(QUrl::fromLocalFile("path/to/your/head_model.obj"));

        // Set up the transform
        transform->setScale(1.0f);

        // Add material
        Qt3DExtras::QPhongMaterial *material = new Qt3DExtras::QPhongMaterial();
        material->setDiffuse(QColor(QRgb(0xbeb32b)));

        // Add components to the head entity
        headEntity->addComponent(mesh);
        headEntity->addComponent(transform);
        headEntity->addComponent(material);

        // Add a marker for streaming data (tracked object)
        trackedObject = new Qt3DCore::QEntity(rootEntity);
        Qt3DExtras::QSphereMesh *sphere = new Qt3DExtras::QSphereMesh();
        sphere->setRadius(0.05f); // Small tracked object
        Qt3DExtras::QPhongMaterial *sphereMaterial = new Qt3DExtras::QPhongMaterial();
        sphereMaterial->setDiffuse(QColor(Qt::blue));
        trackedTransform = new Qt3DCore::QTransform();
        trackedObject->addComponent(sphere);
        trackedObject->addComponent(sphereMaterial);
        trackedObject->addComponent(trackedTransform);

        // Set up the camera
        Qt3DRender::QCamera *camera = view->camera();
        camera->lens()->setPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
        camera->setPosition(QVector3D(0, 0, 5.0f));
        camera->setViewCenter(QVector3D(0, 0, 0));

        // Layout for 3D view and UI controls
        QWidget *container = QWidget::createWindowContainer(view, this);
        QVBoxLayout *layout = new QVBoxLayout(this);

        // Add 3D view
        layout->addWidget(container);

        // Add calibration UI
        QLabel *calibrationLabel = new QLabel("Enter Calibration Points (x,y,z):", this);
        layout->addWidget(calibrationLabel);

        calibrationInput = new QLineEdit(this);
        layout->addWidget(calibrationInput);

        QPushButton *calibrateButton = new QPushButton("Calibrate", this);
        layout->addWidget(calibrateButton);

        connect(calibrateButton, &QPushButton::clicked, this, &Interactive3DWidget::calibrateModel);

        // Add a start streaming button
        QPushButton *startStreamButton = new QPushButton("Start Streaming", this);
        layout->addWidget(startStreamButton);

        connect(startStreamButton, &QPushButton::clicked, this, &Interactive3DWidget::startStreaming);

        setLayout(layout);

        // Timer for updating tracked object position
        updateTimer = new QTimer(this);
        connect(updateTimer, &QTimer::timeout, this, &Interactive3DWidget::updateTrackedObject);
    }

private slots:
    void calibrateModel() {
        // Example: Enter two points "1,0,0;0,1,0" to map real-world points to model points
        QString input = calibrationInput->text();
        QStringList points = input.split(";");
        if (points.size() < 2) {
            qWarning() << "Please enter at least two points.";
            return;
        }

        QVector3D realPoint1 = parsePoint(points[0]);
        QVector3D realPoint2 = parsePoint(points[1]);
        QVector3D modelPoint1(1, 0, 0); // Example fixed points
        QVector3D modelPoint2(0, 1, 0);

        QMatrix4x4 scaleMatrix;
        scaleMatrix.scale((realPoint2 - realPoint1).length() / (modelPoint2 - modelPoint1).length());

        QMatrix4x4 translationMatrix;
        translationMatrix.translate(realPoint1 - modelPoint1);

        calibrationMatrix = translationMatrix * scaleMatrix;
        qDebug() << "Calibration Matrix:" << calibrationMatrix;
    }

    void startStreaming() {
        // Example: Connect to a socket for real-time position data
        socket = new QTcpSocket(this);
        socket->connectToHost("127.0.0.1", 12345); // Adjust IP/port as needed

        if (socket->waitForConnected()) {
            connect(socket, &QTcpSocket::readyRead, this, &Interactive3DWidget::readPositionData);
            updateTimer->start(16); // Refresh rate: ~60fps
        } else {
            qWarning() << "Could not connect to the position data stream.";
        }
    }

    void readPositionData() {
        while (socket->canReadLine()) {
            QString line = socket->readLine().trimmed();
            QVector3D position = parsePoint(line);
            latestPosition = calibrationMatrix * position;
        }
    }

    void updateTrackedObject() {
        if (!latestPosition.isNull()) {
            trackedTransform->setTranslation(latestPosition);
        }
    }

private:
    QVector3D parsePoint(const QString &str) const {
        QStringList coords = str.split(",");
        if (coords.size() == 3) {
            return QVector3D(coords[0].toFloat(), coords[1].toFloat(), coords[2].toFloat());
        }
        return QVector3D();
    }

    Qt3DExtras::Qt3DWindow *view;
    Qt3DCore::QEntity *rootEntity;
    Qt3DCore::QEntity *headEntity;
    Qt3DCore::QTransform *transform;
    Qt3DCore::QEntity *trackedObject;
    Qt3DCore::QTransform *trackedTransform;

    QLineEdit *calibrationInput;
    QMatrix4x4 calibrationMatrix;

    QTcpSocket *socket = nullptr;
    QTimer *updateTimer;
    QVector3D latestPosition;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Interactive3DWidget widget;
    widget.resize(800, 600);
    widget.show();

    return app.exec();
}
