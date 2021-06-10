# Arduino Powered Cascading LED Stair Lights
Arduino controlled cascading LED under stair lights. Makes use of ultrasonic sensors to detect if someone is walking by the top/bottom of a staircase. Cascades the lights on and off in the appropriate direction.

Check out the accompanying blog post for this code [here](https://gidge.dev/stair%20lights/stair-lighting/) for more info and examples.


## Examples

![Example Up](https://gidge.dev/assets/posts/stair-lighting/build/bottom-up.gif)

![Example Down](https://gidge.dev/assets/posts/stair-lighting/build/bottom-down.gif)

## Parts to Modify for Your Own Buid
- Pin definintion: you'll need to match the number of pins to the number of stairs/lights you have.
- Pin definition number values: likewise, this'll have to be edited to match the pin numbers you use on your Arduino. Any beginner Arduino will cover this.
- lightup_time: if you're generally slow going up/down stairs, you'll want to lengthen this.
- interlight_delay: ditto for this one. Going too large/small for this value will result in an awkwardly fast/slow animation.
- cooldown_time: if you feel like the cooldown should be more generious, make this value larger.
- Distance thresholds (distance_top < x and distance_bot < y): minimum distance needed to be picked up by the sensors to start the animation. The bundled uss-calibration.ino will help you get these values.