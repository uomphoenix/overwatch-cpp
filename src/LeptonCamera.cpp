/**
    LeptonCamera.cpp
    Based heavily on the work done by Frank Swenton, available at:
        https://github.com/PureEngineering/LeptonModule/
*/

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include <iostream>
#include <sstream>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"

#include "LeptonCamera.h"

LeptonCamera::LeptonCamera()
    : result(RowPacketBytes*FrameHeight)
    , rawData(FrameWords)
    , tx(RowPacketBytes, 0)
{
    mode = 0;
    bits = 8;

    speed = 16000000;
    delay = 0;

    initialised = false;

    #if HAVE_LEPTON
    if (initLepton())
    {
        std::cout << "Successfully initialized the Lepton SPI interface" << std::endl;
    }
    #endif

    errors = resets = 0;
}

LeptonCamera::~LeptonCamera()
{

}

#if HAVE_LEPTON
bool LeptonCamera::initLepton()
{
    fd = open(device, O_RDWR);
    if (fd < 0)
        std::cout << "Can't open device" << std::endl;
    else if (-1 == ioctl(fd, SPI_IOC_WR_MODE, &mode))
        std::cout << "Can't set SPI mode" << std::endl;
    else if (-1 == ioctl(fd, SPI_IOC_RD_MODE, &mode))
        std::cout << "Can't get SPI mode" << std::endl;
    else if (-1 == ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits))
        std::cout << "Can't set bits per word" << std::endl;
    else if (-1 == ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits))
        std::cout << "Can't get bits per word" << std::endl;
    else if (-1 == ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed))
        std::cout << "Can't set max speed" << std::endl;
    else if (-1 == ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed))
        std::cout << "Can't get max speed" << std::endl;
    else
    {
        initialised = true;
        return true;
    }
    return false;
}
#else
static int counter = 0;
#endif

int LeptonCamera::getPacket(int iRow, unsigned char *packetData)
{
#if HAVE_LEPTON
    _tr.rx_buf = (unsigned long) packetData;
    return ioctl(fd, SPI_IOC_MESSAGE(1), &_tr);
#else
    packetData[0] = 0;
    packetData[1] = iRow;
    for (int i = 4; i < RowPacketBytes; i += 2)
        *(short *)(packetData+i) = ((iRow+counter) % 20) + ((i/2-2) % 26);
    return 1;
#endif
}

bool LeptonCamera::getFrame(cv::Mat& frame)
{
#if HAVE_LEPTON
    if (!initialised)
    {
        throw SPIUnitialisedError();
    }

    usleep(250000);

    _tr.tx_buf = (unsigned long) &tx[0];
    _tr.len = RowPacketBytes;
    _tr.delay_usecs = delay;
    _tr.speed_hz = speed;
    _tr.bits_per_word = bits;
#endif

    int iRow;
    for (iRow = 0; iRow < FrameHeight; ) {
        unsigned char *packet = &result[iRow*RowPacketBytes];

        if (getPacket(iRow, packet) < 1) {
            std::cout << "Error transferring SPI packet" << std::endl;
            return false;
        }

        int packetNumber;
        if ((packet[0] & 0xf)==0xf)
            packetNumber = -1;
        else
            packetNumber = packet[1];

#if DEBUG_LEPTON
        if (sequence.empty() || sequence.back().first!=packetNumber)
            sequence.push_back(std::make_pair(packetNumber, 1));
        else
            ++sequence.back().second;
#endif

        if (packetNumber==-1) {
            usleep(1000);
            errors++;
            return false;
        }

        if (packetNumber != iRow) {
            usleep(1000);
            return false;
        }

        ++iRow;
    }

    if (iRow < FrameHeight) {
        if (++resets >= 750) {
            // If the reset counter hits 750, we should sleep and let the
            // Lepton recover before continuing
            std::cout << "Packet reset counter hit 750";
            resets = 0;
            usleep(750000);
        }
        return false;
    }

#if DEBUG_LEPTON
    std::stringstream os;
    bool chain = false, first = true; int chain0, chain1;
    for (std::list< std::pair<int, int> >::iterator iSeq = sequence.begin(); iSeq != sequence.end(); ++iSeq) {
        if (chain && iSeq->first==chain1+1) { ++chain1; continue; }
        if (chain && chain1!=chain0) os << "-" << chain1;
        if (iSeq->first >= 0 && !chain) { chain = true; chain0 = chain1 = iSeq->first; }
        if (first) first = false; else os << " ";
        if (iSeq->first==-1) os << "*"; else os << iSeq->first;
        if (iSeq->second!=1) { os << "^" << iSeq->second; chain = false; }
    }
    if (chain && chain1!=chain0) os << "-" << chain1;
    std::cout << os << std::endl;
    sequence.clear();
    // std::cout << resets << "resets," << errors << "errors";
#endif

    resets = 0; errors = 0;

    // Here we build our frame data at the same time as the internal rawData
    // buffer... To save on memory later it may be better to only be writing
    // to the frame's memory buffer.
    uint16_t minValue = 65535;
    uint16_t maxValue = 0;
    unsigned char *in = &result[0];
    unsigned short *out = &rawData[0];

    unsigned short *fp;

    for (int iRow = 0; iRow < FrameHeight; ++iRow) {
        in += 4;

        fp = frame.ptr<unsigned short>(iRow);

        for (int iCol = 0; iCol < FrameWidth; ++iCol) {
            unsigned short value = in[0];
            value <<= 8;
            value |= in[1];
            in += 2;
            if (value > maxValue) maxValue = value;
            if (value < minValue) minValue = value;
            *(out++) = value;

            fp[iCol] = value;
        }
    }

#if !HAVE_LEPTON
    usleep(50000);  // Need to slow things down if no ioctl call!
    counter = (counter + 1)%520;
#endif

    return true;
}
