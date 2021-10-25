# Arduino Powered Cascading LED Stair Lights & Real-Time Bass Visualizer
Arduino controlled cascading LED under stair lights. Makes use of ultrasonic sensors to detect if someone is walking by the top/bottom of a staircase. Cascades the lights on and off in the appropriate direction.

Additional functionality to act as a real-time bass visualizer. See example video below.

Check out the accompanying blog posts for this code for more info and examples:

- [Part 1: Basic Cascade Functionality](https://gidge.dev/stair%20lights/stair-lighting/)
- [Part 2: Real-Time Bass Visualization](https://gidge.dev/stair%20lights/stair-lighting-pt2/)


## Examples

![Example Up](https://github.com/gidger/under-stair-lighting/blob/97d05713b8842876cc279ee3c0c744754c52273f/examples/bottom-up.gif)

![Example Down](https://github.com/gidger/under-stair-lighting/blob/97d05713b8842876cc279ee3c0c744754c52273f/examples/bottom-down.gif)

[<img src="https://github.com/gidger/under-stair-lighting/blob/d4d0b198ada6a9371a6818912574b9392130d69f/examples/youtube-thumbnail.jpg">](https://www.youtube.com/watch?v=AO4oNaiYNIw)


## Parts to Modify for Your Own Buid
### Standard Mode:
- led_pins[]: set to the specific pins to that will control individual LEDs.
- num_lights: total number of stair lights.
- Sensor pin numbers (sensor_bottom_trig, etc): set to the specific pins to that are used for the ultrasonic sensors.
- lightup_time: if you're generally slow going up/down stairs, you'll want to lengthen this.
- interlight_delay: ditto for this one. Going too large/small for this value will result in an awkwardly fast/slow animation.
- cooldown_time: if you feel like the cooldown should be more generious, make this value larger.
- Distance thresholds (distance_top < x and distance_bot < y): minimum distance needed to be picked up by the sensors to start the animation. The bundled uss-calibration.ino will help you get these values.

### Music Mode:
- mic_pin: pin number that microphone module is connected to.
- num_lights_music: number of lights to be used in music mode.
- noise_threshold: cude noise filter. Increase to decrease sensitivity, decrease for increased sensitivity.
- amplitude_divisor & bin_multipliers: used to convert amount of bass to number of stairs to illuminate. I reccomend looking in the music_mode() function to see how they're used. Decrease amplitude_divisor to increase the number of stairs to illuminate. Modify bin_multipliers with new values to effect how different frequencies influence number of stairs to illuminate. I reccomend tuning both of these for your setup.
- sample_count & sample_frequency: how many samples to use for the Fourier transform & what frequency to sample at. Your individual hardware will influence this. Take a look at [this video](https://www.youtube.com/watch?v=Mgh2WblO5_c) for a deeper dive into these.