# Biomusic Osc To Midi Router
listens for specific OSC messages and route their 0-1 ranged values to midi cc on channel 1 mapped to values between 0 and 127

this is based on examples of PackOSC library and some very midi examples from planet ccrma

the code bare c++ so should be fast enough to on a modest machine like a raspberry pi.

## build
on a linux system, make sure to have dev tools installed and run `make`

## run
```
bin/biomusic </dev/snd/midiDevice> <oscPort>
```

## change mapping
head to `Biomusic.cpp`, change the mappings and run `make`.

