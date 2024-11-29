#include <QApplication>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DCore/QEntity>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DRender/QMesh>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QObjectPicker>
#include <Qt3DRender/QPickEvent>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QTextStream>
#include <QUrl>

class Interactive3DWidget : public QWidget {
    Q_OBJECT

public:
    Interactive3DWidget(QWidget *parent = nullptr)
        : QWidget(parent),
          view(new Qt3DExtras::Qt3DWindow()),
          rootEntity(new Qt3DCore::QEntity()),
          headEntity(new Qt3DCore::QEntity(rootEntity)),
          transform(new Qt3DCore::QTransform()) {

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

        // Add object picker for interaction
        Qt3DRender::QObjectPicker *picker = new Qt3DRender::QObjectPicker();
        picker->setHoverEnabled(false);
        headEntity->addComponent(picker);

        connect(picker, &Qt3DRender::QObjectPicker::clicked, this, &Interactive3DWidget::onObjectClicked);

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

        // Add list widget for marked positions
        markedPositionsList = new QListWidget(this);
        layout->addWidget(markedPositionsList);

        // Add buttons for deleting and saving/loading
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        QPushButton *deleteButton = new QPushButton("Delete Marker", this);
        QPushButton *saveButton = new QPushButton("Save Positions", this);
        QPushButton *loadButton = new QPushButton("Load Positions", this);

        buttonLayout->addWidget(deleteButton);
        buttonLayout->addWidget(saveButton);
        buttonLayout->addWidget(loadButton);
        layout->addLayout(buttonLayout);

        // Connect buttons to slots
        connect(deleteButton, &QPushButton::clicked, this, &Interactive3DWidget::deleteSelectedMarker);
        connect(saveButton, &QPushButton::clicked, this, &Interactive3DWidget::savePositions);
        connect(loadButton, &QPushButton::clicked, this, &Interactive3DWidget::loadPositions);

        setLayout(layout);
    }

private slots:
    void onObjectClicked(Qt3DRender::QPickEvent *event) {
        if (!event)
            return;

        // Get the position of the intersection
        QVector3D intersection = event->worldIntersection();
        qDebug() << "Intersection Position: " << intersection;

        // Add a marker at the intersection point
        addMarker(intersection);

        // Add position to the list widget and internal storage
        markedPositions.append(intersection);
        markedPositionsList->addItem(positionToString(intersection));
    }

    void deleteSelectedMarker() {
        int currentRow = markedPositionsList->currentRow();
        if (currentRow >= 0 && currentRow < markedPositions.size()) {
            // Remove marker from the 3D scene
            delete markers[currentRow];
            markers.removeAt(currentRow);

            // Remove position from list widget and internal storage
            markedPositions.removeAt(currentRow);
            delete markedPositionsList->takeItem(currentRow);
        }
    }

    void savePositions() {
        QString filePath = QFileDialog::getSaveFileName(this, "Save Positions", "", "Text Files (*.txt)");
        if (filePath.isEmpty())
            return;

        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);
            for (const QVector3D &position : markedPositions) {
                stream << positionToString(position) << "\n";
            }
            file.close();
        }
    }

    void loadPositions() {
        QString filePath = QFileDialog::getOpenFileName(this, "Load Positions", "", "Text Files (*.txt)");
        if (filePath.isEmpty())
            return;

        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream stream(&file);
            markedPositions.clear();
            markers.clear();
            markedPositionsList->clear();

            while (!stream.atEnd()) {
                QString line = stream.readLine();
                QVector3D position = stringToPosition(line);
                addMarker(position);
                markedPositions.append(position);
                markedPositionsList->addItem(line);
            }
            file.close();
        }
    }

private:
    void addMarker(const QVector3D &position) {
        // Create a sphere to mark the position
        Qt3DCore::QEntity *marker = new Qt3DCore::QEntity(rootEntity);

        Qt3DExtras::QSphereMesh *sphere = new Qt3DExtras::QSphereMesh();
        sphere->setRadius(0.02f); // Small sphere

        Qt3DCore::QTransform *markerTransform = new Qt3DCore::QTransform();
        markerTransform->setTranslation(position);

        Qt3DExtras::QPhongMaterial *markerMaterial = new Qt3DExtras::QPhongMaterial();
        markerMaterial->setDiffuse(QColor(Qt::red));

        marker->addComponent(sphere);
        marker->addComponent(markerTransform);
        marker->addComponent(markerMaterial);

        markers.append(marker);
    }

    QString positionToString(const QVector3D &position) const {
        return QString("%1,%2,%3").arg(position.x()).arg(position.y()).arg(position.z());
    }

    QVector3D stringToPosition(const QString &str) const {
        QStringList parts = str.split(",");
        if (parts.size() == 3) {
            return QVector3D(parts[0].toFloat(), parts[1].toFloat(), parts[2].toFloat());
        }
        return QVector3D();
    }

    Qt3DExtras::Qt3DWindow *view;
    Qt3DCore::QEntity *rootEntity;
    Qt3DCore::QEntity *headEntity;
    Qt3DCore::QTransform *transform;

    QList<QVector3D> markedPositions;         // Stores marked positions
    QList<Qt3DCore::QEntity *> markers;       // Stores marker entities
    QListWidget *markedPositionsList;         // UI to display positions
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Interactive3DWidget widget;
    widget.resize(800, 600);
    widget.show();

    return app.exec();
}
