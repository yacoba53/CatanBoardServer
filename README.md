# CatanBoardServer
A simple server for esp8266 to control lights on a DIY Catan Board


## Design

A RESTful webserver will be uploaded to the microcontroller. The open endpoints will control the various lighting functions for a string of individually addressable lights.

A hex is 4.5cm per side

Assumed token order from A-R is:  
[ 5,  2,  6,  3,  8,
 10,  9, 12, 11,  4,
  8, 10,  9,  4,  5,
  6,  3, 11]

## Parts

* [ESP8266](https://www.amazon.com/dp/B010O1G1ES/ref=twister_B086QGXBRW?_encoding=UTF8&psc=1)
* [WS2811 RGB LED Individually Addressable](https://www.amazon.com/gp/product/B076VBSB3B/ref=ppx_od_dt_b_asin_title_s00?ie=UTF8&psc=1)
* [Power Supply](https://www.amazon.com/gp/product/B078RT3ZPS/ref=ppx_od_dt_b_asin_title_s00?ie=UTF8&psc=1)
* Balsa Wood
* Frosted Plexiglass

## Tools

* Computer
* Soldering Iron
* jigsaw


## Endpoints

### Set Board
	Will set resources on the board with the following frequency:

	* 1x Yellow (Desert)
	* 3x Gray (Ore)
	* 3x Red (Brick)
	* 3x Orange (Wheat)
	* 4x Green (Wood)
	* 4x Light Green (Sheep)

	Will flash the starting tile for 30s (first tile for numbered Tokens), and assign token number values according to relative position going counter-clockwise and inward.

	will assign each addressable LED its proper number value 2-12, using 2 per hex up tp 38.


### Roll Dice
	Will pick 2 random numbers from 1-6 each. Will store the result in rollFreqArr.
	Will flas
### Rainbow Spiral