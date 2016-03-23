#pragma once

#include <memory>

#include <QMainWindow>


class Ui_Viewer;

namespace widgetzeug
{
    class MessageStatusWidget;
    class MessageWidget;
    class ScriptPromptWidget;
}

namespace gloperate
{
    class ResourceManager;
    class PluginManager;
    class Painter;
    class ImageExporter;
}

namespace gloperate_qt
{
    class QtOpenGLWindow;
    class DefaultMapping;
    class ScriptEnvironment;
    class ViewerApi;
    class PluginApi;
}


class Viewer : public QMainWindow
{
    Q_OBJECT


public:
    Viewer(QWidget * parent = nullptr, Qt::WindowFlags flags = 0);
    virtual ~Viewer();

    gloperate_qt::QtOpenGLWindow * canvas() const;

    void setPainter(gloperate::Painter & painter);

    gloperate::ResourceManager * resourceManager();
protected:
    void setupMessageWidgets();
    void setupPropertyWidget();
    void setupCanvas();

    void deinitializePainter();

protected:
    std::unique_ptr<gloperate::ResourceManager>      m_resourceManager;

    std::unique_ptr<gloperate::Painter>              m_painter;
    std::unique_ptr<gloperate_qt::DefaultMapping>    m_mapping;

    std::unique_ptr<gloperate_qt::QtOpenGLWindow>    m_canvas;
    std::unique_ptr<widgetzeug::MessageStatusWidget> m_messagesStatus;
    std::unique_ptr<widgetzeug::MessageWidget>       m_messagesLog;

    QDockWidget                                    * m_messagLogDockWidget;
    QDockWidget                                    * m_propertyDockWidget;
};
