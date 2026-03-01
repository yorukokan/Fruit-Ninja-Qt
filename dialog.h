/*    Ad: Okan
   Soyad: YÖRÜK
      No: 22100011067 */


#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QFile>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QList>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

// Fonksiyonlar
private slots:
    void karpuzOlustur();
    void karpuzKonum();
    void karpuzKes();
    void oyunSuresi();

// Gerekli Tanımlamalar
private:
    Ui::Dialog *ui;

    int oyunsuresi=30;
    int kesilenKarpuzSayaci=0;
    int kacirilanKarpuzSayaci=0;

    QList <QPushButton*> karpuzListesi;
    QList <QPushButton*> kacirilanKarpuzListesi;

    QLabel *arkaplan=new QLabel(this);

    QTimer *sureazalt=new QTimer(this);
    QTimer *karpuzOlusturmaZamani=new QTimer(this);
    QTimer *karpuzKonumZamani=new QTimer(this);

};
#endif // DIALOG_H
