#ifndef LEPTONCAMERA_H
#define LEPTONCAMERA_H

#define HAVE_LEPTON false
#define DEBUG_LEPTON false

#include <stdlib.h>
#include <vector>
#include <exception>

#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "opencv2/highgui/highgui.hpp"

#if DEBUG_LEPTON
#include <list>
#include <utility>
#endif

class LeptonCamera
{
    public:
        LeptonCamera();
        virtual ~LeptonCamera();

        bool initLepton();
        int getPacket(int, unsigned char *);

        bool getFrame(cv::Mat *);

        enum
        {
            FrameWidth = 80,
            FrameHeight = 60,
            RowPacketWords = FrameWidth + 2,
            RowPacketBytes = 2*RowPacketWords,
            FrameWords = FrameWidth*FrameHeight
        };
    protected:
    private:
        const char *device = "/dev/spidev0.1"; // Change to 0.1 if necessary!
        unsigned char mode;
        unsigned char bits;
        unsigned int speed;
        unsigned short delay;

        bool initialised;

        std::vector<unsigned char> result;
        std::vector<unsigned short> rawData;

        int fd;
        struct spi_ioc_transfer _tr;
        std::vector<unsigned char> tx;

        #if DEBUG_LEPTON
        std::list< std::pair<int, int> > sequence; // ...of packet #'s received from Lepton, for troubleshooting
        #endif
};

class SPIUnitialisedError : public std::exception
{
    virtual const char *what() const throw()
    {
        return "Cannot get frame when the SPI interface is unitialised";
    }
};

#endif // LEPTONCAMERA_H
