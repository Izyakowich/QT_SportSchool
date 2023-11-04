#ifndef SECTION_H
#define SECTION_H

#include <QWidget>

namespace Ui {
class section;
}

class section : public QWidget
{
    Q_OBJECT

public:
    explicit section(QWidget *parent = nullptr);
    ~section();

private:
    Ui::section *ui;
};

#endif // SECTION_H
