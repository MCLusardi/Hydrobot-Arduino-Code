# Hydrobot-Arduino-Code
This file is a part of the Hydrobot Project, a collaboration between Landon Swartz (computer eng, main idea and design) and Maria Lusardi (computer sci, wrote software).
This project was completed for the Bio-Informatics in Plant Science (BIPS) program at the University of Missouri - Columbia under Dr. David Mendoza.

The Hydrobot is a set of tanks, sensors and software that monitors and controls the qualities of the solution that plants are grown in for experimentation purposes. The goal is
that by having the software and machine manage the solutions in the tanks, the risk of contamination can be reduced and the correct levels of dissolved oxygen, pH and electrical 
conductivity can be more precise.

In total, there will be three tanks and each tank will have three sensors: a dissolved oxygen sensor (DO), an electrical conductivity sensor (EC), and a pH sensor (PH or pH). Each
tank will have one arduino Mega which runs the script that reads in values from these three sensors. There will be one raspberry pi connected to all three arduinos which processes
their input and sends data to the server (CyVerse). From there, the data can be used for analysis.

This particular folder contains test scripts for the arduinos only. The purposes of each script are described in comments at the top of each code. Scripts labeled 'sample-code' 
were taken directly from Atlas Scientific, from whom the sensors were purchased. This sample code has been adapted for the purposes of this project.
