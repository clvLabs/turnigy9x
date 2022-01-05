# Turnigy9X

Arduino library to read information from a `Turnigy 9X` RC controller.

## IMPORTANT

The whole library is based on `pulseIn()` which is **NOT** a good way to read the signals sent from the controller.

I made the library as a *proof of concept* to make sure I could do something else, and stopped working on it as I got to my goal.

Don't use it in a _production_ sketch, as it will eat most of your microcontroller time... once you see it work, start looking for a better library :)

