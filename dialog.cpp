/*    Ad: Okan
   Soyad: YÖRÜK
      No: 22100011067

    Youtube Videosu:
    https://www.youtube.com/watch?v=d_VwM6DwwfY&t=30s  */


#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    // Connect Fonksiyonu ile tanımlamalar

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

// Karpuz Oluşturma Fonksiyonu
void Dialog::karpuzOlustur()
{

    //Youtube videosu ile Resources dosyasından txt dosyasını alma
    QFile konumdosyasi(":/Konum-Skor/konumlar.txt");

    if(!konumdosyasi.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Hata",konumdosyasi.errorString());
        return;
    }

    //konumları okuyup ayırma işlemi (deepseek)
    QTextStream stream(&konumdosyasi);
    QList <QString> konumListesi=stream.readAll().split("\r\n");

    int rastgeleKarpuz=rand()%konumListesi.count();

    QPushButton *karpuz=new QPushButton(this);
    // resim gözükmediği için border image şeklinde tanımlama (Youtube)
    karpuz->setStyleSheet("border-image:url(:/Resimler/images/1.png);");
    karpuz->setGeometry(konumListesi[rastgeleKarpuz].split(" ")[0].toInt(),konumListesi[rastgeleKarpuz].split(" ")[1].toInt(),80,80);
    karpuzListesi.push_back(karpuz);
    karpuz->show();

    // function based tıklanınca kesme (GpDers)
    connect(karpuz,&QPushButton::clicked,this,&Dialog::karpuzKes);

    konumdosyasi.close();
}

// Karpuz Konumları Fonksiyonu
void Dialog::karpuzKonum()
{
    int karpuzSayisi=karpuzListesi.count();
    int i=0;

    for(i=0;i<karpuzSayisi;i++)
    {
        if(karpuzListesi[i]->y()>=720)
        {
            kacirilanKarpuzSayaci++;
            ui->labelKacirilanKarpuzSayi->setText(QString::number(kacirilanKarpuzSayaci));
            kacirilanKarpuzListesi.push_back(karpuzListesi[i]); //append çaılşmadığı için push_back (Youtube)
        }
        else
        {
            // asağı kayması için animasyon olmazsa kaymıyordu (Youtube)
            karpuzListesi[i]->setGeometry(karpuzListesi[i]->x(),karpuzListesi[i]->y()+4,karpuzListesi[i]->width(),karpuzListesi[i]->height());
        }
    }

    //kacirilan karpuzlar tekrardan olmaması için temizleme yapıyoruz
    for(QPushButton *kacirilanKarpuz: kacirilanKarpuzListesi)
    {
        karpuzListesi.removeOne(kacirilanKarpuz);
    }

    kacirilanKarpuzListesi.clear();
}

// Karpuz Kesme Fonksiyonu
void Dialog::karpuzKes()
{
    // butonu alıp yeni butonla değişmek için (Youtube)
    kesilenKarpuzSayaci++;
    QPushButton *butonKarpuz=qobject_cast <QPushButton*>(sender());
    QPushButton *butonKesilenKarpuz=new QPushButton(this);

    QString guncelStyle=butonKarpuz->styleSheet();
    // tıklandığında resim değişmesi için kontrol (border-image diger turlu calısmıyor resim gelmiyor)
    if(guncelStyle=="border-image:url(:/Resimler/images/1.png);")
    {
        butonKesilenKarpuz->setStyleSheet("border-image:url(:/Resimler/images/2.png);");
    }

    // kesilen karpuz resmi için eski konum secildi
    butonKesilenKarpuz->setGeometry(butonKarpuz->x(),butonKarpuz->y(),80,80);
    butonKarpuz->deleteLater();
    //karpuzu listeden silip kesilmis karpuz resmi gösterme
    karpuzListesi.removeOne(sender());
    butonKesilenKarpuz->show();

    ui->labelKesilenKarpuzSayi->setText(QString::number(kesilenKarpuzSayaci));

    // kesilen karpuzun da silinmesi için timer
    QTimer *kesilenKarpuzZamani = new QTimer(this);
    kesilenKarpuzZamani->start(1500);
    //bu kısımda calışmadı (Youtube) üzerinden baktım
    kesilenKarpuzZamani->setSingleShot(true);
    connect(kesilenKarpuzZamani, &QTimer::timeout, [=]() {
        butonKesilenKarpuz->deleteLater();
    });
}

// Oyun Süresi ve Sonu Fonksiyonu
void Dialog::oyunSuresi()
{
    oyunsuresi=oyunsuresi-1;

    ui->labelSureSayi->setText(QString::number(oyunsuresi));

    if(oyunsuresi==0)
    {
        karpuzOlusturmaZamani->stop();
        karpuzKonumZamani->stop();
        sureazalt->stop();

        // Resources içinde çalışmadığı için bu şekilde aldım
        QFile skordosyasi("C:/Users/okqn/Documents/Qt Creator/22100011067_Odev1/skorlar.txt");

        if(!skordosyasi.open(QIODevice::ReadOnly))
        {
            QMessageBox::warning(this,"Hata",skordosyasi.errorString());
            return;
        }

        int maxSkor=0;

        // tüm skorları okuyup listede tutma
        QTextStream stream(&skordosyasi);
        QList <QString> skorListesi=stream.readAll().split("\n");

        // en yuksek skoru bulmamıza yarıyor
        foreach(QString skor, skorListesi)
        {
            if(skor.toInt()>maxSkor)
            {
                maxSkor=skor.toInt();
            }
        }

        skordosyasi.close();

        //tek seferde okup ekleme yapamadıgım için tekrar dosyayı açıp skoru ekleme (Youtube)
        QFile skordosyasi1("C:/Users/okqn/Documents/Qt Creator/22100011067_Odev1/skorlar.txt");

        if (!skordosyasi1.open(QIODevice::Append))
        {
            QMessageBox::warning(this, "Error!", skordosyasi1.errorString());
            return;
        }

        QTextStream stream1(&skordosyasi1);
        stream1 << "\n"+ QString::number(kesilenKarpuzSayaci); //Bu kısım özellikle (Youtube)
        skordosyasi1.close();

        if (kesilenKarpuzSayaci>maxSkor)
        {
            QMessageBox::information(this, "Bilgi!", "Oyun Bitti! Tebrikler Maximum Skor Sizde!\nKesilen Meyve Sayısı: "+
            QString::number(kesilenKarpuzSayaci)+"\nKaçırılan Meyve Sayısı: "+
            QString::number(kacirilanKarpuzSayaci)+"\nMaximum Skor: "+
            QString::number(kesilenKarpuzSayaci),"Tamam");
            close();

        } else
        {
            QMessageBox::information(this, "Bilgi!", "Oyun Bitti! Maximum Skoru Geçemediniz!\nKesilen Meyve Sayısı: "+
            QString::number(kesilenKarpuzSayaci)+"\nKaçırılan Meyve Sayısı: "+
            QString::number(kacirilanKarpuzSayaci)+"\nMaximum Skor: "+
            QString::number(maxSkor), "Tamam");
            close();
        }
    }

}
