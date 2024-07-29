#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <Processing.NDI.Lib.h>

class NDIFinder : public QObject {
    Q_OBJECT

public:
    NDIFinder(QObject* parent = nullptr) : QObject(parent) {
        // Initialize NDI
        if (!NDIlib_initialize()) {
            qCritical() << "Failed to initialize NDI.";
            QCoreApplication::exit(1);
        }

        // Create NDI finder
        m_ndiFinder = NDIlib_find_create_v2();
        if (!m_ndiFinder) {
            qCritical() << "Failed to create NDI finder.";
            QCoreApplication::exit(1);
        }

        // Connect timer to search function
        connect(&m_timer, &QTimer::timeout, this, &NDIFinder::searchForSource);
        m_timer.start(5000); // Search every 5 seconds
    }

    ~NDIFinder() {
        // Cleanup NDI resources
        if (m_ndiFinder)
            NDIlib_find_destroy(m_ndiFinder);
        NDIlib_destroy();
    }

private slots:
    void searchForSource() {
        // Check for sources
        if (!NDIlib_find_wait_for_sources(m_ndiFinder, 5000 /* milliseconds */)) {
            qDebug() << "No change to the sources found.";
            return;
        }

        // Get the updated list of sources
        uint32_t no_sources = 0;
        const NDIlib_source_t* p_sources = NDIlib_find_get_current_sources(m_ndiFinder, &no_sources);

        // Search for the desired source
        const char* desiredIpAddress = "131.246.110.186"; // Modify this to your desired IP address
        const uint16_t desiredPort = 49265; // Modify this to your desired port
        for (uint32_t i = 0; i < no_sources; i++) {
            if (strcmp(p_sources[i].connection_address, desiredIpAddress) == 0 &&
                p_sources[i].connection_port == desiredPort) {
                qDebug() << "Desired source found:" << p_sources[i].p_ndi_name;
                QCoreApplication::quit();
                return;
            }
        }

        qDebug() << "Desired source not found.";
    }

private:
    NDIlib_find_instance_t m_ndiFinder = nullptr;
    QTimer m_timer;
};

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    NDIFinder ndiFinder;

    return app.exec();
}

#include "main.moc"
