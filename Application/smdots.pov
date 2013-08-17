// POVRay code used to render the dots used in:
//     BinaryClock 1.1 and BinaryClock ScreenSaver version 0.7, by David Enderson
// This code was initially written by a friend of mine, Jaeger (http://jaeger.festing.org).
// Then after I learned more about POVRay, I adapted it to suit my needs.
// This is intended to render a square image.  The images I rendered for the 
// screensaver were in resolutions of 100x100, and 150x150.
// to get it to work:
// 1. Install povray
// 2. Open a terminal and go to povray's directory
// 3. type: povray +Ismdots.pov +Osmdot.png +A2 +FN8 +W13 +H13 
// The only reason that shouldn't work is if you have settings in your povray.ini you need to adjust.

// for yellow
#declare dotcolor = color rgb <1.0, 1.0, 0.0>;
// for blue
//#declare dotcolor = color rgb <0.0, 0.0, 1.0>;
// for cyan
//#declare dotcolor = color rgb <0.0, 0.9, 1.0>;
// for red
//#declare dotcolor = color rgb <1.0, 0.0, 0.2>;
// for purple
//#declare dotcolor = color rgb <0.7, 0.2, 1.0>;
#declare ON=1;

sphere {
  < 0, 0, 0 >, 1
  pigment { color dotcolor }
  finish {
    phong .31
    phong_size 2
    ambient .13
  }
}

camera {
  location < 0, 0, -3 >
  look_at < 0, 0, 0 >
  right < .72, 0, 0>
  up < 0, .72, 0>
}

light_source {
  < 2, 2, -6 >
#if(ON = 1)
  color rgb <1.0, 1.0, 1.0>
#else
  color rgb <0.3, 0.3, 0.3>
#end
}