# background-noise-toy-model-root

###### Introduction to Nuclear and Particle Physics - stats lab project 
###### Yachay Tech University

> "This is the physics version of the discovery of DNA."
> --Peter Knight, president of Britain's Institute of Physics

A root code for fitting and anylizing a Higgs-like signal from experimental measurements. Distinguishes between backgroung signals (quark-antiquark jets) from real H boson peaks, and fits the different statistical parameters for each plot)

You must have [ROOT - Data Analysis Framework](https://root.cern.ch/downloading-root) installed.

Once set up, you should provide some data to the algorithm to start the fitting.

For running the code under Linux/MacOS, place it in a separated folder:

```
$ cd 
$ mkdir toy_model
```

Dowload the required files in the new folder.

```
$ wget https://github.com/joshuamsalazar/background-noise-toy-model-root/blob/master/toy.c
$ wget https://github.com/joshuamsalazar/background-noise-toy-model-root/blob/master/toy_discovery.root
```

And then run the root code:

```
$ root -l toy.c
```
It will automatically generate output files where the fit parameters are specified.


###### Credits: 
- Anthony Ramos
- Cynthia Arias
- Carlos Cocha
- Juan Daniel Torres
- Iskra Zambrano
- Andrés Vega
- Juan David Alcivar
- Génesis Mendoza
- Carla Borja
- Eduardo López



Licence: ![](https://licensebuttons.net/l/by-nc-sa/4.0/88x31.png)

Creative Commons - Share your work, when we share everyone wins!
