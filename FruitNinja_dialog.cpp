#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    karpuzOlusturmaZamani->start(400);
    connect(karpuzOlusturmaZamani,&QTimer::timeout,this,&Dialog::karpuzOlustur);


    karpuzKonumZamani->start(25);
    connect(karpuzKonumZamani,&QTimer::timeout,this,&Dialog::karpuzKonum);

    sureazalt->start(1000);
    connect(sureazalt,&QTimer::timeout,this,&Dialog::oyunSuresi);
    ui->labelSureSayi->setText(QString::number(oyunsuresi));

    arkaplan->setStyleSheet("border-image:url(:/Resimler/images/back.jpg);");
    arkaplan->setGeometry(0,100,1400,700);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::karpuzOlustur()
{
    QFile konumdosyasi(":/Konum-Skor/konumlar.txt");

    if(!konumdosyasi.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Hata",konumdosyasi.errorString());
        return;
    }

    QTextStream stream(&konumdosyasi);
    QList <QString> konumListesi=stream.readAll().split("\r\n");

    int rastgeleKarpuz=rand()%konumListesi.count();

    QPushButton *karpuz=new QPushButton(this);
    karpuz->setStyleSheet("border-image:url(:/Resimler/images/1.png);");
    karpuz->setGeometry(konumListesi[rastgeleKarpuz].split(" ")[0].toInt(),konumListesi[rastgeleKarpuz].split(" ")[1].toInt(),80,80);
    karpuzListesi.push_back(karpuz);
    karpuz->show();

    connect(karpuz,&QPushButton::clicked,this,&Dialog::karpuzKes);

    konumdosyasi.close();
}

void Dialog::karpuzKonum()
{
    int karpuzSayisi=karpuzListesi.count();
    int i=0;

    for(i=0;i<karpuzSayisi;i++)
    {
        if(karpuzListesi[i]->y()>=750)
        {
            kacirilanKarpuzSayaci++;
            ui->labelKacirilanKarpuzSayi->setText(QString::number(kacirilanKarpuzSayaci));
            kacirilanKarpuzListesi.push_back(karpuzListesi[i]);
        }
        else
        {
            karpuzListesi[i]->setGeometry(karpuzListesi[i]->x(),karpuzListesi[i]->y()+3,karpuzListesi[i]->width(),karpuzListesi[i]->height());
        }
    }

    for(QPushButton *kacirilanKarpuz: kacirilanKarpuzListesi)
    {
        karpuzListesi.removeOne(kacirilanKarpuz);
    }

    kacirilanKarpuzListesi.clear();
}

void Dialog::karpuzKes()
{
    kesilenKarpuzSayaci++;
    QPushButton *butonKarpuz=qobject_cast <QPushButton*>(sender());
    QPushButton *butonKesilenKarpuz=new QPushButton(this);

    QString guncelStyle=butonKarpuz->styleSheet();

    if(guncelStyle=="border-image:url(:/Resimler/images/1.png);")
    {
        butonKesilenKarpuz->setStyleSheet("border-image:url(:/Resimler/images/2.png);");
    }

    butonKesilenKarpuz->setGeometry(butonKarpuz->x(),butonKarpuz->y(),80,80);
    butonKarpuz->deleteLater();
    karpuzListesi.removeOne(sender());
    butonKesilenKarpuz->show();

    ui->labelKesilenKarpuzSayi->setText(QString::number(kesilenKarpuzSayaci));

    QTimer *kesilenKarpuzZamani = new QTimer(this);
    kesilenKarpuzZamani->start(1000);
    kesilenKarpuzZamani->setSingleShot(true);
    connect(kesilenKarpuzZamani, &QTimer::timeout, [=]() {
        butonKesilenKarpuz->deleteLater();
    });
}

void Dialog::oyunSuresi()
{
    oyunsuresi=oyunsuresi-1;

    ui->labelSureSayi->setText(QString::number(oyunsuresi));

    if(oyunsuresi==0)
    {
        karpuzOlusturmaZamani->stop();
        karpuzKonumZamani->stop();
        sureazalt->stop();

        QFile skordosyasi("C:/Users/okqn/Documents/Qt Creator/22100011067_Odev1/skorlar.txt");

        if(!skordosyasi.open(QIODevice::ReadOnly))
        {
            QMessageBox::warning(this,"Hata",skordosyasi.errorString());
            return;
        }

        int maxSkor=0;

        QTextStream stream(&skordosyasi);
        QList <QString> skorListesi=stream.readAll().split("\n");

        foreach(QString skor, skorListesi)
        {
            if(skor.toInt()>maxSkor)
            {
                maxSkor=skor.toInt();
            }
        }

        skordosyasi.close();

        QFile skordosyasi1("C:/Users/okqn/Documents/Qt Creator/22100011067_Odev1/skorlar.txt");

        if (!skordosyasi1.open(QIODevice::Append))
        {
            QMessageBox::warning(this, "Error!", skordosyasi1.errorString());
            return;
        }

        QTextStream stream1(&skordosyasi1);
        stream1 << "\n"+ QString::number(kesilenKarpuzSayaci);
        skordosyasi1.close();

        if (kesilenKarpuzSayaci > maxSkor) {
            QMessageBox::information(this, "Bilgi!", "Oyun Bitti! Tebrikler Maximum Skor Sizde!\nKesilen Meyve Sayısı: "+
                                                         QString::number(kesilenKarpuzSayaci)+"\nKaçırılan Meyve Sayısı: "+
                                                         QString::number(kacirilanKarpuzSayaci)+"\nMaximum Skor: "+
                                                         QString::number(kesilenKarpuzSayaci),"Tamam");

            close();
        } else {
            QMessageBox::information(this, "Bilgi!", "Oyun Bitti! Maximum Skoru Geçemediniz!\nKesilen Meyve Sayısı: "+
                                                         QString::number(kesilenKarpuzSayaci)+"\nKaçırılan Meyve Sayısı: "+
                                                         QString::number(kacirilanKarpuzSayaci)+"\nMaximum Skor: "+
                                                         QString::number(maxSkor), "Tamam");
            close();
        }
    }

}
