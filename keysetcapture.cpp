#include "keysetcapture.h"
#include "ui_keysetcapture.h"

#include <QDebug>

KeySetCapture::KeySetCapture(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeySetCapture)
{
    ui->setupUi(this);

    ui->editKey->setText("No Key Set");

    installEventFilter(this);
}

KeySetCapture::~KeySetCapture()
{
    delete ui;
}

/**
 * @brief KeySetCapture::eventFilter
 * @param e
 * @return
 */
bool KeySetCapture::eventFilter(QObject *, QEvent *e)
{
    if (e->type() == QEvent::KeyRelease && waitingOnInput)
    {
        mKey = static_cast<QKeyEvent *>(e);

        if (mKey->key() == Qt::Key_Shift ||
            mKey->key() == Qt::Key_Control ||
            mKey->key() == Qt::Key_Escape)
        {
            return false;
        }

        ui->editDescription->setReadOnly(false);

        KeyCode = mKey->key();
        KeyText = mKey->text();
        KeyDescription = ui->editDescription->text();

        ui->editKey->setText(QString("Key Set: %1").arg(mKey->text().toUpper()));
        waitingOnInput = false;
        ui->buttonKey->setEnabled(true);
        ui->editKey->setStyleSheet(QString("QLineEdit { background: green; padding: 10 20 }"));

        enteredKey = true;
    }

    return false;
}

/**
 * @brief KeySetCapture::on_buttonKey_clicked
 */
void KeySetCapture::on_buttonKey_clicked()
{
    if (waitingOnInput)
    {
        return;
    }

    ui->editDescription->setReadOnly(true);

    mKey = NULL;
    ui->editKey->setText("Waiting for Key Press");
    waitingOnInput = true;
    ui->buttonKey->setEnabled(false);
    ui->editKey->setStyleSheet(QString("QLineEdit { background: red; padding: 10 20 }"));

    enteredKey = false;

    KeyCode = -1;
    KeyText = "";
    KeyDescription = "";

    ui->buttonKey->setFocus();
}

/**
 * @brief KeySetCapture::on_buttonBox_clicked
 * @param button
 */
void KeySetCapture::on_buttonBox_clicked(QAbstractButton *button)
{
    if((QPushButton *)button == ui->buttonBox->button(QDialogButtonBox::Ok))
    {
        KeyDescription = ui->editDescription->text();
    }
}
