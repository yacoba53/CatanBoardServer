# CatanBoardServer
A simple server for esp8266 to control lights on a DIY Catan Board


## Design

A RESTful webserver will be uploaded to the microcontroller. 
The open endpoints will control the various lighting functions 
for a string of individually addressable lights.

A hex is 4.5cm per side

Assumed token order from A-R is:  
[ 5,  2,  6,  3,  8,
 10,  9, 12, 11,  4,
  8, 10,  9,  4,  5,
  6,  3, 11]

### Endpoints

#### Set Board
	Will set resources on the board with the following frequency:

	* 1x Yellow (Desert)
	* 3x Gray (Ore)
	* 3x Red (Brick)
	* 3x Orange (Wheat)
	* 4x Green (Wood)
	* 4x Light Green (Sheep)

	Will flash the starting tile for 30s (first tile for numbered Tokens), 
	and assign token number values according to relative 
	position going counter-clockwise and inward.

	will assign each addressable LED its proper number value 2-12, 
	using 2 per hex up tp 38.


#### Roll Dice
	Will pick 2 random numbers from 1-6 each. 

	Will flash tiles with matching value for 15s.

	Will store the value result in rollFreqArr.
	
	Will store the resource result in resourceFreqArr.


#### Rainbow Spiral
	Will set the board to spiral with a rainbow pattern. 
	Not for gameplay, just for show.

## Box

### Parts

* [ESP8266](https://www.amazon.com/dp/B010O1G1ES/ref=twister_B086QGXBRW?_encoding=UTF8&psc=1)
* [WS2811 RGB LED Individually Addressable](https://www.amazon.com/gp/product/B076VBSB3B/ref=ppx_od_dt_b_asin_title_s00?ie=UTF8&psc=1)
* [Power Supply](https://www.amazon.com/gp/product/B078RT3ZPS/ref=ppx_od_dt_b_asin_title_s00?ie=UTF8&psc=1)
* Balsa Wood
* Frosted Plexiglass

### Tools

* Computer
* Soldering Iron
* jigsaw

### Box Pieces

Base (with hex walls)
* 1 x 45cmx45cm wood square base (to cut to hex base)
* 72 x 4.5cmx6cm balsa separators (with small wire hole in center)
* 4 x latches ( to hold top to bottom, but still be removable)

top (acrylic cover and walls)

* 6 x 26.5cmx6cm wood outer walls (4 with 17cmx4cm holes)
* 1 x 40cmx40cm acrylic square (to cut to hex top)
* 1 x 40cmx40cm balse square (to cut to hex top outline)
* 6 x 26.5cmx4.5cm wood top pieces

drawers (4 drawers)
* 8 x 18.5cmx5cm wood drawer front/back
* 8 x 4cm#x3cm drawer sides (#slightly smaller)
* 4 x 17cm#x3.5cm drawer bottoms (#slightly smaller)
* 4 x drawer knobs
