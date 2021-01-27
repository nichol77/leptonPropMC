# leptonPropMC
Old Code to Simulate Propagations of Ultra-High Energy Leptons from Neutrino Interactions

## Warning: Old Old Old Code last touched in 2007
Currently this code is in a derelict state and does not compile. Probably it wouldn't take very long to update the Makefile to support a modern compiler and a modern version of ROOT.


### Propagator and Neutrino Event
The Propagator class is the heart of the simulation and takes an intial neutrino (specified by flavour, energy, position and direction) and propagates the neutirno allowing for the neutrino to undergo interactiosn and change into a charged lepton (via weak neutral current) that can then be followed.

The actual interactions are all coded up in the delightfully undocumented interactions.cxx file.

