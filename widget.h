#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include "gpio.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void updateButtonState();
    void updateCountState(short pin);
    void getFlank();
private:
    QLabel* counterlabel = new QLabel("Counter: " + QString::number(m_cntnum), this);
    QList<QLabel*> m_input_display;
    QTimer* m_timer;
    gpio* m_gpio;
    int m_cntnum;
    QList<bool> m_statehistory;
    QList<bool> m_states;
};

#endif // WIDGET_H
