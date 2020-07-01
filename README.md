# Self Organising LEDs

This project uses an esp32cam board to drive a string of WS2118 LEDs. It uses some basic image processing to detect where the LEDs are so that you can display geometric patterns even if the LEDs are not arranged in a grid.

[![Demo Video](https://img.youtube.com/vi/Ueim2Ko8VWo/0.jpg)](https://www.youtube.com/watch?v=Ueim2Ko8VWo)

# Setup

You will need python3, node and yarn installed.

Create a virtual environment, activate it and install the requirements using:

```
python3 -m venv venv
source ./venv/bin/activate
pip install -r requirements.txt
```

You can now use the `Makefile` to build the firmware, frontend and upload them to your device.

```
make upload
```
