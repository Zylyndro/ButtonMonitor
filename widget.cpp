#include "config.h"
#include "widget.h"
#include <QGridLayout>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    // Widget s &QTimer::timeouttructure
    setWindowTitle("ButtonMonitor");
    resize(300, 100);
    QGridLayout* grid = new QGridLayout(this);
    setLayout(grid);

    // Labels for input pins / buttons
    int column = 0;
    m_cntnum = 0;
    m_statehistory = {0, 0, 0};
    m_states = {0, 0, 0};

    for (auto pin : BUTTONS) {
        // pin numbers
        QLabel* label = new QLabel("GPIO " + QString::number(pin), this);
        label->setAlignment(Qt::AlignCenter);
        grid->addWidget(label, 0, column); // links oben (1. Zeile / 1. - 3. Spalte)

        // pin states
        QLabel* state = new QLabel("0", this);
        state->setAlignment(Qt::AlignCenter);
        grid->addWidget(state, 1, column++); // links oben (2. Zeile / 1. - 3. Spalte)
        m_input_display.push_back(state);
    }

    counterlabel->setAlignment(Qt::AlignCenter);
    grid->addWidget(counterlabel, 2, 1);

    // initialize hardware
    m_gpio = new gpio();

    m_timer = new QTimer(this);
    // Connect QTimer (Signal) with GUI function (Slot) "update display"
    connect(m_timer, &QTimer::timeout, this, &Widget::updateButtonState);
    connect(m_timer, &QTimer::timeout, this, &Widget::getFlank);
    m_timer->start(T_UPDATE);

    counterlabel->setText("Counter: " + QString::number(m_cntnum));
}

Widget::~Widget()
{

}

void Widget::getFlank()
{
    int cnt = 0;
    for (auto pin : BUTTONS)
    {
        m_states[cnt] = !m_gpio->get(pin);
        cnt++;
    }
    cnt = 0;
    for (auto pin : BUTTONS)
    {
        if(m_statehistory[cnt] !=  m_states[cnt]) updateCountState(pin);
        cnt++;
    }
}

void Widget::updateCountState(short pin)
{
  if(pin == 22)  m_cntnum++;
  if(pin == 17)  m_cntnum--;
  if(pin == 27)  m_cntnum = 0;

  counterlabel->setText("Counter: " + QString::number(m_cntnum / 2));

  for(int cnt = 0; cnt < 3; cnt++) m_statehistory[cnt] = m_states[cnt];
}

void Widget::updateButtonState()
{
    int n = 0;
    for (auto pin : BUTTONS)
    {
        int state = !m_gpio->get(pin);
        m_input_display[n++]->setText(QString::number(state));
    }
}
