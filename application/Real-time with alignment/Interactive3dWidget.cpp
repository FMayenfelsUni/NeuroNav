#include <QApplication>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DCore/QEntity>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DRender/QMesh>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QConeMesh>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTimer>
#include <QVector3D>
#include <QQuaternion>
#include <QTcpSocket>

class Interactive3DWidget : public QWidget {
    Q_OBJECT

public:
    Interactive3DWidget(QWidget *parent = nullptr)
        : QWidget(parent),
          view(new Qt3DExtras::Qt3DWindow()),
          rootEntity(new Qt3DCore::QEntity()),
          headEntity(new Qt3DCore::QEntity(rootEntity)),
          transform(new Qt3DCore::QTransform()),
          trackedPoint(QVector3D()),
          markedPoint(QVector3D(0, 1, 0)), // Default marked point
          headCenter(QVector3D(0, 0, 0)) {

        // Set up the 3D window
        view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));
        view->setRootEntity(rootEntity);

        // Create the head mesh
        Qt3DRender::QMesh *mesh = new Qt3DRender::QMesh();
        mesh->setSource(QUrl::fromLocalFile("path/to/your/head_model.obj")); // Replace with your OBJ file path

        // Set up the transform
        transform->setScale(1.0f);

        // Add material
        Qt3DExtras::QPhongMaterial *material = new Qt3DExtras::QPhongMaterial();
        material->setDiffuse(QColor(QRgb(0xbeb32b)));

        // Add components to the head entity
        headEntity->addComponent(mesh);
        headEntity->addComponent(transform);
        headEntity->addComponent(material);

        // Add a marker for the tracked object
        trackedObject = new Qt3DCore::QEntity(rootEntity);
        Qt3DExtras::QSphereMesh *sphere = new Qt3DExtras::QSphereMesh();
        sphere->setRadius(0.05f); // Small tracked object
        Qt3DExtras::QPhongMaterial *sphereMaterial = new Qt3DExtras::QPhongMaterial();
        sphereMaterial->setDiffuse(QColor(Qt::blue));
        trackedTransform = new Qt3DCore::QTransform();
        trackedObject->addComponent(sphere);
        trackedObject->addComponent(sphereMaterial);
        trackedObject->addComponent(trackedTransform);

        // Create the arrow entity
        arrowEntity = new Qt3DCore::QEntity(rootEntity);
        createArrow();

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
        QLabel *markerLabel = new QLabel("Set Marked Point (x,y,z):", this);
        layout->addWidget(markerLabel);

        markerInput = new QLineEdit(this);
        layout->addWidget(markerInput);

        QPushButton *setMarkerButton = new QPushButton("Set Marked Point", this);
        layout->addWidget(setMarkerButton);

        connect(setMarkerButton, &QPushButton::clicked, this, &Interactive3DWidget::setMarkedPoint);

        QPushButton *startStreamButton = new QPushButton("Start Streaming", this);
        layout->addWidget(startStreamButton);

        connect(startStreamButton, &QPushButton::clicked, this, &Interactive3DWidget::startStreaming);

        setLayout(layout);

        // Timer for updating arrow and alignment
        updateTimer = new QTimer(this);
        connect(updateTimer, &QTimer::timeout, this, &Interactive3DWidget::updateAlignmentVisualization);
        updateTimer->start(16); // Refresh at ~60fps
    }

private slots:
    void setMarkedPoint() {
        QString input = markerInput->text();
        QStringList coords = input.split(",");
        if (coords.size() == 3) {
            markedPoint = QVector3D(coords[0].toFloat(), coords[1].toFloat(), coords[2].toFloat());
            qDebug() << "Marked Point Set To:" << markedPoint;
        } else {
            qWarning() << "Invalid Input: Please enter coordinates in x,y,z format.";
        }
    }

    void startStreaming() {
        // Example: Connect to a socket for real-time position data
        socket = new QTcpSocket(this);
        socket->connectToHost("127.0.0.1", 12345); // Adjust IP/port as needed

        if (socket->waitForConnected()) {
            connect(socket, &QTcpSocket::readyRead, this, &Interactive3DWidget::readPositionData);
        } else {
            qWarning() << "Could not connect to the position data stream.";
        }
    }

    void readPositionData() {
        while (socket->canReadLine()) {
            QString line = socket->readLine().trimmed();
            QVector3D position = parsePoint(line);
            latestPosition = position;
            trackedPoint = position;
            trackedTransform->setTranslation(trackedPoint);
        }
    }

    void updateAlignmentVisualization() {
        // Compute alignment vector
        QVector3D vectorA = markedPoint - headCenter;
        QVector3D vectorB = trackedPoint - headCenter;

        // Update alignment angle and vector
        float alignmentAngle = QVector3D::dotProduct(vectorA.normalized(), vectorB.normalized());
        QVector3D alignmentVector = vectorB - vectorA;

        qDebug() << "Alignment Angle (cos):" << alignmentAngle;

        // Update arrow visualization
        updateArrow(alignmentVector);

        // Center the scene on the tracked point
        view->camera()->setViewCenter(trackedPoint);
    }

private:
    QVector3D parsePoint(const QString &str) const {
        QStringList coords = str.split(",");
        if (coords.size() == 3) {
            return QVector3D(coords[0].toFloat(), coords[1].toFloat(), coords[2].toFloat());
        }
        return QVector3D();
    }

    void createArrow() {
        // Arrow shaft
        Qt3DExtras::QCylinderMesh *cylinder = new Qt3DExtras::QCylinderMesh();
        cylinder->setRadius(0.02f);
        cylinder->setLength(1.0f);

        Qt3DExtras::QPhongMaterial *shaftMaterial = new Qt3DExtras::QPhongMaterial();
        shaftMaterial->setDiffuse(QColor(Qt::green));

        Qt3DCore::QTransform *shaftTransform = new Qt3DCore::QTransform();

        Qt3DCore::QEntity *shaftEntity = new Qt3DCore::QEntity(arrowEntity);
        shaftEntity->addComponent(cylinder);
        shaftEntity->addComponent(shaftMaterial);
        shaftEntity->addComponent(shaftTransform);

        // Arrow head
        Qt3DExtras::QConeMesh *cone = new Qt3DExtras::QConeMesh();
        cone->setBottomRadius(0.04f);
        cone->setLength(0.2f);

        Qt3DExtras::QPhongMaterial *coneMaterial = new Qt3DExtras::QPhongMaterial();
        coneMaterial->setDiffuse(QColor(Qt::green));

        Qt3DCore::QTransform *coneTransform = new Qt3DCore::QTransform();
        coneTransform->setTranslation(QVector3D(0, 0.6f, 0)); // Offset to top of cylinder

        Qt3DCore::QEntity *coneEntity = new Qt3DCore::QEntity(arrowEntity);
        coneEntity->addComponent(cone);
        coneEntity->addComponent(coneMaterial);
        coneEntity->addComponent(coneTransform);

        // Store the arrow components for updates
        arrowTransform = new Qt3DCore::QTransform();
        arrowEntity->addComponent(arrowTransform);
    }

    void updateArrow(const QVector3D &alignmentVector) {
        float length = alignmentVector.length();
        QVector3D direction = alignmentVector.normalized();

        // Set arrow position (midpoint of vector)
        arrowTransform->setTranslation(headCenter + 0.5f * alignmentVector);

        // Adjust arrow orientation
        QQuaternion rotation = QQuaternion::fromDirection(direction, QVector3D(0, 1, 0));
        arrowTransform->setRotation(rotation);

        // Adjust arrow length
        arrowTransform->setScale3D(QVector3D(1, length, 1));
    }

private:
    Qt3DExtras::Qt3DWindow *view;
    Qt3DCore::QEntity *rootEntity;
    Qt3DCore::QEntity *headEntity;
    Qt3DCore::QTransform *transform;

    Qt3DCore::QEntity *trackedObject;
    Qt3DCore::QTransform *trackedTransform;

    Qt3DCore::QEntity *arrowEntity;
    Qt3DCore::QTransform *arrowTransform;

    QVector3D headCenter;
    QVector3D markedPoint;
    QVector3D trackedPoint;

    QTcpSocket *socket;
    QVector3D latestPosition;

    QTimer *updateTimer;

    QLineEdit *markerInput;
};
