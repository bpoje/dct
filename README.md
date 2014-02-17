C++ app using Qt4:
* Discrete cosine transform (2D DCT) of image
* Discrete Fourier transform (2D DFT) of image
* Filter (convolution) images using 2D DFT or 2D DCT

Both DFT and DCT are calculated using FFT.

Packages:
```
sudo apt-get install qt4-qmake libqt4-dev build-essential
```

Compile:
```
qmake -project "QMAKE_CXXFLAGS += -std=c++0x"
qmake
make
```

Screenshot:
![Alt text](/images/screen.jpeg "Screenshot")
