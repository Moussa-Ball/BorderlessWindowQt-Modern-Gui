#ifndef __WINDOW__H_
#define __WINDOW__H_

#include <QtWidgets>
#include <QString>

class Window : public QMainWindow
{
    Q_OBJECT
public:
    Window();
    // ReSharper disable once CppHidingFunction
    void show();

protected:
    void closeEvent(QCloseEvent *event) override;
#ifdef Q_OS_WIN
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
    void mousePressEvent(QMouseEvent *event) override;
    void changeEvent(QEvent* e) override;
#endif
private slots:
    void slot_show_system_menu() const;
    void slot_minimized();
    void slot_maximized();
    void slot_restored();
    void slot_closed();

private:
#ifdef Q_OS_WIN
    auto set_borderless(bool enabled) const -> void;
    QPushButton *minimize_button_;
    QPushButton *maximize_button_;
    QPushButton *restore_button_;
    QPushButton *close_Button_;
    QWidget *title_bar_widget_;
    QPushButton *icon_button_;
    QMenuBar *menu_bar_;
    QPoint mouse_point_;
#endif
};

#endif
