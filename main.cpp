#include <QCoreApplication>
#include <QImage>
#include <QRgb>
#include <QFile>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QImage image;
    image.load(argv[1]);

    QImage image2 = image.convertToFormat(QImage::Format_RGB16);

    unsigned short out[image.width() * image.height()];

    /*
    for(int y = 0; y < image.height(); y++)
    {
        for(int x = 0; x < image.width(); x++)
        {
            QColor color(image.pixel(x, y));
            out[x + y*image.width()] = ((color.red() >> 3 & 0b11111) << 11) | ((color.green() >> 2 & 0b111111) << 5) | ((color.blue() >> 3 & 0b11111)) ;
        }
    }*/

    memcpy(out, image2.bits(), image.width() * image.height() * 2);


    QImage result( (const uchar*)out, image.width(), image.height(), QImage::Format_RGB16);
    result.save("debug.png");


    for(int y = 0; y < image.height(); y++)
    {
        for(int x = 0; x < image.width(); x++)
        {
            int offset = x  + y*image.width();
            out[offset] = ((out[offset] & 0x00ff) << 8) | ((out[offset] & 0xff00) >> 8);
        }
    }

    QFile outf(argv[2]);
    outf.open(QFile::WriteOnly);
    outf.write((const char *)out, image.width() * image.height() * 2);
    outf.close();
}
